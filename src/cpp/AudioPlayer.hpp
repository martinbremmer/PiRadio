#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <mutex>
#include <thread>

#include <QTimer>
#include <QObject>
#include <QString>
#include <QThread>
#include <QQmlContext>
#include <QQmlListProperty>

#include <vlc/vlc.h>

class AudioPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString stream READ stream WRITE stream NOTIFY streamChanged);
    Q_PROPERTY(QString artist READ artist WRITE artist NOTIFY artistChanged);
    Q_PROPERTY(QString title  READ title  WRITE title  NOTIFY titleChanged );
public:
    explicit AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer();

    QString stream() const;
    void stream(QString url);

    QString artist() const;
    void artist(QString name);

    QString title() const;
    void title(QString song);

public:
signals:
    void streamChanged(QString);
    void artistChanged(QString);
    void titleChanged(QString);

private slots:
    void metaPoller();

private:
    void play();
    void stop();

    QString m_stream;
    QString m_artist;
    QString m_title;

    libvlc_instance_t     *m_vlcInstance;
    libvlc_media_player_t *m_vlcPlayer;
    libvlc_media_t        *m_vlcMedia;

    std::mutex m_mutex;

    /* Getting meta information can take quite a long time.
     * Don't block UI, but do it in a separate thread.
     * But the properties should still be updated in the 'main'
     * QML thread (in which m_timer lives). */
    bool m_metaTerminate;
    QTimer m_metaTimer;
    QString m_metaArtist;
    QString m_metaTitle;
    std::thread m_metaThread;
    void metaWorker();
    QString metaVlcArtist();
    QString metaVlcTitle();
    QString metaVlcNow();
    QString metaFFNow(const QString& url);
    QString metaFormat(const QString& str);
    void metaUpdate(const QString& url, const QString& artist, const QString& title);
};

#endif /* AUDIOPLAYER_H */
