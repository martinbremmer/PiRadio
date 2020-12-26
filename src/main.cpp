#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "cpp/CoverArt.hpp"
#include "cpp/AudioPlayer.hpp"
#include "cpp/RadioStation.hpp"
#include "cpp/RadioStations.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CoverArt>("PiRadio.CoverArt", 1, 0, "CoverArt");
    qmlRegisterType<AudioPlayer>("PiRadio.AudioPlayer", 1, 0, "AudioPlayer");
    qmlRegisterType<RadioStation>("PiRadio.RadioStation", 1, 0, "RadioStation");
    qmlRegisterType<RadioStations>("PiRadio.RadioStations", 1, 0, "RadioStations");

    const QUrl url(QStringLiteral("qrc:/src/main.qml"));
    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &QGuiApplication::quit);
    engine.load(url);

    return app.exec();
}
