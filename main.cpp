#include "mainwindow.h"

#include <QApplication>
#include "log.h"

int main(int argc, char *argv[])
{
    QT_LOG::logInit(QString(argv[0]) + ".log");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
