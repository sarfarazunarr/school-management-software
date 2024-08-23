#include "teacher.h"
#include "ui_teacher.h"
#include "newteacher.h"
#include "updateteacher.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>

Teacher::Teacher(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Teacher)
{
    ui->setupUi(this);
    setWindowTitle("Teachers - Apna School");
    setWindowIcon(QIcon(":/new/images/C:/Users/Administrator/Downloads/school.png"));
    // Setup the teacher data table
    updateTeacher();

    // Connect the selectionChanged signal to a slot
    connect(ui->TeacherDataTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            [this]() {
                QModelIndex selectedIndex = ui->TeacherDataTable->selectionModel()->currentIndex();
                QString selectedText = selectedIndex.data(Qt::DisplayRole).toString();
                ui->teacherId->setText(selectedText);
            });
}

Teacher::~Teacher()
{
    delete ui;
}

void Teacher::updateTeacher()
{
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS teachers (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, date_of_birth DATE NOT NULL, age INTEGER NOT NULL, cnic TEXT UNIQUE NOT NULL, gender TEXT NOT NULL CHECK (gender IN ('Male', 'Female', 'Other')), qualification TEXT, designation TEXT, department TEXT, date_of_joining DATE NOT NULL DEFAULT CURRENT_DATE, salary INTEGER, phone_number TEXT, email TEXT, address TEXT)")) {
        qDebug() << "Error: Could not create table." << query.lastError();
    } else {
        qDebug() << "Teacher Database is ready!";
    }

    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("teachers");
    model->select();
    ui->TeacherDataTable->setModel(model);
    ui->TeacherDataTable->resizeColumnsToContents();
    ui->TeacherDataTable->show();
    emit dataUpdated();
}

void Teacher::on_idsearchbtn_clicked()
{
    int id = ui->teacherId->text().toInt();

    QSqlQuery query;
    query.prepare("SELECT * FROM teachers WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.first()) {
            ui->pt_name->setText(query.value("name").toString());
            ui->pt_dob->setText(query.value("date_of_birth").toString());
            ui->pt_age->setText(query.value("age").toString());
            ui->pt_cnic->setText(query.value("cnic").toString());
            ui->pt_gender->setText(query.value("gender").toString());
            ui->pt_qualification->setText(query.value("qualification").toString());
            ui->pt_department->setText(query.value("department").toString());
            ui->pt_designation->setText(query.value("designation").toString());
            ui->pt_joinindate->setText(query.value("date_of_joining").toString());
            ui->pt_salary->setText(query.value("salary").toString());
            ui->pt_phone->setText(query.value("phone_number").toString());
            ui->pt_email->setText(query.value("email").toString());
            ui->pt_address->setText(query.value("address").toString());
        } else {
            QMessageBox::information(this, "Error", "No teacher found with this ID");
        }
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}

void Teacher::on_actionnewteacher_triggered()
{
    Newteacher* newteacher = new Newteacher(this);

    // Connect the signal from NewTeacher to the slot in Teacher
    connect(newteacher, &Newteacher::teacherAdded, this, &Teacher::updateTeacher);

    newteacher->show();
}

void Teacher::on_actionupdateteacher_triggered()
{
    Updateteacher* up = new Updateteacher(this);
    connect(up, &Updateteacher::updateData, this, &Teacher::updateTeacher);
    up->show();
}
