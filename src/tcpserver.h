#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVector>
#include <QDataStream>
#include "message.h"

class TCPServer : public QTcpServer
{
    Q_OBJECT
public:
    TCPServer( QObject * parent = nullptr);
    ~TCPServer()= default;

    QTcpSocket * socket;

    void start(const QHostAddress & adress = QHostAddress::Any,quint16 port = 2022);
    void stop();

private:
    QVector<QTcpSocket*> Sockets;
    QByteArray m_Data;
    void SendToClient(QTcpSocket *socket, const QString & message);
    quint16 m_DataSize = 0;

signals:
    void msg(const Message & msg);


public slots:
   virtual  void incomingConnection(qintptr socket_Descriptor);
    void slotReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState SocketState);

};

#endif // TCPSERVER_H
