#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "student.h"

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

