#include <QApplication>
#include <QQmlApplicationEngine>
#include "webServer.h"
#include "server.h"
#include "FileSystem.h"
#include "loger.h"
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    // Step 1: get access to the root object

    QObject *rootObject = engine.rootObjects().first();

    WebServer webServer(rootObject);
    Server tcpServer(rootObject);
    FileSystem fileSystem;

    Loger::rootView=rootObject;

    QMetaObject::invokeMethod(rootObject, "setCurrentPath",
            Q_ARG(QVariant,  QDir::currentPath()));

    // Step 2: connect qml signal to C++ slot
    QObject::connect(rootObject,SIGNAL(viewClicked(QString)), &tcpServer, SLOT(write(QString)));
    QObject::connect(rootObject,SIGNAL(createConnectionTCP(QString, int)), &tcpServer, SLOT(createConection(QString, int)));
    QObject::connect(rootObject,SIGNAL(sendMessageTCP(QString)), &tcpServer, SLOT(write(QString)));
    QObject::connect(rootObject,SIGNAL(saveLogToFile(QString, QString)), &fileSystem, SLOT(write(QString, QString)));


    QObject::connect(rootObject,SIGNAL(createConnectionWS(QString, int)), &webServer, SLOT(createConection(QString, int)));
    QObject::connect(rootObject,SIGNAL(sendMessageWS(QString)), &webServer, SLOT(write(QString)));

    return app.exec();
}
