#ifndef RADIOSTATIONS_H
#define RADIOSTATIONS_H

#include <QtQml>
#include <QObject>
#include <QVector>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlListProperty>
#include "RadioStation.hpp"

class RadioStations : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<RadioStation> stations READ stations CONSTANT);
    Q_PROPERTY(RadioStation* current READ current WRITE current NOTIFY currentChanged);
public:
    explicit RadioStations(QObject *parent = nullptr);
    ~RadioStations();

    void load();

    QQmlListProperty<RadioStation> stations();

    RadioStation* current() const;
    void current(RadioStation* radio);

signals:
    void currentChanged(RadioStation*);

private:
    void loadStation(const QJsonObject &obj);

    RadioStation *m_current;
    QList<RadioStation *> m_stations;
};

#endif /* RADIOSTATIONS_H */
