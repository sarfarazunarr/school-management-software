#ifndef UPDATESTUDENT_H
#define UPDATESTUDENT_H

#include <QDialog>

namespace Ui {
class UpdateStudent;
}

class UpdateStudent : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateStudent(QWidget *parent = nullptr);
    ~UpdateStudent();

private slots:
    void on_pushButton_3_clicked();

    void on_deleteBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::UpdateStudent *ui;
};

#endif // UPDATESTUDENT_H
