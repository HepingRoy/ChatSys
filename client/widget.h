#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include<QList>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private slots:  //建立槽函数
    void onReadyRead();//可读
    void onConnected();//连接
    void onDisconnected();//断开
    void  onError(QAbstractSocket::SocketError socketError);//错误



    void on_textSend_clicked();

    void on_image_clicked();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpSocket tcpSocket;
    int imageIndex;

    qint32 sizePackLast;

    QString Host="81.68.107.40";



};
#endif // WIDGET_H
