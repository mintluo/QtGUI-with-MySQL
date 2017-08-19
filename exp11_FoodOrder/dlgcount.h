#ifndef DLGCOUNT_H
#define DLGCOUNT_H

#include <QDialog>

namespace Ui {
class dlgcount;
}

class dlgcount : public QDialog
{
    Q_OBJECT

public:
    explicit dlgcount(QWidget *parent = 0);
    ~dlgcount();

private slots:
    void on_pbExit_clicked();

    void on_pbCount_clicked();

    void on_pbSearchInfo_clicked();

private:
    Ui::dlgcount *ui;
};

#endif // DLGCOUNT_H
