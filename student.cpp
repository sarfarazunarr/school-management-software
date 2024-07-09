#include "student.h"
#include "ui_student.h"
#include "newstudent.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>
#include "updatestudent.h"

Student::Student(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Student)
{
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS students (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, date_of_birth DATE NOT NULL, age INTEGER NOT NULL, cnic TEXT UNIQUE NOT NULL, gender TEXT NOT NULL CHECK (gender IN ('Male', 'Female', 'Other')), previous_class TEXT, new_class TEXT, guardian_name TEXT,  father_name TEXT, guardian_cnic TEXT, guardian_phone_number TEXT, guardian_address TEXT, date_of_registration DATE NOT NULL DEFAULT CURRENT_DATE)")) {
        qDebug() << "Error: Could not create table." << query.lastError();
    }
    ui->setupUi(this);
    QSqlTableModel *model = new QSqlTableModel();
    QTableView *view = new QTableView();
    model->setTable("students");
    model->select();
    ui->StudentDataTable->setModel(model);
    ui->StudentDataTable->resizeColumnsToContents();
    ui->StudentDataTable->show();

    // Connect the selectionChanged signal to a slot
    connect(ui->StudentDataTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            [this]() {
                QModelIndex selectedIndex = ui->StudentDataTable->selectionModel()->currentIndex();
                QString selectedText = selectedIndex.data(Qt::DisplayRole).toString();
                ui->idSearch->setText(selectedText);
            });
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


void Student::on_idsearchbtn_clicked()
{
    int id = ui->idSearch->text().toInt();

    QSqlQuery query;
    query.prepare("SELECT * FROM students WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.first()) {
            ui->std_name->setText(query.value("name").toString());
            ui->std_dob->setText(query.value("date_of_birth").toString());
            ui->std_age->setText(query.value("age").toString());
            ui->std_cnic->setText(query.value("cnic").toString());
            ui->std_gender->setText(query.value("gender").toString());
            ui->std_class->setText(query.value("new_class").toString());
            ui->std_gname->setText(query.value("guardian_name").toString());
            ui->std_fname->setText(query.value("father_name").toString());
            ui->std_gcnic->setText(query.value("guardian_cnic").toString());
            ui->std_gphone->setText(query.value("guardian_phone_number").toString());
            ui->std_address->setText(query.value("guardian_address").toString());
            ui->std_admdate->setText(query.value("date_of_registration").toString());
        } else {
            QMessageBox::information(this, "Error", "No student found with this ID");
        }
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}


void Student::on_actionUpdate_Student_triggered()
{
    UpdateStudent* update = new UpdateStudent(this);
    update->show();
}


void Student::on_actiondeleteStudent_triggered()
{
    UpdateStudent* update = new UpdateStudent(this);
    update->show();
}


void Student::on_studentSearchbtn_clicked()
{
    // QString searchQuery = ui->studentSearch->text();
    // QSqlQuery query;

    // // Prepare the search query
    // query.prepare("SELECT * FROM students WHERE id = :id OR name LIKE :searchQuery OR cnic LIKE :searchQuery");

    // // Bind the values
    // query.bindValue(":id", searchQuery.toInt());
    // query.bindValue(":searchQuery", "%" + searchQuery + "%");

    // // Execute the query
    // if (query.exec()) {
    //     // Update the studentModel
    //     studentModel->setQuery(query);
    //     ui->StudentDataTable->setModel(studentModel);
    //     ui->StudentDataTable->resizeColumnsToContents();
    // } else {
    //     QMessageBox::critical(this, "Error", query.lastError().text());
    // }
    QMessageBox::critical(this, "Unavailable", "Currently in development mode");
}

