#include "newteacher.h"
#include "ui_newteacher.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>

Newteacher::Newteacher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Newteacher)
{
    ui->setupUi(this);
}

Newteacher::~Newteacher()
{
    delete ui;
}

void Newteacher::on_pushButton_clicked()
{
    // Ensure the database connection is open
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Error: Database is not open.";
        return;
    }

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

    // Prepare the SQL query to insert data
    QSqlQuery query;
    query.prepare("INSERT INTO teachers (name, date_of_birth, age, cnic, gender, qualification, designation, department, date_of_joining, salary, phone_number, email, address) VALUES (:name, :date_of_birth, :age, :cnic, :gender, :qualification, :designation, :department, :joining_date, :salary, :phone, :email, :address)");

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

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Error: Could not insert data." << query.lastError();
    } else {
        QMessageBox::information(this, "Success", "New Teacher Added!");
        emit teacherAdded();
    }
}

