#ifndef NEWSTUDENT_H
#define NEWSTUDENT_H

#include <QMainWindow>

namespace Ui {
class NewStudent;
}

class NewStudent : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewStudent(QWidget *parent = nullptr);
    ~NewStudent();

private slots:
    void on_pushButton_clicked();

private:
    Ui::NewStudent *ui;
};

#endif // NEWSTUDENT_H
