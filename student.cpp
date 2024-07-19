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
    ui->setupUi(this);

    // Setup the student data table
    updateStudentData();

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

    // Connect the signal from NewStudent to the slot in Student
    connect(ns, &NewStudent::studentAdded, this, &Student::updateStudentData);
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
    connect(update, &UpdateStudent::studentupdated, this, &Student::updateStudentData);
    update->show();
}

void Student::on_actiondeleteStudent_triggered()
{
    UpdateStudent* update = new UpdateStudent(this);
    connect(update, &UpdateStudent::studentupdated, this, &Student::updateStudentData);
    update->show();
}

void Student::on_studentSearchbtn_clicked()
{
    QString searchQuery = ui->studentSearch->text();
    QSqlQuery query;

    // Prepare the search query
    query.prepare("SELECT * FROM students WHERE id = :id OR name LIKE :searchQuery OR cnic LIKE :searchQuery");

    // Bind the values
    query.bindValue(":id", searchQuery.toInt());
    query.bindValue(":searchQuery", "%" + searchQuery + "%");

    // Execute the query
    if (query.exec()) {
        // Create a model and set it up with the query
        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(query);

        // Update the view with the new model
        ui->StudentDataTable->setModel(model);
        ui->StudentDataTable->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}

void Student::updateStudentData()
{
    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("students");
    model->select();
    ui->StudentDataTable->setModel(model);
    ui->StudentDataTable->resizeColumnsToContents();
    emit dataUpdated();
}

