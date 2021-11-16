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

public Q_SLOTS:
   void OnProgramFall();

private Q_SLOTS:
   void OnGetMessage();
   void OnConnectionError(QAbstractSocket::SocketError socketError);
   void OnDisconnected();
   void OnConnectToRarm();
   void OnCheckConnection();

private:
   MessagesRepository *m_repository;
   QTcpSocket *m_pTcpSocketToRarm;

   QByteArray gettingMessageArray;
   const QUrl *m_rarmAdress;
   QTimer *m_timeToConnectRarm;

   bool m_needToSend;
   bool m_dataContains;

   const int VoiRmoWorksBitsMask = 0x06; //0000 0000 0000 0000 0000 0000 0000 0110 Little Endian

   std::array<quint8, 8> *m_messagesWantedToGetFromRarm;

private:

   void initRepository();

   void initAdditionalVariables();

   void connectObjects();

   void startTimer()
   {
      m_timeToConnectRarm->start(2000);
   }

private:

   void stopRarmConnect()
   {
      m_pTcpSocketToRarm->abort();
   }
   void reconnectToRarm()
   {
      m_pTcpSocketToRarm->connectToHost(m_rarmAdress->host(), m_rarmAdress->port(), QIODevice::ReadWrite);
   }
   bool isRarmConnected()
   {
      return (m_pTcpSocketToRarm->state() == QTcpSocket::ConnectedState);
   }
   void checkRmoAndVoiStates(int systemIds);
   QByteArray createSyncMessage();
   void sendRarmSyncMessage();
};

#endif // SERVER_INCOMINGDATAPARSER_H
