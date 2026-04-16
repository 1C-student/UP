#include <QApplication>
#include <QDir>
#include <QDebug>
#include "authwindow.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Working directory:" << QDir::currentPath();
    qDebug() << "Application path:" << QCoreApplication::applicationDirPath();

    QDir dir("photos");
    qDebug() << "photos folder exists:" << dir.exists();
    if (dir.exists()) {
        qDebug() << "photos folder contents:" << dir.entryList();
    }

    if (!dir.exists()) dir.mkdir("photos");

    if (!Database::getInstance().connect()) return -1;

    AuthWindow w;
    w.show();

    return a.exec();
}
