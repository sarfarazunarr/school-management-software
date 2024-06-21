#include "student.h"
#include "ui_student.h"
#include "newstudent.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QTableView>

Student::Student(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Student)
{
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, date_of_birth DATE NOT NULL, age INTEGER NOT NULL, cnic TEXT UNIQUE NOT NULL, gender TEXT NOT NULL CHECK (gender IN ('Male', 'Female', 'Other')), previous_class TEXT, new_class TEXT, guardian_name TEXT,  father_name TEXT, guardian_cnic TEXT, guardian_phone_number TEXT, guardian_address TEXT, date_of_registration DATE NOT NULL DEFAULT CURRENT_DATE)")) {
        qDebug() << "Error: Could not create table." << query.lastError();
    } else {
        qDebug() << "Table created successfully!";
    }
    ui->setupUi(this);
    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("students");
    model->select();

    // Customize the headers
    model->setHeaderData(0, Qt::Horizontal, "Student ID");          // Assuming 'id' is the first column
    model->setHeaderData(1, Qt::Horizontal, "Name");                // Assuming 'name' is the second column
    model->setHeaderData(9, Qt::Horizontal, "Father Name");         // Assuming 'guardian_name' is the tenth column
    model->setHeaderData(3, Qt::Horizontal, "Age");                 // Assuming 'age' is the fourth column
    model->setHeaderData(7, Qt::Horizontal, "Class");               // Assuming 'new_class' is the eighth column
    model->setHeaderData(11, Qt::Horizontal, "Phone Number");       // Assuming 'guardian_phone_number' is the twelfth column
    model->setHeaderData(5, Qt::Horizontal, "Gender");              // Assuming 'gender' is the sixth column

    // Set up the view
    QTableView *view = new QTableView();
    ui->StudentDataTable->setModel(model);
    ui->StudentDataTable->resizeColumnsToContents();
    ui->StudentDataTable->show();
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

