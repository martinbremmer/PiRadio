#include <iostream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "RadioStations.hpp"

RadioStations::RadioStations(QObject *parent) :
    QObject(parent),
    m_current(nullptr)
{
    this->load();
}

RadioStations::~RadioStations()
{
    for (int i = 0; i < this->m_stations.count(); ++i) {
        delete this->m_stations[i];
    }
}

QQmlListProperty<RadioStation> RadioStations::stations()
{
    return { this, this->m_stations };
}


RadioStation* RadioStations::current() const
{
    if (!this->m_current) {
        throw std::runtime_error("Wierdly, there's no current selected radio station.");
    }
    return this->m_current;
}

void RadioStations::current(RadioStation* radio)
{
    if (!radio || (radio == this->m_current)) {
        return;
    }
    this->m_current = radio;
    emit currentChanged(radio);
}

void RadioStations::load()
{
    /* Read json file. */
    QString filename("res/stations.json");
    QJsonParseError err;
    QString json;
    QFile file;
    file.setFileName(filename);
    if (!(file.open(QIODevice::ReadOnly | QIODevice::Text))) {
        throw std::runtime_error("stations.json: Unavailable");
    }
    json = file.readAll();
    file.close();

    /* Parse and get root element. */
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8(), &err);
    if (doc.isNull()) {
        throw std::runtime_error(std::string("stations.json: [error] ") + err.errorString().toStdString());
    }
    if (!doc.isArray()) {
        throw std::runtime_error("stations.json: Root element is not an array.");
    }

    /* Run through all stations. */
    QJsonArray array = doc.array();
    for (int i = 0; i < array.size(); ++i) {
        this->loadStation(array[i].toObject());
    }
    if (this->m_stations.size() == 0) {
        throw std::runtime_error("stations.json: No radio stations found.");
    }

    /* TODO: Get selected radio of previous run (which is most likely the preferred radio station at the moment). */
    this->m_current = this->m_stations[0];
}

void RadioStations::loadStation(const QJsonObject &obj)
{
    /* Get json elements. */
    QJsonValue name   = obj["name"];
    QJsonValue icon   = obj["icon"];
    QJsonValue stream = obj["stream"];

    /* Check json element types. */
    if (!(name.isString())  ||
        !(icon.isString())  ||
        !(stream.isString()) )
    {
        throw std::runtime_error("stations.json: Invalid station element type.");
    }

    /* Create a station from the json information. */
    RadioStation* station =
            new RadioStation(name.toString(), stream.toString(), icon.toString());

    /* Store the station in the list. */
    this->m_stations.append(station);
}
