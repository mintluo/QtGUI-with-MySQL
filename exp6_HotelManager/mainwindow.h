#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQuery>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    //房间类型选择 pushbutton
    void on_rb1_clicked();

    void on_rb2_clicked();

    void on_rb3_clicked();

    void on_rb4_clicked();

    void on_rb5_clicked();

    void on_rb6_clicked();

    void on_pbSubmit_clicked();

    void on_pbCancle_clicked();

    void on_pbOut_Search_clicked();

    void on_pbCheckout_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    QSqlTableModel *roominfo_model;     //获取roominfo
    QSqlTableModel *priceinfo_model;    //获取price_info
    QSqlTableModel *checkout_model;    //获取checkout
    QSqlTableModel *model_checkout;    //获取checkout
    QSqlTableModel *model_checkin;    //获取checkin
};

#endif // MAINWINDOW_H
