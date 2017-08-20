#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

private slots:
    void on_btnOk_clicked();

    void on_btnQuit_clicked();

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
