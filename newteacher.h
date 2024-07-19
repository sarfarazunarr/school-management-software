#ifndef NEWTEACHER_H
#define NEWTEACHER_H

#include <QMainWindow>

namespace Ui {
class Newteacher;
}

class Newteacher : public QMainWindow
{
    Q_OBJECT

public:
    explicit Newteacher(QWidget *parent = nullptr);
    ~Newteacher();

signals:
    void teacherAdded();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Newteacher *ui;
};

#endif // NEWTEACHER_H
