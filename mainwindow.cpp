#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "student.h"
#include "teacher.h"
#include "timetable.h"
#include "exam.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Apna School - Manage your school now!");
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

