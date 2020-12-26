#include "RadioStation.hpp"

RadioStation::RadioStation(QObject *parent) :
    QObject(parent)
{
}

RadioStation::RadioStation(const QString& name, const QString& stream, const QString& icon, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_stream(stream),
    m_icon(icon)
{
}

QString RadioStation::name()
{
    return this->m_name;
}

QString RadioStation::stream()
{
    return this->m_stream;
}

QString RadioStation::icon()
{
    return this->m_icon;
}
