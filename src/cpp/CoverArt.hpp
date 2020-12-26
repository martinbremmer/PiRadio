#ifndef COVERART_H
#define COVERART_H

#include <mutex>
#include <thread>
#include <condition_variable>

#include <QTimer>
#include <QObject>
#include <QString>
#include <QStringList>

class CoverArt : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString artist READ artist WRITE artist NOTIFY artistChanged);
    Q_PROPERTY(QString title  READ title  WRITE title  NOTIFY titleChanged );
    Q_PROPERTY(QString image  READ image  WRITE image  NOTIFY imageChanged );
public:
    explicit CoverArt(QObject *parent = nullptr);
    ~CoverArt();

    QString artist() const;
    void artist(QString name);

    QString title() const;
    void title(QString song);

    QString image() const;
    void image(QString url);

public:
signals:
    void artistChanged(QString);
    void titleChanged(QString);
    void imageChanged(QString);

private slots:
    void imagePoller();

private:
    QString m_artist;
    QString m_title;
    QString m_image;

    mutable std::mutex m_mutex;
    std::condition_variable m_condvar;

    /* Getting cover art information can take quite a long time.
     * Don't block UI, but do it in a separate thread.
     * But the properties should still be updated in the 'main'
     * QML thread (in which m_timer lives). */
    bool m_terminate;
    QTimer m_coverTimer;
    QString m_cover64;
    std::thread m_coverThread;
    void coverWorker();
    QStringList coverIDs(const QString& artist, const QString& title);
    QString coverMetaValue(const QString& key, const QString& line);
    QString cover64(const QStringList& ids);
};

#endif /* COVERART_H */
