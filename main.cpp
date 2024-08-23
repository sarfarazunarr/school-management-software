#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSplashScreen>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create a splash screen
    QPixmap pixmap(":/new/images/C:/Users/Administrator/Downloads/splash.jpeg");
    QSplashScreen splash(pixmap);
    splash.show();

    // Create a connection to the SQLite database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("school.db");

    if (!db.open()) {
        qDebug() << "Error: Could not open database." << db.lastError();
        return -1;
    } else {
        qDebug() << "Database opened successfully!";
    }

    // Simulate some loading time
    QThread::sleep(4); // sleep for 3 seconds

    // Initialize the main window
    MainWindow w;
    w.show();

    // Close the splash screen
    splash.finish(&w);

    return a.exec();
}
