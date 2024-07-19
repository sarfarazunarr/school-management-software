#ifndef STUDENT_H
#define STUDENT_H

#include <QMainWindow>

namespace Ui {
class Student;
}

class Student : public QMainWindow
{
    Q_OBJECT

public:
    explicit Student(QWidget *parent = nullptr);
    ~Student();
signals:
    void dataUpdated();
public slots:
    void updateStudentData();

private slots:
    void on_actionNew_Student_triggered();

    void on_idsearchbtn_clicked();

    void on_actionUpdate_Student_triggered();

    void on_actiondeleteStudent_triggered();

    void on_studentSearchbtn_clicked();

private:
    Ui::Student *ui;
};

#endif // STUDENT_H
