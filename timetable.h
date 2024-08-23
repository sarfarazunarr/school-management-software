#ifndef TIMETABLE_H
#define TIMETABLE_H

#include <QMainWindow>

namespace Ui {
class Timetable;
}

class Timetable : public QMainWindow
{
    Q_OBJECT

public slots:
    void getNewData();

public:
    explicit Timetable(QWidget *parent = nullptr);
    ~Timetable();

private slots:
    void on_actionaddData_triggered();

    void on_Daysname_currentTextChanged(const QString &arg1);

private:
    Ui::Timetable *ui;
};

#endif // TIMETABLE_H
