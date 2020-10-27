#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private slots:  //建立槽函数
      void onNewConnection();//新连接
      void onConnected();//连接
      void onDisconnected();//断开
      void  onError(QAbstractSocket::SocketError );//错误
      void onReadyRead();//可读




      void on_textSend_clicked();

      void on_Image_clicked();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpServer server;
    QList<QTcpSocket*> clients;//创建一个用于存储用户socket的容器
    int imageIndex;//图片号
    qint32 sizePackLast;//包大小
};
#endif // WIDGET_H

