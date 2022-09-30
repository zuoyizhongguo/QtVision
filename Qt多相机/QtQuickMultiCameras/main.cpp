#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "hub.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //与engine有关的，要放在load之前
    Hub hub;
    engine.rootContext()->setContextProperty("hub",&hub);       //暴露给qml
    engine.addImageProvider("live",&hub.m_ImgProvider);         //给qml供图的类对象

    const QUrl url(u"qrc:/QtQuickMultiCameras/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);                                           //engine相关设置，放在load之前

    return app.exec();
}
