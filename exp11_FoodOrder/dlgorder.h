#ifndef DLGORDER_H
#define DLGORDER_H

#include <QDialog>
#include <QSqlTableModel>
#include <QVector>

namespace Ui {
class dlgorder;
}

class dlgorder : public QDialog
{
    Q_OBJECT

public:
    explicit dlgorder(QWidget *parent = 0);
    ~dlgorder();

private slots:
    void on_pbFresh_clicked();

    void on_pbAdd_clicked();

    void on_pbSave_clicked();

    void on_pbCancel_Order_clicked();

    //显示该类型的所有信息
    void type(int index);

private:
    Ui::dlgorder *ui;

    QSqlTableModel *model;

    //定义容器
    QVector<int> order;
};

#endif // DLGORDER_H
