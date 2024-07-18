#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "student.h"
#include "teacher.h"
#include "timetable.h"
#include "exam.h"
#include <QDate>
#include <QDesktopServices>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Apna School - Manage your school now!");
    QDate currentDate = QDate::currentDate();
    QString date = currentDate.toString();
    ui->today->setText(date);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    Student* st = new Student(this);
    st->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    Teacher* nt = new Teacher(this);
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

