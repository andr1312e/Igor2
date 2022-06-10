#include "Server/sockettorarm.h"
#include <iterator>

SocketToRarm::SocketToRarm(const QString &rarmAdress, const quint16 rarmPost, QObject *parent)
    : QObject(parent)
    , m_rarmAdress(rarmAdress)
    , m_rarmPort(rarmPost)
    , m_messagesWantedToGetFromRarm(
{
    RMO_VOI_TRACK_SETTING_MESSAGE,//1 Сообщение о проритетности трассы
    VOI_RMO_TRACK_DELETE_MESSAGE,//106 Сообщение об удалении трассы от вторички
    RMO_VOI_TARGET_POSITION_MESSAGE, //3 Сообщение с параметрами ЦУ
    RMO_VOI_TARGET_DELETE_MESSAGE, //4 Сообщение с удалением ЦУ
    RMO_VOI_DRIVE_TO_ROSITION_MESSAGE,//6 Сообщение с данными куда смотреть антенне
    RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE,//9 Сообщение с сектором биологической защиты
    RMO_OTHER_FORGET_ALL_DATA, //14 отчиска всех
    DEVICES_METEO_KIT_GET_MESSAGE,//162 Метео сообщение
    RARM_SYSTEM_CONTROL_MESSAGE,//253 Сообщение с состоянием подсистем
})
{
    InitRepository();
    InitAdditionalVariables();
    ConnectObjects();
    StartCheckConnectionTimer();
}

SocketToRarm::~SocketToRarm()
{
    delete m_repository;

    if (IsRarmConnected())
    {
        StopRarmConnect();
    }

    delete m_timeToConnectRarm;
    delete m_pTcpSocketToRarm;
}


void SocketToRarm::InitRepository()
{
    m_repository = new MessagesRepository();
}

void SocketToRarm::InitAdditionalVariables()
{
    m_pTcpSocketToRarm = new QTcpSocket(this);
    m_timeToConnectRarm = new QTimer(this);

    m_timeToConnectRarm->setTimerType(Qt::VeryCoarseTimer);
    m_needToSend = false;
    m_dataCollected = false;

}

void SocketToRarm::ConnectObjects()
{
    connect(m_pTcpSocketToRarm, &QTcpSocket::connected, this, &SocketToRarm::OnConnectToRarm);
    connect(m_pTcpSocketToRarm, &QTcpSocket::disconnected, this, &SocketToRarm::OnDisconnected);
    connect(m_timeToConnectRarm, &QTimer::timeout, this, &SocketToRarm::OnCheckConnection);
}

void SocketToRarm::StartCheckConnectionTimer()
{
    m_timeToConnectRarm->start(2000);
}

void SocketToRarm::OnProgramFall()
{
    m_needToSend = true;
}

void SocketToRarm::OnGetMessage()
{
    m_gettingMessageArray.append(m_pTcpSocketToRarm->readAll());

    QDataStream in(&m_gettingMessageArray, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_3);
    in.setByteOrder(QDataStream::LittleEndian);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);

    static quint8 lastID;
    static quint16 lastSize;
    static quint32 lastPos;
    quint8 messageId;
    quint16 messageSize;
    bool headerHandling = false;
    in >> messageSize;
    in >> messageId;

    while ((messageSize - 1) <= (m_gettingMessageArray.count() - in.device()->pos()))
    {
        headerHandling = true;
        lastPos = in.device()->pos();

        switch (messageId)
        {
        case RMO_VOI_TRACK_SETTING_MESSAGE:   //1 Сообщение о проритетности трассы
        {
            if (m_repository->m_trackSettingsMesageSize != (messageSize - 1))
            {
                qDebug() << "RMO_VOI_TRACK_SETTING_MESSAGE размеры не соотвествующие:получили " << messageSize << " и должно быть " <<  m_repository->m_trackSettingsMesageSize;
                in.skipRawData(messageSize - 1);
            }
            else
            {
                RMOTrackSetting settings;
                in.readRawData((char *) &settings, messageSize - 1);
                m_repository->AppendTrack(settings);
                qDebug() << "получили RMO_VOI_TRACK_SETTING_MESSAGE";
            }

            break;
        }

        case VOI_RMO_TRACK_DELETE_MESSAGE:   //106 Сообщение об удалении трассы от вторички
        {
            if (m_repository->m_trackSettingsDeleteSize != (messageSize - 1))
            {
                qDebug() << "VOI_RMO_TRACK_DELETE_MESSAGE размеры не соотвествующие:получили " << messageSize << " и должно быть " <<  m_repository->m_trackSettingsDeleteSize;
                in.skipRawData(messageSize - 1);
            }
            else
            {
                DeleteTrackMessage deleteTrack;
                in.readRawData((char *) &deleteTrack, messageSize - 1);
                m_repository->DeleteTrack(deleteTrack.aimID);
                qDebug() << "получили VOI_RMO_TRACK_DELETE_MESSAGE";
            }

            break;
        }

        case RMO_VOI_TARGET_POSITION_MESSAGE:   //3 Сообщение с параметрами ЦУ
        {
            if (m_repository->m_targetPositionMessageSize != (messageSize - 1))
            {
                qDebug() << "RMO_VOI_TARGET_POSITION_MESSAGE размеры не соотвествующие:получили " << messageSize << " и должно быть " <<  m_repository->m_targetPositionMessageSize;
                in.skipRawData(messageSize - 1);
            }
            else
            {
                RMOTargetPositionMessage targetPosition;
                in.readRawData((char *) &targetPosition, messageSize - 1);
                m_repository->AppendTargetPosition(targetPosition);
                qDebug() << "получили RMO_VOI_TARGET_POSITION_MESSAGE";
            }

            break;
        }

        case RMO_VOI_TARGET_DELETE_MESSAGE:   //4 Сообщение с удалением ЦУ
        {
            if (m_repository->m_targetPositionDeleteSize != (messageSize - 1))
            {
                qDebug() << "RMO_VOI_TARGET_DELETE_MESSAGE размеры не соотвествующие:получили " << messageSize << " и должно быть " <<  sizeof(RMO_VOI_TARGET_POSITION_MESSAGE);
                in.skipRawData(messageSize - 1);
            }
            else
            {
                RMOTargetDeleteMessage targetDelete;
                in.readRawData((char *) &targetDelete, messageSize - 1);
                m_repository->DeleteTargetPosition(targetDelete.id);
                qDebug() << "получили RMO_VOI_TARGET_DELETE_MESSAGE";
            }

            break;
        }

        case RMO_VOI_DRIVE_TO_ROSITION_MESSAGE:   //6 Cообщение с данными куда смотреть антенне
        {
            if (m_repository->m_driveToPositionMessageSize != (messageSize - 1))
            {
                qDebug() << "RMO_VOI_DRIVE_TO_ROSITION_MESSAGE размеры не соотвествующие:получили " << messageSize << " и должно быть " <<  m_repository->m_driveToPositionMessageSize;
                in.skipRawData(messageSize - 1);
            }
            else
            {
                m_dataCollected = true;
                RMODriveToPositionMessage message;
                in.readRawData((char *) &message, messageSize - 1);
                m_repository->SetDriveToPosition(message);
                qDebug() << "получили RMO_VOI_DRIVE_TO_ROSITION_MESSAGE";
            }

            break;
        }

        case RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE:   //9 Cообщение с сектором биологической защиты для ВОИ
        {
            if (m_repository->m_defenceSectorMessageSize != (messageSize - 1))
            {
                qDebug() << "RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE размеры не соотвествующие:получили " << messageSize << " и должно быть " <<  m_repository->m_defenceSectorMessageSize;
                in.skipRawData(messageSize - 1);
            }
            else
            {
                RMOBioDefenceSectorMessage message;
                in.readRawData((char *) &message, messageSize - 1);
                m_repository->EditBioDefenceSectorList(message);
                qDebug() << "получили RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE";
            }

            break;
        }

        case RMO_OTHER_FORGET_ALL_DATA:   //14 отчистка всех сообщений
        {
            if (m_repository->m_deleteMessageSize != (messageSize - 1))
            {
                qDebug() << "RMO_OTHER_FORGET_ALL_DATA размеры не соотвествующие:получили " << messageSize << " и должно быть " <<  m_repository->m_deleteMessageSize;
                in.skipRawData(messageSize - 1);
            }
            else
            {
                m_dataCollected = false;
                m_repository->ClearAllArrays();
                qDebug() << "получили RMO_OTHER_FORGET_ALL_DATA";
            }

            break;
        }

        case RARM_SYSTEM_CONTROL_MESSAGE:   // 255 Сообщение с состоянием подсистем
        {
            if (m_repository->m_rarmSystemControlSize != (messageSize - 1))
            {
                qDebug() << "RARM_SYSTEM_CONTROL_MESSAGE размеры не соотвествующие:получили " << messageSize << " и должно быть " <<  m_repository->m_rarmSystemControlSize;
                in.skipRawData(messageSize - 1);
            }
            else
            {
                RARMSysControlMessage message;
                in.readRawData((char *) &message, messageSize - 1);
                CheckRmoAndVoiStates(message.sysControl);
                qDebug() << "получили RARM_SYSTEM_CONTROL_MESSAGE";
            }

            break;
        }
        case DEVICES_METEO_KIT_GET_MESSAGE:
        {
            if (sizeof(DevicesMeteoKitGetMessage) == (messageSize - 1))
            {
                DevicesMeteoKitGetMessage message;
                in.readRawData((char *) &message, messageSize - 1);
                Q_EMIT ToSendWeather(message);
            }
            else
            {
                qDebug() << "DEVICES_METEO_KIT_GET_MESSAGE размеры не соотвествующие:получили " << messageSize << " и должно быть " <<  sizeof(DevicesMeteoKitGetMessage);
                in.skipRawData(messageSize - 1);
            }
        }
        default:
        {
            qDebug() << "Получено сообщение " << messageId << "  не обрабатываем! Убери из списка или напиши обработку";

            if (!IsRarmConnected())
            {
                qDebug() << "Рарм не подключен";
                m_gettingMessageArray.clear();
                return;
            }

            if ((messageSize - 1) > (m_gettingMessageArray.size() - in.device()->pos()))
            {
                m_gettingMessageArray.clear();
                qDebug() << "Отчищаем буфер сообщений";
                return;
            }
            else
            {
                in.skipRawData(messageSize - 1);
                qDebug() << "Пропустили " << messageSize << " байт";
            }

            break;
        }
        }

        if ((m_gettingMessageArray.size() - in.device()->pos()) > 3)
        {
            lastPos = in.device()->pos();
            lastID = messageId;
            lastSize = messageSize;
            in >> messageSize;
            in >> messageId;
            headerHandling = false;
        }
    }

    if (!headerHandling)
    {
        in.device()->seek(in.device()->pos() - 3);
    }

    m_gettingMessageArray.remove(0, in.device()->pos());
}

void SocketToRarm::OnConnectionError(QAbstractSocket::SocketError socketError)
{
    if (socketError ==  QAbstractSocket::ConnectionRefusedError ||
            socketError ==  QAbstractSocket::HostNotFoundError ||
            socketError ==  QAbstractSocket::SocketTimeoutError ||
            socketError ==  QAbstractSocket::NetworkError)
    {
        qDebug() << QStringLiteral("Пробуем получить соединение снова...") << socketError;
        ReconnectToRarm();
    }
    else
    {
        qDebug() << QStringLiteral("Ошибка: ") << socketError;
    }
}

void SocketToRarm::OnDisconnected()
{
    qDebug() << QStringLiteral("Отсоединились от РАРМ...");
    disconnect(m_pTcpSocketToRarm, &QTcpSocket::readyRead, this, &SocketToRarm::OnGetMessage);
}

void SocketToRarm::OnConnectToRarm()
{
    m_gettingMessageArray.clear();

    qDebug() << "IncomingDataHandler::slotConnected Подкючаемся к рарму";
    QByteArray message;

    QDataStream dataStream(&message, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_3);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    const qint16 sizeOfMessage = qint16((m_messagesWantedToGetFromRarm.size() + 2) * sizeof(quint8)); //messagesCount+MessageId+myId
    dataStream << sizeOfMessage;//размер
    dataStream << quint8(255);//ид сообщения
    dataStream << quint8(PROCESS_CONTROLLER);// мой ид

    for (const quint8 &messageIdToGet : m_messagesWantedToGetFromRarm)
    {
        dataStream << messageIdToGet;
    }

    m_pTcpSocketToRarm->write(message);
    m_pTcpSocketToRarm->flush();
    connect(m_pTcpSocketToRarm, &QTcpSocket::readyRead, this, &SocketToRarm::OnGetMessage);
}

void SocketToRarm::OnCheckConnection()
{
//    qDebug() << "Время проверить поключение";

//    if (!IsRarmConnected()) {
//        ReconnectToRarm();
//        qDebug() << "Переподключаемся";
//    }
}

void SocketToRarm::StopRarmConnect()
{
    m_pTcpSocketToRarm->abort();
    m_timeToConnectRarm->stop();
}

void SocketToRarm::ReconnectToRarm()
{
    m_pTcpSocketToRarm->connectToHost(m_rarmAdress, m_rarmPort, QIODevice::ReadWrite);
}

bool SocketToRarm::IsRarmConnected() const noexcept
{
    return m_pTcpSocketToRarm->state() == QTcpSocket::ConnectedState;
}

void SocketToRarm::CheckRmoAndVoiStates(int systemIds)
{
    if (m_needToSend && m_dataCollected)
    {
        if (VoiRmoWorksBitsMask == (systemIds & VoiRmoWorksBitsMask))
        {
            m_needToSend = false;
            SendRarmSyncMessage();
        }
    }
}

QByteArray SocketToRarm::CreateSyncMessage()
{
    QByteArray syncMessage;
    QDataStream out(&syncMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(0);
    out << quint8(150);//id сообщения которое я отправляю

    const QLinkedList<RMOTrackSetting> trackSettingList = m_repository->GetTrackList(); //1

    for (const RMOTrackSetting &trackSetting : trackSettingList)
    {
        out << m_repository->m_trackSettingsMesageSize;
        out << m_repository->m_trackSettingMessageId;//ид подсообщениий
        out.writeRawData(reinterpret_cast<const char *>(&(trackSetting)), m_repository->m_trackSettingsMesageSize);
    }

    const QLinkedList<RMOTargetPositionMessage> targetPositionList = m_repository->GetAllTargetPositionsList();

    for (const RMOTargetPositionMessage &targetPosition : targetPositionList)
    {
        out << m_repository->m_targetPositionMessageSize;
        out << m_repository->m_targetPositionMessageId;
        out.writeRawData(reinterpret_cast<const char *>(&(targetPosition)), m_repository->m_targetPositionMessageSize);
    }

    out << m_repository->m_driveToPositionMessageSize;
    out << m_repository->m_driveToPositionMessageId;
    out.writeRawData(reinterpret_cast<const char *>(&m_repository->GetRMODriveToPositionMessage()), m_repository->m_driveToPositionMessageSize);

    const QLinkedList<RMOBioDefenceSectorMessage> rmoBioDefenceSectorList = m_repository->GetRMOBioDefenceSectorList();

    for (const RMOBioDefenceSectorMessage &bioDefenceSector : rmoBioDefenceSectorList)
    {
        out << m_repository->m_defenceSectorMessageSize;
        out << m_repository->m_defenceSectorMessageId;
        out.writeRawData(reinterpret_cast<const char *>(&(bioDefenceSector)), m_repository->m_defenceSectorMessageSize);
    }

    out.device()->seek(0);//перемещаемся в ноль что бы записать размер на 0 позицию
    out << quint16(syncMessage.size() - sizeof(quint16));//пишем размер
    return syncMessage;
}

void SocketToRarm::SendRarmSyncMessage()
{
    m_pTcpSocketToRarm->write(CreateSyncMessage());
    m_pTcpSocketToRarm->flush();
}
