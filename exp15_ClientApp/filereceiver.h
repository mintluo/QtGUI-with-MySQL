#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QDialog>

namespace Ui {
class FileReceiver;
}

class FileReceiver : public QDialog
{
    Q_OBJECT

public:
    explicit FileReceiver(QWidget *parent = 0);
    ~FileReceiver();

private slots:
    void on_btnCancel_clicked();

    void on_btnClose_clicked();

private:
    Ui::FileReceiver *ui;
};

#endif // FILERECEIVER_H
