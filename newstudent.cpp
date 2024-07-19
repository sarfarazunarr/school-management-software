#include "newstudent.h"
#include "ui_newstudent.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

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

void NewStudent::on_pushButton_clicked()
{
    // Ensure the database connection is open
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return;
    }

    // Capture form data
    QString name = ui->studentName->text();
    QDate dateOfBirth = ui->dobDate->date();
    int age = ui->ageSpinBox->value();
    QString cnic = ui->cnicLineEdit->text();
    QString gender = ui->gender->currentText();
    QString previousClass = ui->previousClassLineEdit->text();
    QString newClass = ui->newClassLineEdit->text();
    QString fatherName = ui->fatherNameLineEdit->text();
    QString guardianName = ui->guardianNameLineEdit->text();
    QString guardianCnic = ui->guardianCnicLineEdit->text();
    QString guardianPhoneNumber = ui->guardianPhoneNumberLineEdit->text();
    QString guardianAddress = ui->guardianAddressLineEdit->toPlainText();

    // Prepare the SQL query to insert data
    QSqlQuery query;
    query.prepare("INSERT INTO students (name, date_of_birth, age, cnic, gender, previous_class, new_class, father_name, guardian_name, guardian_cnic, guardian_phone_number, guardian_address) "
                  "VALUES (:name, :date_of_birth, :age, :cnic, :gender, :previous_class, :new_class, :father_name, :guardian_name, :guardian_cnic, :guardian_phone_number, :guardian_address)");
    query.bindValue(":name", name);
    query.bindValue(":date_of_birth", dateOfBirth);
    query.bindValue(":age", age);
    query.bindValue(":cnic", cnic);
    query.bindValue(":gender", gender);
    query.bindValue(":previous_class", previousClass);
    query.bindValue(":new_class", newClass);
    query.bindValue(":father_name", fatherName);
    query.bindValue(":guardian_name", guardianName);
    query.bindValue(":guardian_cnic", guardianCnic);
    query.bindValue(":guardian_phone_number", guardianPhoneNumber);
    query.bindValue(":guardian_address", guardianAddress);

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Error: Could not insert data." << query.lastError();
    } else {
        qDebug() << "Data inserted successfully!";
        QMessageBox::information(this, "Success","Students Successfully enrolled!");
        emit studentAdded();  // Emit the signal to notify other windows
    }
}
