#include "student.h"
#include "ui_student.h"
#include "newstudent.h"
Student::Student(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Student)
{
    ui->setupUi(this);
}

Student::~Student()
{
    delete ui;
}

void Student::on_actionNew_Student_triggered()
{
    NewStudent* ns = new NewStudent(this);
    ns->show();
}

