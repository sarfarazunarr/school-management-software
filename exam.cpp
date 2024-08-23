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
#include <QStandardPaths>


exam::exam(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::exam)
{
    ui->setupUi(this);

    setWindowTitle("Exam Management - Apna School");
    setWindowIcon(QIcon(":/new/images/C:/Users/Administrator/Downloads/school.png"));

    QSqlTableModel *model = new QSqlTableModel();
    ui->Teachers->setModel(model);
    QTableView *view = new QTableView();
    model->setTable("teachersBlock");
    model->select();
    ui->Teachers->setModel(model);
    ui->Teachers->resizeColumnsToContents();
    ui->Teachers->show();
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

    // Create a new table for blocks
    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS blocks (blockName TEXT, numofstudents INTEGER, studentIDs TEXT)")) {
        qDebug() << "Failed to create table:" << query.lastError();
        return;
    }

    query.prepare("SELECT COUNT(*) FROM blocks WHERE blockName = :blockName");
    query.bindValue(":blockName", blockName);
    if (!query.exec()) {
        qDebug() << "Failed to check block existence:" << query.lastError();
        return;
    }
    if (query.next() && query.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Block Exists", "Block " + blockName + " already exists.");
        return;
    }

    // Find all students in the specified class
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

    // Remove students already assigned to other blocks
    query.prepare("SELECT studentIDs FROM blocks");
    if (query.exec()) {
        while (query.next()) {
            QStringList assignedStudents = query.value("studentIDs").toString().split(",");
            for (const QString &assignedID : assignedStudents) {
                studentIDs.removeAll(assignedID.toInt());
            }
        }
    }

    // Check if there are enough students
    if (studentIDs.size() < numofstudents) {
        QMessageBox::warning(this, "Not Enough Students", "There are not enough students available to create this block.");
        return;
    }

    // Randomize or serialize the student IDs based on the assignment type
    if (assignType == "Random") {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(studentIDs.begin(), studentIDs.end(), g);
    }

    // Select the top N student IDs
    studentIDs = studentIDs.mid(0, numofstudents);

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

    // Determine the user's Documents directory and create Blocks folder if it doesn't exist
    QString documentsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString blocksDir = documentsDir + "/Blocks";

    QDir dir;
    if (!dir.exists(blocksDir)) {
        dir.mkpath(blocksDir);  // Create Blocks directory
    }

    // Save the PDF in the Blocks folder
    QString pdfFilePath = blocksDir + "/block" + blockName + ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(pdfFilePath);

    QPainter painter(&printer);
    painter.setFont(QFont("Helvetica", 12));
    painter.drawText(300, 90, "Block" + blockName);

    int y = 500;
    foreach (int studentID, studentIDs) {
        QString studentName = getStudentName(studentID);
        painter.drawText(10, y, QString::number(studentID));
        painter.drawText(200, y, studentName);
        y += 200;
    }

    painter.end();
    ui->generateList->setText("Generated");

    // Open the PDF file
    QString mainpath = "file:///" + pdfFilePath;
    mainpath.replace(" ", "%20");
    QDesktopServices::openUrl(QUrl(mainpath));
}

void exam::on_check_clicked()
{
    QString blockName = ui->blockName2->text();
    QString id = ui->stdId_3->text();

    if (blockName.isEmpty() || id.isEmpty()) {
        ui->checkStatus->setText("Block name or Student ID cannot be empty.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT studentIDs FROM blocks WHERE blockName = :blockName");
    query.bindValue(":blockName", blockName);

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return;
    }

    QString studentIDsString;
    if (query.next()) {
        studentIDsString = query.value("studentIDs").toString();
    }

    QStringList studentIDsList = studentIDsString.split(",");
    if (studentIDsList.contains(id)) {
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

    QString blockName = "Unknown";
    if (query.first()) {
        // Query to find the block where the student belongs
        QSqlQuery blockQuery;
        blockQuery.prepare("SELECT blockName FROM blocks WHERE studentIDs LIKE :studentId");
        blockQuery.bindValue(":studentId", "%" + QString::number(studentId) + "%");

        if (!blockQuery.exec()) {
            qDebug() << "Failed to execute query:" << blockQuery.lastError();
            return;
        }

        if (blockQuery.next()) {
            blockName = blockQuery.value("blockName").toString();
        }

        // Setup the printer for PDF generation
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);

        // Set the PDF output path to the user's Documents/Slips folder
        QString documentsDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QString slipsDir = documentsDir + "/Slips";

        QDir dir;
        if (!dir.exists(slipsDir)) {
            dir.mkpath(slipsDir);  // Create Slips directory if it doesn't exist
        }

        QString pdfFilePath = slipsDir + "/student_slip.pdf";
        printer.setOutputFileName(pdfFilePath);

        QPainter painter(&printer);
        painter.setFont(QFont("Helvetica", 18, 2));

        // Draw the school name as the heading
        painter.drawText(2500, 100, "University of Sufism & Modern Sciences");

        // Draw a line below the heading
        painter.drawLine(0, 500, 800, 70); // Adjust the coordinates to fit your needs
        painter.setFont(QFont("Helvetica", 14));

        painter.setFont(QFont("Helvetica", 12));
        painter.drawText(100, 700, "Student ID: " + query.value("id").toString());
        painter.drawText(100, 1000, "Student Name: " + query.value("name").toString());
        painter.drawText(100, 1500, "Student CNIC: " + query.value("cnic").toString());
        painter.drawText(100, 2000, "Student Class: " + query.value("new_class").toString());
        painter.drawText(100, 2500, "Father Name: " + query.value("father_name").toString());
        painter.drawText(100, 3000, "Block: " + blockName);

        painter.end();
        ui->pushButton_2->setText("Generated");

        QString mainpath = "file:///" + pdfFilePath;
        mainpath.replace(" ", "%20");

        qDebug() << "Formatted path: " << mainpath;

        QDesktopServices::openUrl(QUrl(mainpath));
    } else {
        QMessageBox::information(this, "Error", "No student found with this ID");
    }
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


void exam::on_deleteallblocks_clicked()
{
    QSqlQuery query;

    // SQL query to delete the entire blocks table
    if (!query.exec("DROP TABLE IF EXISTS blocks")) {
        // If the query fails, show an error message
        qDebug() << "Failed to delete blocks table:" << query.lastError();
        QMessageBox::warning(this, "Error", "Failed to delete blocks table: " + query.lastError().text());
        return;
    }

    // SQL query to delete the entire blocks table
    if (!query.exec("DROP TABLE IF EXISTS teachersBlock")) {
        // If the query fails, show an error message
        qDebug() << "Failed to delete teachers blocks table:" << query.lastError();
        QMessageBox::warning(this, "Error", "Failed to delete blocks table: " + query.lastError().text());
        return;
    }

    // Notify the user that the table was successfully deleted
    QMessageBox::information(this, "Success", "All blocks have been successfully deleted.");
}


