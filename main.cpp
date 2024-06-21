#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Create a connection to the SQLite database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("school.db");

    if (!db.open()) {
        qDebug() << "Error: Could not open database." << db.lastError();
        return -1;
    } else {
        qDebug() << "Database opened successfully!";
    }


    MainWindow w;
    w.show();
    return a.exec();
}
