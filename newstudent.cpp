#include "newstudent.h"
#include "ui_newstudent.h"

NewStudent::NewStudent(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NewStudent)
{
    ui->setupUi(this);
}

NewStudent::~NewStudent()
{
    delete ui;
}
