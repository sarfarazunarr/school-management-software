#include "exam.h"
#include "ui_exam.h"
#include <QPrinter>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QString>
#include <random>
#include <algorithm>
#include <QSqlTableModel>
#include <QDesktopServices>

exam::exam(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::exam)
{
    ui->setupUi(this);
}

exam::~exam()
{
    delete ui;
}

QString exam::getStudentName(int studentID) {
    QSqlQuery query;
    query.prepare("SELECT name FROM students WHERE id = :id");
    query.bindValue(":id", studentID);
    query.exec();
    if (query.next()) {
        return query.value("name").toString();
    }
    return QString();
}

void exam::on_generateList_clicked()
{
    ui->generateList->setText("Generating...");
    QString blockName = ui->blockName->text();
    int numofstudents = ui->numberofstudents->text().toInt();
    QString studentClass = ui->stclass->text();
    QString assignType = ui->type->currentText();

    // Debug output
    qDebug() << "Block Name:" << blockName;
    qDebug() << "Number of Students:" << numofstudents;
    qDebug() << "Student Class:" << studentClass;
    qDebug() << "Assignment Type:" << assignType;

    // Create a new table for blocks
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS blocks (blockName TEXT, numofstudents INTEGER, studentIDs TEXT)")) {
        qDebug() << "Failed to create table:" << query.lastError();
        return;
    }

    // Find all students in the specified class (or all students if class is empty)
    query.prepare("SELECT id FROM students WHERE (:class IS NULL OR new_class = :class)");
    query.bindValue(":class", studentClass.isEmpty() ? QVariant(QVariant::String) : QVariant(studentClass));
    if (!query.exec()) {
        qDebug() << "Failed to fetch students:" << query.lastError();
        return;
    }

    // Get the list of student IDs
    QList<int> studentIDs;
    while (query.next()) {
        studentIDs.append(query.value("id").toInt());
    }

    // Debug output
    qDebug() << "Student IDs:" << studentIDs;

    // Randomize the student IDs based on the assignment type
    if (assignType == "Serialize") {
        // Serialize: list of students in series
        // (no need to shuffle the list)
    } else if (assignType == "Random") {
        // Random: randomize the list of student IDs
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(studentIDs.begin(), studentIDs.end(), g);
    }

    // Select the top N student IDs (where N is the number of students)
    studentIDs = studentIDs.mid(0, numofstudents);

    // Debug output
    qDebug() << "Selected Student IDs:" << studentIDs;

    // Convert QList<int> to a comma-separated QString
    QStringList studentIDStrings;
    for (int id : studentIDs) {
        studentIDStrings.append(QString::number(id));
    }
    QString studentIDsString = studentIDStrings.join(",");

    // Insert the block data into the blocks table
    query.prepare("INSERT INTO blocks (blockName, numofstudents, studentIDs) VALUES (:blockName, :numofstudents, :studentIDs)");
    query.bindValue(":blockName", blockName);
    query.bindValue(":numofstudents", numofstudents);
    query.bindValue(":studentIDs", studentIDsString);
    if (!query.exec()) {
        qDebug() << "Failed to insert block data:" << query.lastError();
        return;
    }

    // Setup the printer for PDF generation
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("block " + blockName + " .pdf");

    QPainter painter(&printer);
    painter.setFont(QFont("Helvetica", 12));

    // Draw the block name
    painter.drawText(300, 90, "Block" + blockName);

    // Draw the list of students
    int y = 500;
    foreach (int studentID, studentIDs) {
        QString studentName = getStudentName(studentID);
        // Debug output
        qDebug() << "Student ID:" << studentID << "Student Name:" << studentName;
        painter.drawText(10, y, studentName);
        y += 200;  // Increment y-coordinate for each new student name
    }

    painter.end();
    ui->generateList->setText("Generated");
    QString currentDir = QDir::currentPath();
    qDebug() << "Current directory: " << currentDir << "/block_list.pdf";

    // Ensure the path is correctly formatted as a file URL
    QString mainpath = "file:///" + currentDir + "/block_list.pdf";
    mainpath.replace(" ", "%20"); // Replace spaces with %20

    qDebug() << "Formatted path: " << mainpath;

    QDesktopServices::openUrl(QUrl(mainpath));

}

void exam::on_check_clicked()
{
    QString blockName = ui->blockName2->text();
    QString id = ui->stdId_3->text();

    // Prepare the query to check if the student ID is in the specified block
    QSqlQuery query;
    query.prepare("SELECT studentIDs FROM blocks WHERE blockName = :blockName");
    query.bindValue(":blockName", blockName);

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError();
        return;
    }

    bool studentFound = false;

    // Check if the student ID exists in the block
    if (query.next()) {
        QString studentIDs = query.value("studentIDs").toString();
        QStringList studentIDList = studentIDs.split(',');

        if (studentIDList.contains(id)) {
            studentFound = true;
        }
    }

    // Update the UI based on the result
    if (studentFound) {
        ui->checkStatus->setText("Student ID " + id + " is in block " + blockName);
    } else {
        ui->checkStatus->setText("Student ID " + id + " is NOT in block " + blockName);
    }
}



void exam::on_pushButton_2_clicked()
{
    int studentId = ui->stdId->text().toInt();
    ui->pushButton_2->setText("Generating...");
    QSqlQuery query;
    query.prepare("SELECT * FROM students WHERE id = :id");
    query.bindValue(":id", studentId);
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError();
        return;
    }

    QSqlQuery query2;
    query2.prepare("SELECT blockName FROM blocks WHERE FIND_IN_SET(:id, studentIds) > 0");
    query2.bindValue(":id", studentId);

    if (!query2.exec()) {
        qDebug() << "Failed to execute query:" << query2.lastError();
    }

    // Setup the printer for PDF generation
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("student_slip.pdf");

    QPainter painter(&printer);
    painter.setFont(QFont("Helvetica", 18, 2));

    // Draw the school name as the heading
    painter.drawText(2500, 100, "Univeristy of Sufism & Modern Sciences"); // Replace with your school name

    // Draw a line below the heading
    painter.drawLine(0, 500, 800, 70); // Adjust the coordinates to fit your needs
    painter.setFont(QFont("Helvetica", 14));
    if (query.exec()) {
        if (query.first()) {
            painter.setFont(QFont("Helvetica", 12));
            painter.drawText(100, 700, "Student ID: " + query.value("id").toString());
            painter.drawText(100, 1000, "Student Name: " + query.value("name").toString());
            painter.drawText(100, 1500, "Student CNIC: " + query.value("cnic").toString());
            painter.drawText(100, 2000, "Student Class: " + query.value("new_class").toString());
            painter.drawText(100, 2500, "Father Name: " + query.value("father_name").toString());
            painter.drawText(100, 3000, "Block: " + query2.value(0).toString());
        } else {
            QMessageBox::information(this, "Error", "No student found with this ID");
        }}

    ui->pushButton_2->setText("Generated");
    QString currentDir = QDir::currentPath();
    qDebug() << "Current directory: " << currentDir << "/student_slip.pdf";

    // Ensure the path is correctly formatted as a file URL
    QString mainpath = "file:///" + currentDir + "/student_slip.pdf";
    mainpath.replace(" ", "%20"); // Replace spaces with %20

    qDebug() << "Formatted path: " << mainpath;

    QDesktopServices::openUrl(QUrl(mainpath));

    painter.end();
}


void exam::on_addTeacher_clicked()
{
    int TeacherId = ui->TeacherId->text().toInt();
    QString BlockName = ui->blockName2_2->text();
    QDate date = ui->dateEdit->date();

    // Create table if it doesn't exist
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS teachersBlock (id INTEGER PRIMARY KEY, teacherId INTEGER, name TEXT, blockName TEXT, date DATE)");
    query.exec();

    // Fetch teacher name from teachers table
    QString teacherName;
    query.prepare("SELECT name FROM teachers WHERE id = :id");
    query.bindValue(":id", TeacherId);
    if (query.exec()) {
        if (query.next()) {
            teacherName = query.value(0).toString();
        }
    }

    // Insert data into teachers table
    query.prepare("INSERT INTO teachersBlock (teacherId, name, blockName, date) VALUES (:teacherId, :name, :blockName, :date)");
    query.bindValue(":teacherId", TeacherId);
    query.bindValue(":name", teacherName);
    query.bindValue(":blockName", BlockName);
    query.bindValue(":date", date);
    query.exec();

    // Fetch data from teachers table and show in TableView
    QSqlTableModel *model = new QSqlTableModel();
    ui->Teachers->setModel(model);
    QTableView *view = new QTableView();
    model->setTable("teachersBlock");
    model->select();
    ui->Teachers->setModel(model);
    ui->Teachers->resizeColumnsToContents();
    ui->Teachers->show();
}

