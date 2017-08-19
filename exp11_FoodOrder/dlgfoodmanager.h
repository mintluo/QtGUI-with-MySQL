#ifndef DLGFOODMANAGER_H
#define DLGFOODMANAGER_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class dlgfoodmanager;
}

class dlgfoodmanager : public QDialog
{
    Q_OBJECT

public:
    explicit dlgfoodmanager(QWidget *parent = 0);
    ~dlgfoodmanager();

private slots:
    void on_pbFresh_clicked();

    void on_pbAdd_clicked();

    void on_pbUpdate_Delete_clicked();

    void on_pbExit_clicked();

    //显示食物列表槽函数
    void type(int index);

private:
    Ui::dlgfoodmanager *ui;
    QSqlTableModel *model;
};

#endif // DLGFOODMANAGER_H
