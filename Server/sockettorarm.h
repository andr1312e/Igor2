#ifndef SERVER_INCOMINGDATAPARSER_H
#define SERVER_INCOMINGDATAPARSER_H

#include <QTcpSocket>
#include <QTimer>
#include <QDataStream>
#include <QDebug>

#include "Server/messagesrepository.h"

class SocketToRarm : public QObject
{
    Q_OBJECT

public:
    explicit SocketToRarm(const QString &rarmAdress, const quint16 rarmPost, QObject *parent);
    ~SocketToRarm();

private:
    void InitRepository();
    void InitAdditionalVariables();
    void ConnectObjects();
    void StartCheckConnectionTimer();
Q_SIGNALS:
    void ToSendWeather(const DevicesMeteoKitGetMessage &message);
public Q_SLOTS:
    void OnProgramFall();

private Q_SLOTS:
    void OnGetMessage();
    void OnConnectionError(QAbstractSocket::SocketError socketError);
    void OnDisconnected();
    void OnConnectToRarm();
    void OnCheckConnection();

private:
    void StopRarmConnect();
    void ReconnectToRarm();
    bool IsRarmConnected() const noexcept;
    void CheckRmoAndVoiStates(int systemIds);
    QByteArray CreateSyncMessage();
    void SendRarmSyncMessage();

private:
    const QString m_rarmAdress;
    const quint16 m_rarmPort;

    const std::array<quint8, 9> m_messagesWantedToGetFromRarm;
    const int VoiRmoWorksBitsMask = 0x06; //0000 0000 0000 0000 0000 0000 0000 0110 Little Endian

    MessagesRepository *m_repository;
    QTcpSocket *m_pTcpSocketToRarm;

    QByteArray m_gettingMessageArray;

    QTimer *m_timeToConnectRarm;

    bool m_needToSend;
    bool m_dataCollected;
};

#endif // SERVER_INCOMINGDATAPARSER_H
