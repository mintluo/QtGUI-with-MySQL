#ifndef FILESENDER_H
#define FILESENDER_H

#include <QDialog>

namespace Ui {
class FileSender;
}

class FileSender : public QDialog
{
    Q_OBJECT

public:
    explicit FileSender(QWidget *parent = 0);
    ~FileSender();

private slots:
    void on_btnQuit_clicked();

private:
    Ui::FileSender *ui;
};

#endif // FILESENDER_H
