#ifndef NEWTIMETABLE_H
#define NEWTIMETABLE_H

#include <QMainWindow>

namespace Ui {
class Newtimetable;
}

class Newtimetable : public QMainWindow
{
    Q_OBJECT

public:
    explicit Newtimetable(QWidget *parent = nullptr);
    ~Newtimetable();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Newtimetable *ui;
};

#endif // NEWTIMETABLE_H
