#include "updateteacher.h"
#include "ui_updateteacher.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>

Updateteacher::Updateteacher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Updateteacher)
{
    ui->setupUi(this);
}

Updateteacher::~Updateteacher()
{
    delete ui;
}

void Updateteacher::on_pushButton_3_clicked()
{
    int id = ui->idSearch->text().toInt();

    QSqlQuery query;
    query.prepare("SELECT * FROM teachers WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.first()) {
            ui->teacherName->setText(query.value("name").toString());
            ui->dobDate->setDate(QDate::fromString(query.value("date_of_birth").toString(), "yyyy-MM-dd"));
            ui->ageSpinBox->setValue(query.value("age").toInt());
            ui->cnicLineEdit->setText(query.value("cnic").toString());
            ui->gender->setCurrentText(query.value("gender").toString());
            ui->qualification->setText(query.value("qualification").toString());
            ui->department->setText(query.value("department").toString());
            ui->designation->setText(query.value("designation").toString());
            ui->joiningDate->setDate(QDate::fromString(query.value("date_of_joining").toString(), "yyyy-MM-dd"));
            ui->salary->setText(query.value("salary").toString());
            ui->phone_number->setText(query.value("phone_number").toString());
            ui->email->setText(query.value("email").toString());
            ui->address->setPlainText(query.value("address").toString());

        } else {
            QMessageBox::information(this, "Error", "No student found with this ID");
        }
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }

}


void Updateteacher::on_deleteBtn_clicked()
{
        int id = ui->idSearch->text().toInt();
        QSqlQuery query;

        // Confirm deletion
        QMessageBox::StandardButton confirm = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this teacher?", QMessageBox::Yes | QMessageBox::No);
        if (confirm == QMessageBox::Yes) {
            query.prepare("DELETE FROM teachers WHERE id = :id");
            query.bindValue(":id", id);

            if (query.exec()) {
                QMessageBox::information(this, "Success", "Teacher deleted successfully");
                // Clear fields
                ui->teacherName->clear();
                ui->dobDate->setDate(QDate::currentDate());
                ui->ageSpinBox->setValue(0);
                ui->cnicLineEdit->clear();
                ui->gender->clear();
                ui->qualification->clear();
                ui->department->clear();
                ui->designation->clear();
                ui->joiningDate->setDate(QDate::currentDate());
                ui->salary->clear();
                ui->phone_number->clear();
                ui->email->clear();
                ui->address->clear();
            } else {
                QMessageBox::critical(this, "Error", query.lastError().text());
            }
        }
}


void Updateteacher::on_pushButton_clicked()
{
    int id = ui->idSearch->text().toInt();
        // Capture form data
        QString name = ui->teacherName->text();
        QDate dateOfBirth = ui->dobDate->date();
        int age = ui->ageSpinBox->value();
        QString cnic = ui->cnicLineEdit->text();
        QString gender = ui->gender->currentText();
        QString qualification = ui->qualification->text();
        QString designation = ui->designation->text();
        QString department = ui->department->text();
        QDate joiningDate = ui->joiningDate->date();
        QString salary = ui->salary->text();
        QString phone = ui->phone_number->text();
        QString email = ui->email->text();
        QString address = ui->address->toPlainText();

        // Prepare the SQL query to update data
        QSqlQuery query;
        query.prepare("UPDATE teachers SET name = :name, date_of_birth = :date_of_birth, age = :age, cnic = :cnic, gender = :gender, qualification = :qualification, designation = :designation, department = :department, date_of_joining = :joining_date, salary = :salary, phone_number = :phone, email = :email, address = :address WHERE id = :id");

        // Bind the values
        query.bindValue(":name", name);
        query.bindValue(":date_of_birth", dateOfBirth);
        query.bindValue(":age", age);
        query.bindValue(":cnic", cnic);
        query.bindValue(":gender", gender);
        query.bindValue(":qualification", qualification);
        query.bindValue(":designation", designation);
        query.bindValue(":department", department);
        query.bindValue(":joining_date", joiningDate);
        query.bindValue(":salary", salary);
        query.bindValue(":phone", phone);
        query.bindValue(":email", email);
        query.bindValue(":address", address);
        query.bindValue(":id", id);

        // Execute the query
        if (!query.exec()) {
            qDebug() << "Error: Could not update data." << query.lastError();
        } else {
            QMessageBox::information(this, "Success", "Data Updated Successfully!");
        }
}

