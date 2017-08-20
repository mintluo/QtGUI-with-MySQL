#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <QMessageBox>
#include <QTextCharFormat>
#include "filesender.h"
#include "filereceiver.h"

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = 0);
    ~ChatWidget();
    QString getSecAddr();
    void setSecAddr(QString,QString);

private:
    Ui::ChatWidget *ui;
    QFile saveFile;//聊天记录文件
    QString secretAddress;//聊天对象IP
    QString getIP();//获取IP
    void closeEvent(QCloseEvent *);
    QColor color;

    bool eventFilter(QObject *target, QEvent *event);//事件过滤器

signals:
    void sendMessagesFromChat(QString ,QString);
    void closeChat(QString);
    void sendFile(QString,QString);

private slots:
    void on_textHistory_clicked();//是否显示聊天记录
    void on_textunderline_clicked(bool checked);//设置聊天字体是否有下划线
    void on_textSave_clicked();//发送文件
    void on_textBin_clicked();//清空聊天消息框
    void on_btnClose_clicked();//关闭聊天窗口
    void on_btnSend_clicked();//发送消息
    void setMessage(QString address,QString first,QString second);//接收消息
    void on_textcolor_clicked();//设置聊天字体颜色
    void on_textitalic_clicked(bool checked);//设置聊天字体是否倾斜
    void on_textbold_clicked(bool checked);//设置聊天字体是否加粗
    void on_comboBox_currentIndexChanged(QString );//设置聊天字体大小
    void currentFormatChanged(const QTextCharFormat &format);
};

#endif // CHATWIDGET_H
