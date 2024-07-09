#ifndef UPDATETEACHER_H
#define UPDATETEACHER_H

#include <QMainWindow>

namespace Ui {
class Updateteacher;
}

class Updateteacher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Updateteacher(QWidget *parent = nullptr);
    ~Updateteacher();

private slots:
    void on_pushButton_3_clicked();

    void on_deleteBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::Updateteacher *ui;
};

#endif // UPDATETEACHER_H
