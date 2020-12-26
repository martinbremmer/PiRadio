#ifndef RADIOSTATION_H
#define RADIOSTATION_H

#include <QObject>
#include <QString>
//#include "QmlHelpers.hpp"

class RadioStation : public QObject
{
    Q_OBJECT
public:
    explicit RadioStation(QObject *parent = nullptr);
    explicit RadioStation(const QString& name, const QString& stream, const QString& icon, QObject *parent = nullptr);

public:
    Q_PROPERTY(QString name READ name CONSTANT);
    Q_PROPERTY(QString stream READ stream CONSTANT);
    Q_PROPERTY(QString icon READ icon CONSTANT);

    QString name();
    QString stream();
    QString icon();

private:
    QString m_name;
    QString m_stream;
    QString m_icon;
};

#endif /* RADIOSTATION_H */
