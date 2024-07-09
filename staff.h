#ifndef STAFF_H
#define STAFF_H

#include <QMainWindow>

namespace Ui {
class Staff;
}

class Staff : public QMainWindow
{
    Q_OBJECT

public:
    explicit Staff(QWidget *parent = nullptr);
    ~Staff();

private:
    Ui::Staff *ui;
};

#endif // STAFF_H
