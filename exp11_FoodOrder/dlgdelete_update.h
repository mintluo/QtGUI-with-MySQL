#ifndef DLGDELETE_UPDATE_H
#define DLGDELETE_UPDATE_H

#include <QDialog>
#include <QString>

namespace Ui {
class dlgdelete_update;
}

class dlgdelete_update : public QDialog
{
    Q_OBJECT

public:
    explicit dlgdelete_update(QWidget *parent = 0);
    ~dlgdelete_update();

private slots:
    void on_pbDelete_clicked();

    void on_pbUpdate_clicked();

    void on_pbExit_clicked();

private:
    Ui::dlgdelete_update *ui;

    void Datainit();
    void clear();
    QString fid;
};

#endif // DLGDELETE_UPDATE_H
