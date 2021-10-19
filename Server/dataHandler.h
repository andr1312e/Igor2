#ifndef SERVER_INCOMINGDATAPARSER_H
#define SERVER_INCOMINGDATAPARSER_H

#include <QTcpSocket>
#include <QUrl>
#include <QTimer>
#include <QDataStream>
#include <QDebug>

#include "Server/messagesrepository.h"

class SocketToRarm : public QObject
{
    Q_OBJECT

public:
    SocketToRarm(const QString rarmAdress, QObject *parent);
    ~SocketToRarm();

public slots:
    void programFall();

private slots:
    void getMessage();
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connectToRarm();

    void checkConnection();

private:
    MessagesRepository *m_repository;
    QTcpSocket *m_pTcpSocketToRarm;

    QByteArray gettingMessageArray;
    const QUrl m_rarmAdress;
    QTimer *m_timeToConnectRarm;

    bool m_needToSend;
    bool m_dataContains;

    const int VoiRmoWorksBitsMask=0x06;//0000 0000 0000 0000 0000 0000 0000 0110 Little Endian

    std::array<quint8, 8> *m_messagesWantedToGetFromRarm;

private:

    void initRepository();

    void initAdditionalVariables();

    void createConnections();

    void startTimer(){ m_timeToConnectRarm->start(2000);}

private:

    void stopRarmConnect() { m_pTcpSocketToRarm->abort(); }
    void reconnectToRarm(){m_pTcpSocketToRarm->connectToHost(m_rarmAdress.host(), m_rarmAdress.port(), QIODevice::ReadWrite);}
    bool isRarmConnected() { return (m_pTcpSocketToRarm->state() == QTcpSocket::ConnectedState); }
    void checkRmoAndVoiStates(int systemIds);
    QByteArray createSyncMessage();
    void sendRarmSyncMessage();
};

#endif // SERVER_INCOMINGDATAPARSER_H
