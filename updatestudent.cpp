#include "updatestudent.h"
#include "ui_updatestudent.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>

UpdateStudent::UpdateStudent(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateStudent)
{
    ui->setupUi(this);
    setWindowTitle("Update Student - Apna School");
    setWindowIcon(QIcon(":/new/images/C:/Users/Administrator/Downloads/school.png"));
}

UpdateStudent::~UpdateStudent()
{
    delete ui;
}

void UpdateStudent::on_pushButton_3_clicked()
{
    int id = ui->idSearch->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT * FROM students WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.first()) {
            ui->studentName->setText(query.value("name").toString());
            ui->dobDate->setDate(QDate::fromString(query.value("date_of_birth").toString(), "yyyy-MM-dd"));
            ui->ageSpinBox->setValue(query.value("age").toInt());
            ui->cnicLineEdit->setText(query.value("cnic").toString());
            ui->gender->setCurrentText(query.value("gender").toString());
            ui->newClassLineEdit->setText(query.value("new_class").toString());
            ui->guardianNameLineEdit->setText(query.value("guardian_name").toString());
            ui->fatherNameLineEdit->setText(query.value("father_name").toString());
            ui->guardianCnicLineEdit->setText(query.value("guardian_cnic").toString());
            ui->guardianPhoneNumberLineEdit->setText(query.value("guardian_phone_number").toString());
            ui->guardianAddressLineEdit->setPlainText(query.value("guardian_address").toString());
        } else {
            QMessageBox::information(this, "Error", "No student found with this ID");
        }
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}


void UpdateStudent::on_deleteBtn_clicked()
{
    int id = ui->idSearch->text().toInt();
    QSqlQuery query;

    // Confirm deletion
    QMessageBox::StandardButton confirm = QMessageBox::question(this, "Confirm Deletion", "Are you sure you want to delete this student?", QMessageBox::Yes | QMessageBox::No);
    if (confirm == QMessageBox::Yes) {
        query.prepare("DELETE FROM students WHERE id = :id");
        query.bindValue(":id", id);

        if (query.exec()) {
            QMessageBox::information(this, "Success", "Student deleted successfully");
            // Clear fields
            ui->studentName->clear();
            ui->dobDate->setDate(QDate::currentDate());
            ui->ageSpinBox->setValue(0);
            ui->cnicLineEdit->clear();
            ui->gender->clear();
            ui->newClassLineEdit->clear();
            ui->guardianNameLineEdit->clear();
            ui->fatherNameLineEdit->clear();
            ui->guardianCnicLineEdit->clear();
            ui->guardianPhoneNumberLineEdit->clear();
            ui->guardianAddressLineEdit->clear();
            emit studentupdated();
        } else {
            QMessageBox::critical(this, "Error", query.lastError().text());
        }
    }
}




void UpdateStudent::on_pushButton_clicked()
{
    int id = ui->idSearch->text().toInt();
    QSqlQuery query;

    // Prepare the update query
    query.prepare("UPDATE students SET "
                  "name = :name, "
                  "date_of_birth = :date_of_birth, "
                  "age = :age, "
                  "cnic = :cnic, "
                  "gender = :gender, "
                  "new_class = :new_class, "
                  "guardian_name = :guardian_name, "
                  "father_name = :father_name, "
                  "guardian_cnic = :guardian_cnic, "
                  "guardian_phone_number = :guardian_phone_number, "
                  "guardian_address = :guardian_address "
                  "WHERE id = :id");

    // Bind the values
    query.bindValue(":id", id);
    query.bindValue(":name", ui->studentName->text());
    query.bindValue(":date_of_birth", ui->dobDate->date().toString("yyyy-MM-dd"));
    query.bindValue(":age", ui->ageSpinBox->value());
    query.bindValue(":cnic", ui->cnicLineEdit->text());
    query.bindValue(":gender", ui->gender->currentText());
    query.bindValue(":new_class", ui->newClassLineEdit->text());
    query.bindValue(":guardian_name", ui->guardianNameLineEdit->text());
    query.bindValue(":father_name", ui->fatherNameLineEdit->text());
    query.bindValue(":guardian_cnic", ui->guardianCnicLineEdit->text());
    query.bindValue(":guardian_phone_number", ui->guardianPhoneNumberLineEdit->text());
    query.bindValue(":guardian_address", ui->guardianAddressLineEdit->toPlainText());

    // Execute the query
    if (query.exec()) {
        emit studentupdated();
        QMessageBox::information(this, "Success", "Student updated successfully");
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}
