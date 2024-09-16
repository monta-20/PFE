#include "mainwindow.h"
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("QtWebSockets example: echoclient");
    //Sets a descriptive text displayed when the user runs the program with -h or --help:MainWindow -h
    parser.addHelpOption();
    //Adds a standard help option with usage information accessible using -h or --help.
    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
                                 QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);

    //Adds the debug option to the parser -d or --debug
    parser.process(a);
     //bool debug = parser.isSet(dbgOption);
    //Checks if the "debug" option was present in the arguments.
    //MainWindow client(QUrl(QStringLiteral("ws://localhost:8080")), true);

   // MainWindow cl(QUrl(QStringLiteral("ws://localhost:1234")), true);
    // QObject::connect(&client, &MainWindow::closed, &a, &QCoreApplication::quit);
    // QObject::connect(&cl, &MainWindow::closed, &a, &QCoreApplication::quit);
    //Connects the closed signal of the client object to the quit slot of the

    //QCoreApplication. This ensures the application exits when the WebSocket connection
    //closes.
    MainWindow client(QUrl(QStringLiteral("ws://localhost:1234")),QUrl(QStringLiteral("ws://localhost:8080")), true);
    client.show();
    return a.exec();
}
