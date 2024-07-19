#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "student.h"
#include "teacher.h"
#include "timetable.h"
#include "exam.h"
#include <QDate>
#include <QDesktopServices>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Apna School - Manage your school now!");
    QDate currentDate = QDate::currentDate();
    QString date = currentDate.toString();
    ui->today->setText(date);

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return;
    } else {
       updateData();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateData()
{
    QSqlQuery query("SELECT COUNT(*) AS num_students FROM students");
    query.exec(); // Execute the query
    if (query.next()) {
        int numStudents = query.value("num_students").toInt();
        ui->numberofstudents->setText(QString::number(numStudents));
    }

    QSqlQuery query2("SELECT COUNT(*) AS num_teachers FROM teachers");
    query2.exec(); // Execute the second query
    if (query2.next()) {
        int numTeachers = query2.value("num_teachers").toInt();
        ui->numberofteachers->setText(QString::number(numTeachers));
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    Student* st = new Student(this);
    connect(st, &Student::dataUpdated, this, &MainWindow::updateData);
    st->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    Teacher* nt = new Teacher(this);
    connect(nt, &Teacher::dataUpdated, this, &MainWindow::updateData);
    nt->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    Timetable* tt = new Timetable(this);
    tt->show();
}


void MainWindow::on_pushButton_5_clicked()
{
    exam* ex = new exam(this);
    ex->show();
}


void MainWindow::on_pushButton_6_clicked()
{
    QDesktopServices::openUrl(QUrl("http://localhost:5173/"));
}


void MainWindow::on_pushButton_7_clicked()
{
    QDesktopServices::openUrl(QUrl("E://apnaschool//apnacopier.exe"));
}


void MainWindow::on_pushButton_9_clicked()
{
    QDesktopServices::openUrl(QUrl("https://usms.edu.pk"));
}



void MainWindow::on_pushButton_10_clicked()
{
    QDesktopServices::openUrl(QUrl("http://localhost:5173/"));
}

