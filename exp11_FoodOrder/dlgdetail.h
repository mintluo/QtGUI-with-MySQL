#ifndef DLGDETAIL_H
#define DLGDETAIL_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class dlgdetail;
}

class dlgdetail : public QDialog
{
    Q_OBJECT

public:
    explicit dlgdetail(QWidget *parent = 0);
    ~dlgdetail();

private slots:
    void on_pbOk_clicked();

private:
    Ui::dlgdetail *ui;

    int cid;
    QSqlTableModel *model;

    void datainit();
};

#endif // DLGDETAIL_H
