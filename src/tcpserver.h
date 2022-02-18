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

class TCPServer : public QTcpServer
{
    Q_OBJECT
public:
    TCPServer( QObject * parent = nullptr);
    ~TCPServer()= default;

    QTcpSocket * socket;

    void start(const QHostAddress & adress = QHostAddress::Any,quint16 port = 2022);

private:
    QVector<QTcpSocket*> Sockets;
    QByteArray m_Data;
    void SendToClient(QTcpSocket *socket, const QString & message);
    quint16 m_DataSize = 0;


public slots:
   virtual  void incomingConnection(qintptr socket_Descriptor);
    void slotReadyRead();

};

#endif // TCPSERVER_H
