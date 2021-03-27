#include "mainwindow.h"

#include <QApplication>
#include <QRemoteObjectNode>

int main(int argc, char *argv[])
{
    // Prepare application and resources and lauch window.
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resource);
    MainWindow w;
    w.show();
    return a.exec();
}
