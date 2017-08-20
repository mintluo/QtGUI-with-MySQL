#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <qmap.h>
#include "chatwidget.h"
#include "filesender.h"

namespace Ui {
class Widget;
}

//枚举变量标志信息的类型，分别为消息，新用户加入，和用户退出
enum MessageType{Message,NewParticipant,ParticipantLeft,FileName,Refuse,AskStartChat,StartChat,RefuseChat};

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    //获取帐号
    QString getUserName();
    //给Name赋值
    void SetName(QString);

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *);
    void sendMessage(MessageType type,QString serverAddress="",QString message = "");//发送消息
    void newParticipant(QString localHostName,QString ipAddress);//新用户加入
    void participantLeft(QString localHostName);//用户离开
    void hasPendingFile(QString localhostname,QString serverAddress,
                        QString clientAddress,QString fileName);

signals:
    void sendMessagesZ(QString address,QString first,QString second);

private slots:
    void on_btnAll_clicked();//群聊

    void on_btnCancel_clicked();

    void on_tableWidget_doubleClicked(QModelIndex index);//请求聊天

    void closeFromChat(QString);//关闭聊天窗口
    void sendMessagesToMain(QString,QString);//处理聊天窗口发来的消息
    void processPendingDatagrams();//接收数据
    void sentFileName(QString,QString);//发送文件
    void hasFileToSend(QString,QString);//对方确认接收文件后，处理信号
private:
    Ui::Widget *ui;

    QString Name;//用户名
    QUdpSocket *udpSocket;
    QMap<QString,ChatWidget *> list;//聊天窗口列表
    qint32 port;//端口号
    FileSender * Sender;//发送文件窗口
    QString getIP();//获取本地IP

};

#endif // WIDGET_H
