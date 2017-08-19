#ifndef DLGADDFOOD_H
#define DLGADDFOOD_H

#include <QDialog>

namespace Ui {
class dlgaddfood;
}

class dlgaddfood : public QDialog
{
    Q_OBJECT

public:
    explicit dlgaddfood(QWidget *parent = 0);
    ~dlgaddfood();

private slots:
    void on_pbAdd_clicked();

    void on_pbExit_clicked();

private:
    Ui::dlgaddfood *ui;

    void clear();
};

#endif // DLGADDFOOD_H
