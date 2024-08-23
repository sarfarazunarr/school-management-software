#include "newtimetable.h"
#include "ui_newtimetable.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

Newtimetable::Newtimetable(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Newtimetable)
{
    ui->setupUi(this);
    setWindowTitle("New Timetable - Apna School");
    setWindowIcon(QIcon(":/new/images/C:/Users/Administrator/Downloads/school.png"));
}

Newtimetable::~Newtimetable()
{
    delete ui;
}

void Newtimetable::on_pushButton_clicked()
{
    // Retrieve data from the form
    QString day = ui->day->currentText();
    QString className = ui->className->currentText();  // Assuming class and className are the same
    int period = ui->period->value();
    QString subject = ui->subject->text();
    QString teacher = ui->teacher->text();
    QString startTime = ui->startTime->time().toString("HH:mm:ss");  // Assuming startTime is a QTimeEdit
    QString endTime = ui->endTime->time().toString("HH:mm:ss");  // Assuming endTime is a QTimeEdit

    // Open database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        if (!db.open()) {
            qDebug() << "Error: Could not open database." << db.lastError();
            return;
        }
    }

    // Prepare and execute the SQL query
    QSqlQuery query;
    query.prepare("INSERT INTO timetable (day, class, period, subject, teacher, start_time, end_time, class_name) "
                  "VALUES (:day, :class, :period, :subject, :teacher, :start_time, :end_time, :class_name)");
    query.bindValue(":day", day);
    query.bindValue(":class", className);
    query.bindValue(":period", period);
    query.bindValue(":subject", subject);
    query.bindValue(":teacher", teacher);
    query.bindValue(":start_time", startTime);
    query.bindValue(":end_time", endTime);
    query.bindValue(":class_name", className);

    if (!query.exec()) {
        qDebug() << "Error: Could not insert data." << query.lastError();
        QMessageBox::information(this, "Error", "Error Occured");
    } else {
        qDebug() << "Data inserted successfully!";
        emit updateTimeTable();
        QMessageBox::information(this, "Success", "Data Inserted Successfully!");
        this->close();
    }
}


