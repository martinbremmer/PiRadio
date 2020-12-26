#include <string>
#include <sstream>
#include <ostream>
#include <iostream>
#include <unistd.h>

#include "Base64.hpp"
#include "CoverArt.hpp"

#include "musicbrainz5/PUID.h"
#include "musicbrainz5/Query.h"
#include "musicbrainz5/HTTPFetch.h"

#include "coverart/CoverArt.h"
#include "coverart/HTTPFetch.h"

CoverArt::CoverArt(QObject *parent) :
    QObject(parent),
    m_terminate(false),
    m_coverTimer(this),
    m_coverThread(&CoverArt::coverWorker, this)
{
    /* Start cover art monitor polling timer. */
    QObject::connect(&(this->m_coverTimer), SIGNAL(timeout()), this, SLOT(imagePoller()));
    this->m_coverTimer.start(500);
}

CoverArt::~CoverArt()
{
    this->m_terminate = true;
    this->m_condvar.notify_all();
    this->m_coverTimer.stop();
    this->m_coverThread.join();
}

QString CoverArt::artist() const
{
    const std::lock_guard<std::mutex> lock(this->m_mutex);
    return this->m_artist;
}

void CoverArt::artist(QString name)
{
    std::unique_lock<std::mutex> lock(this->m_mutex);
    if (name == this->m_artist) {
        return;
    }
    this->m_artist = name;
    this->m_condvar.notify_all();
    this->m_cover64 = "";
    lock.unlock();
    this->image("");
    emit artistChanged(name);
}

QString CoverArt::title() const
{
    const std::lock_guard<std::mutex> lock(this->m_mutex);
    return this->m_title;
}

void CoverArt::title(QString song)
{
    std::unique_lock<std::mutex> lock(this->m_mutex);
    if (song == this->m_title) {
        return;
    }
    this->m_title = song;
    this->m_condvar.notify_all();
    this->m_cover64 = "";
    lock.unlock();
    this->image("");
    emit titleChanged(song);
}

QString CoverArt::image() const
{
    const std::lock_guard<std::mutex> lock(this->m_mutex);
    return this->m_image;
}

void CoverArt::image(QString img)
{
    std::unique_lock<std::mutex> lock(this->m_mutex);
    if (img == this->m_image) {
        return;
    }
    this->m_image = img;
    lock.unlock();
    emit imageChanged(img);
}

void CoverArt::imagePoller()
{
    /* Set properties to the found artist and title. */
    std::unique_lock<std::mutex> lock(this->m_mutex);
    QString base64(this->m_cover64);
    lock.unlock();
    this->image(base64);
}

void CoverArt::coverWorker()
{
    QString artist;
    QString title;
    while (!this->m_terminate) {
        std::unique_lock<std::mutex> lock(this->m_mutex);
        /* Don't wait if the artist or title changed. */
        if ((artist == this->m_artist) || (title == this->m_title)) {
            if (this->m_cover64.size() > 0) {
                /* Wait until new artist/title or termination. */
                this->m_condvar.wait(lock);
            } else {
                /* No cover. Maybe due to bad connection. Try again over 10 seconds. */
                this->m_condvar.wait_for(lock, std::chrono::seconds(10));
            }
            if (this->m_terminate) {
                break;
            }
        }
        artist = this->m_artist;
        title  = this->m_title;
        lock.unlock();

        /* Nothing to search for if there's no artist or title. */
        if ((artist.size() == 0) || (title.size() == 0)) {
            continue;
        }

        /* Get various GUIDs from musicbrainz. */
        QStringList ids = this->coverIDs(artist, title);
        /* Try getting a cover from one of those GUIDs. */
        QString base64 = this->cover64(ids);

        lock.lock();
        /* Don't use this image if artist/title has changed in the mean time. */
        if ((artist == this->m_artist) && (title == this->m_title)) {
            this->m_cover64 = base64;
        }
    }
}


QStringList CoverArt::coverIDs(const QString& artist, const QString& title)
{
    QStringList ids;
    QStringList all_ids;
    QStringList album_ids;
    QStringList single_ids;

    MusicBrainz5::CQuery Query("PiRadio-1.0");

    try {
        std::string search = std::string("artist:\"")+artist.toStdString()+
                                "\" AND recording:\""+title.toStdString()+
                                "\" AND status:official";

        MusicBrainz5::CQuery::tParamMap params;
        params["query"] = search;
        params["limit"] = "10";

        MusicBrainz5::CMetadata metadata = Query.Query("recording","","",params);
        std::stringstream metastringstream;
        metastringstream << metadata;
        QString id_key("ID:");
        QString pt_key("PrimaryType:");
        for (std::string l; std::getline(metastringstream, l); ) {
            QString line(l.c_str());
            QString id;
            QString pt;
            if (line.contains(id_key)) {
                id = this->coverMetaValue(id_key, line);
                if ((id.size() > 0) && (!all_ids.contains(id))) {
                    all_ids.append(id);
                }
            } else if (line.contains(pt_key)) {
                /* Check if the last ID was either Album or Single. */
                pt = this->coverMetaValue(pt_key, line);
                if ((pt == "Album") && (!album_ids.contains(id))) {
                    album_ids.append(all_ids.last());
                } else if ((pt == "Single") && (!single_ids.contains(id))) {
                    single_ids.append(all_ids.last());
                }
            }
        }
    } catch (MusicBrainz5::CExceptionBase& Error) {
        std::cout << "MusicBrainz Exception: '" << Error.what() << "'" << std::endl;
        std::cout << "LastResult: " << Query.LastResult() << std::endl;
        std::cout << "LastHTTPCode: " << Query.LastHTTPCode() << std::endl;
        std::cout << "LastErrorMessage: " << Query.LastErrorMessage() << std::endl;
    }

    /* When getting the covers, first try Albums, then Singles, then the rest. */
    ids.append(album_ids);
    ids.append(single_ids);
    for (int i = 0; i < all_ids.size(); i++) {
        if (!ids.contains(all_ids[i])) {
            ids.append(all_ids[i]);
        }
    }

    return ids;
}

QString CoverArt::coverMetaValue(const QString& key, const QString& line)
{
    QString val;
    int key_start = line.indexOf(key);
    int key_end   = key_start + key.size();
    int val_start = key_end;
    while ((val_start < line.size()) && (line[val_start].isSpace())) {
        val_start++;
    }
    if (val_start < line.size()) {
        val = line.right(line.size() - val_start);
    }
    return val;
}

QString CoverArt::cover64(const QStringList& ids)
{
    for (int i = 0; i < ids.size(); i++) {
        const QString &id = ids[i];

        CoverArtArchive::CCoverArt CoverArt("PiRadio-1.0");

        try {
            std::vector<unsigned char> imageData = CoverArt.FetchFront(id.toStdString());
            if (imageData.size() > 0) {
                std::string image64 = "data:image/jpg;base64,";
                image64 += Base64::encode(imageData);
                return QString(image64.c_str());
            }
        } catch (CoverArtArchive::CResourceNotFoundError& Error) {
            /* Ignore 'not found', because that's expected quite often. */
        } catch (CoverArtArchive::CExceptionBase& Error) {
            std::cout << "CoverArtArchive Exception: '" << Error.what() << "'" << std::endl;
            std::cout << "LastResult: " << CoverArt.LastResult() << std::endl;
            std::cout << "LastHTTPCode: " << CoverArt.LastHTTPCode() << std::endl;
            std::cout << "LastErrorMessage: " << CoverArt.LastErrorMessage() << std::endl;
        }
    }
    return "";
}
