#include "timetable.h"
#include "ui_timetable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QStandardItemModel>
#include <QStandardItem>
#include "newtimetable.h"
#include <QMessageBox>

Timetable::Timetable(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Timetable)
{
    ui->setupUi(this);
    setWindowTitle("Timetable - Apna School");
    setWindowIcon(QIcon(":/new/images/C:/Users/Administrator/Downloads/school.png"));
    // Database connection and table creation
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS timetable (day TEXT NOT NULL, class TEXT NOT NULL, period INTEGER NOT NULL, subject TEXT NOT NULL, teacher TEXT NOT NULL, start_time TIME NOT NULL, end_time TIME NOT NULL, class_name TEXT NOT NULL)")) {
        qDebug() << "Error: Could not create table." << query.lastError();
    } else {
        qDebug() << "TimeTable database is ready!";
        getNewData();
    }

}

Timetable::~Timetable()
{
    delete ui;
}

void Timetable::on_actionaddData_triggered()
{
    Newtimetable* nt = new Newtimetable(this);
    connect(nt, &Newtimetable::updateTimeTable, this, &Timetable::getNewData);
    nt->show();
}

void Timetable::getNewData()
{
    // Get the current day
    QDate currentDate = QDate::currentDate();
    QString dayString = currentDate.toString("dddd");
    ui->dayname->setText(dayString);

    // Set up the model
    QStandardItemModel *model = new QStandardItemModel();

    QSqlQuery query;
    // Fetch data for the current day
    query.prepare("SELECT class, period, subject, teacher FROM timetable WHERE day = :day");
    query.bindValue(":day", dayString);
    if (!query.exec()) {
        qDebug() << "Error: Could not retrieve timetable data." << query.lastError();
    } else {
        // Populate the model with data
        while (query.next()) {
            QString className = query.value("class").toString();
            int period = query.value("period").toInt();
            QString subject = query.value("subject").toString();
            QString teacher = query.value("teacher").toString();

            // Find the column index for the class
            int columnIndex = -1;
            for (int col = 0; col < model->columnCount(); ++col) {
                if (model->headerData(col, Qt::Horizontal).toString() == className) {
                    columnIndex = col;
                    break;
                }
            }

            // If the class column doesn't exist, create it
            if (columnIndex == -1) {
                columnIndex = model->columnCount();
                model->setHorizontalHeaderItem(columnIndex, new QStandardItem(className));
            }

            // Set the subject and teacher in the appropriate cell
            QString cellText = subject + "\n" + teacher;
            model->setItem(period - 1, columnIndex, new QStandardItem(cellText));
        }
    }

    // Set the model on the table view
    ui->timetabledata->setModel(model);
    ui->timetabledata->resizeColumnsToContents();
}

void Timetable::on_Daysname_currentTextChanged(const QString &arg1)
{
    // Get the selected day
    QString Day = ui->Daysname->currentText();

    // Update the day name label
    ui->dayname->setText(Day);

    // Fetch data for the selected day
    QSqlQuery query;
    query.prepare("SELECT class, period, subject, teacher FROM timetable WHERE day = :day");
    query.bindValue(":day", Day);
    if (!query.exec()) {
        qDebug() << "Error: Could not retrieve timetable data." << query.lastError();
    } else {
        // Clear the previous model
        QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->timetabledata->model());
        if (model) {
            model->removeRows(0, model->rowCount());
        }

        // Populate the model with data for the selected day
        while (query.next()) {
            QString className = query.value("class").toString();
            int period = query.value("period").toInt();
            QString subject = query.value("subject").toString();
            QString teacher = query.value("teacher").toString();

            // Find the column index for the class
            int columnIndex = -1;
            for (int col = 0; col < model->columnCount(); ++col) {
                if (model->headerData(col, Qt::Horizontal).toString() == className) {
                    columnIndex = col;
                    break;
                }
            }

            // If the class column doesn't exist, create it
            if (columnIndex == -1) {
                columnIndex = model->columnCount();
                model->setHorizontalHeaderItem(columnIndex, new QStandardItem(className));
            }

            // Set the subject and teacher in the appropriate cell
            QString cellText = subject + "\n" + teacher;
            model->setItem(period - 1, columnIndex, new QStandardItem(cellText));
        }

        // Set the model on the table view
        ui->timetabledata->setModel(model);
        ui->timetabledata->resizeColumnsToContents();
    }
}

