#ifndef TEACHER_H
#define TEACHER_H

#include <QMainWindow>

namespace Ui {
class Teacher;
}

class Teacher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Teacher(QWidget *parent = nullptr);
    ~Teacher();
signals:
    void dataUpdated();
public slots:
    void updateTeacher();

private slots:
    void on_idsearchbtn_clicked();

    void on_actionnewteacher_triggered();

    void on_actionupdateteacher_triggered();

private:
    Ui::Teacher *ui;
};

#endif // TEACHER_H
