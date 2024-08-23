#ifndef EXAM_H
#define EXAM_H

#include <QMainWindow>

namespace Ui {
class exam;
}

class exam : public QMainWindow
{
    Q_OBJECT

public:
    explicit exam(QWidget *parent = nullptr);
    ~exam();

private slots:
    void on_generateList_clicked();
    QString getStudentName(int studentID);
    void on_check_clicked();

    void on_pushButton_2_clicked();

    void on_addTeacher_clicked();

    void on_deleteallblocks_clicked();

private:
    Ui::exam *ui;
};

#endif // EXAM_H
