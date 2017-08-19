#ifndef DLGSEARCH_H
#define DLGSEARCH_H

#include <QDialog>

namespace Ui {
class dlgsearch;
}

class dlgsearch : public QDialog
{
    Q_OBJECT

public:
    explicit dlgsearch(QWidget *parent = 0);
    ~dlgsearch();

private slots:
    void on_pbClientDetail_clicked();

    void on_pbCancel_clicked();

private:
    Ui::dlgsearch *ui;
    void init();
};

#endif // DLGSEARCH_H
