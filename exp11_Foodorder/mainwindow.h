#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

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
    void on_pbMain_Manager_clicked();

    void on_pbMain_Order_clicked();

    void on_pbMain_Search_clicked();

    void on_pbMain_Count_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
};

#endif // MAINWINDOW_H
