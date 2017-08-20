#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_btnLogin_clicked();

    void on_btnRegister_clicked();

private:
    Ui::Login *ui;
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器
};

#endif // LOGIN_H
