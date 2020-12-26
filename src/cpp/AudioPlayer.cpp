#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h>

#include "AudioPlayer.hpp"

AudioPlayer::AudioPlayer(QObject *parent) :
    QObject(parent),
    m_vlcInstance(nullptr),
    m_vlcPlayer(nullptr),
    m_vlcMedia(nullptr),
    m_metaTerminate(false),
    m_metaTimer(this),
    m_metaThread(&AudioPlayer::metaWorker, this)
{
    this->m_vlcInstance = libvlc_new(0, NULL);
    if (!this->m_vlcInstance) {
        throw std::runtime_error("Could not create vlc instance.");
    }

    /* Start meta info monitor polling timer. */
    QObject::connect(&(this->m_metaTimer), SIGNAL(timeout()), this, SLOT(metaPoller()));
    this->m_metaTimer.start(500);
}

AudioPlayer::~AudioPlayer()
{
    this->m_metaTerminate = true;
    this->m_metaTimer.stop();
    this->stop();
    if (this->m_vlcInstance) {
        libvlc_release(this->m_vlcInstance);
    }
    this->m_metaThread.join();
}

QString AudioPlayer::stream() const
{
    return this->m_stream;
}

void AudioPlayer::stream(QString url)
{
    if (url == this->m_stream) {
        return;
    }
    const std::lock_guard<std::mutex> lock(this->m_mutex);
    this->m_metaArtist = "";
    this->m_metaTitle  = "";
    this->artist("");
    this->title("");
    this->stop();
    this->m_stream = url;
    this->play();
    emit streamChanged(url);
}

QString AudioPlayer::artist() const
{
    return this->m_artist;
}

void AudioPlayer::artist(QString name)
{
    if (name == this->m_artist) {
        return;
    }
    this->m_artist = name;
    emit artistChanged(name);
}

QString AudioPlayer::title() const
{
    return this->m_title;
}

void AudioPlayer::title(QString song)
{
    if (song == this->m_title) {
        return;
    }
    this->m_title = song;
    emit titleChanged(song);
}


void AudioPlayer::play()
{
    std::string url = this->m_stream.toStdString();
    std::cout << "AudioPlayer::play(" << url << ")" << std::endl;

    this->m_vlcMedia = libvlc_media_new_location(this->m_vlcInstance, url.c_str());
    if (!this->m_vlcMedia) {
        std::cout << "Could not open url: " << url << std::endl;
        return;
    }

    this->m_vlcPlayer = libvlc_media_player_new_from_media(this->m_vlcMedia);
    if (!this->m_vlcPlayer) {
        std::cout << "Could not create player for: " << url << std::endl;
        return;
    }

    if (libvlc_media_player_play(this->m_vlcPlayer) != 0) {
        std::cout << "Could not start: " << url << std::endl;
    }
}

void AudioPlayer::stop()
{
    if (this->m_vlcPlayer) {
        libvlc_media_player_stop(this->m_vlcPlayer);
        libvlc_media_player_release(this->m_vlcPlayer);
        this->m_vlcPlayer = nullptr;
    }
    if (this->m_vlcMedia) {
        libvlc_media_release(this->m_vlcMedia);
        this->m_vlcMedia = nullptr;
    }
}

void AudioPlayer::metaPoller()
{
    /* Set properties to the found artist and title. */
    const std::lock_guard<std::mutex> lock(this->m_mutex);
    this->artist(this->m_metaArtist);
    this->title(this->m_metaTitle);
}

void AudioPlayer::metaWorker()
{
    while (!this->m_metaTerminate) {
        QString metaArtist;
        QString metaTitle;
        QString metaNow;

        this->m_mutex.lock();
        QString url = this->m_stream;
        this->m_mutex.unlock();

        /* Try getting 'now playing on stream' from vlc. */
        metaNow = this->metaVlcNow();

        if (metaNow.length() == 0) {
            /* Try getting 'now playing on stream' from ffprobe. */
            metaNow = this->metaFFNow(url);
        }

        if (metaNow.length() > 0) {
            /* Extract 'artist' and 'title' from 'now playin on stream'. */
            int separator = metaNow.indexOf(" - ");
            metaArtist = metaNow.left(separator);
            metaTitle  = metaNow.right((metaNow.size() - separator) - 3);
        }

        if (metaArtist.length() == 0) {
            /* With a stream, 'artist' is often empty.
             * But we don't have one yet, so try anyway. */
            metaArtist = this->metaVlcArtist();
        }

        if (metaTitle.length() == 0) {
            /* With a stream, 'title' is often the radio name.
             * But we don't have one yet, so try anyway. */
            metaTitle = this->metaVlcTitle();
        }

        this->metaUpdate(url, metaArtist, metaTitle);

        sleep(5);
    }
}

QString AudioPlayer::metaVlcArtist()
{
    const std::lock_guard<std::mutex> lock(this->m_mutex);
    if (this->m_vlcMedia) {
        return QString(libvlc_media_get_meta(this->m_vlcMedia, libvlc_meta_Artist));
    }
    return QString();
}

QString AudioPlayer::metaVlcTitle()
{
    const std::lock_guard<std::mutex> lock(this->m_mutex);
    if (this->m_vlcMedia) {
        return QString(libvlc_media_get_meta(this->m_vlcMedia, libvlc_meta_Title));
    }
    return QString();
}

QString AudioPlayer::metaVlcNow()
{
    const std::lock_guard<std::mutex> lock(this->m_mutex);
    if (this->m_vlcMedia) {
        return QString(libvlc_media_get_meta(this->m_vlcMedia, libvlc_meta_NowPlaying));
    }
    return QString();
}

QString AudioPlayer::metaFFNow(const QString& url)
{
    if (url.size() == 0) {
        return QString();
    }

    /* Execute ffprobe. */
    std::string ffprobe = std::string("ffprobe ") + url.toStdString() + std::string(" 2>&1");
    FILE *pipe = popen(ffprobe.c_str(), "r");
    if (!pipe) {
        perror("Cannot open pipe.");
        return QString();
    }

    /* Extract ffprobe output. */
    std::string result;
    char* buffer = new char[512];
    while ((fgets(buffer, sizeof(buffer), pipe)) != NULL) {
        result += buffer;
    }

    /* Closing ffprobe pipe. */
    int ret = pclose(pipe);
    if (ret != 0) {
        /* Program ends with error, and result has the error message. */
        std::cerr << "ret: " << ret << "\nMessage: " << result << std::endl;
        return QString();
    }

    /* Run through ouput lines. */
    std::string streamTitle;
    std::string streamTitleKey("    StreamTitle     : ");
    std::istringstream iss(result);
    for (std::string line; std::getline(iss, line); ) {
        size_t pos = line.find(streamTitleKey);
        if (pos != std::string::npos) {
            streamTitle = line.substr(streamTitleKey.size());
            break;
        }
    }

    /* Use streamTitle as 'now playing'. */
    return QString(streamTitle.c_str());
}

QString AudioPlayer::metaFormat(const QString& str)
{
    QString formatted;
    /* Every word starts with upper, rest lower. */
    QStringList parts = str.toLower().split(' ');
    for (int i = 0; i < parts.size(); ++i) {
        QString &part = parts[i];
        if (part.size() > 0) {
            if (formatted.size() > 0) {
                formatted.append(" ");
            }
            part.replace(0, 1, part[0].toUpper());
            formatted.append(part);
        }
    }
    return formatted;
}

void AudioPlayer::metaUpdate(const QString& url, const QString& artist, const QString& title)
{
    const std::lock_guard<std::mutex> lock(this->m_mutex);
    /* Check if the stream hasn't changed during the meta acquiring. */
    if (url == this->m_stream) {
        this->m_metaArtist = this->metaFormat(artist);
        this->m_metaTitle  = this->metaFormat(title);
    }
}
