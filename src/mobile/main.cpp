#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "SpyTank.h"
#include "VideoReceiver.h"
#include "QmlVideoProducer.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Register SpyTank enums and class
    qmlRegisterType<SpyTank>("com.spytank", 1, 0, "SpyTank");
    qmlRegisterUncreatableType<QAbstractSocket>("com.spytank", 1, 0, "SocketState", "Access to SocketState enum");

    SpyTank tank;
    VideoReceiver video;
    QmlVideoProducer videoProducer;

    QObject::connect(&video, &VideoReceiver::frameReceived, &videoProducer, &QmlVideoProducer::handleFrame);

    engine.rootContext()->setContextProperty("tank", &tank);
    engine.rootContext()->setContextProperty("video", &video);
    engine.rootContext()->setContextProperty("videoProducer", &videoProducer);

    using namespace Qt::StringLiterals;
    const QUrl url(u"qrc:/qt/qml/com/spytank/Main.qml"_s);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
