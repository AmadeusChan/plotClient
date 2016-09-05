#ifndef MY_NETWORK_H
#define MY_NETWORK_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QList>
#include <QObject>
#include <QString>

class myNetwork:public QObject{

    Q_OBJECT

public:
    myNetwork(QObject *parent=0);
    ~myNetwork()=default;

private:
    QTcpServer *server;
    QTcpSocket *socket;
    bool connected_flag;
    QByteArray ba;
    QString strings;

    QHostAddress getIpAddress();

signals:
    void serverSetUp(QHostAddress,int);
    void clientSetUp(QHostAddress,int);
    void connected();
    void disconnected();
    void dataRead(QString);
    void dataWritten();

public slots:
    void setUpServer();
    void actionOfConnectedServer();
    void setUpClient(QHostAddress address_,int port_);
    void actionOfConnectedClient();
    void readData();
    void writeData(QString data_);
    void disconnectNetwork();

};


#endif // MYNETWORK_H
