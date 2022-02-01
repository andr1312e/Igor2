#include "Server/sockettorarm.h"
#include <iterator>

SocketToRarm::SocketToRarm(const QString &rarmAdress, const quint16 rarmPost, QObject *parent)
    : QObject(parent)
    , m_rarmAdress(rarmAdress)
    , m_rarmPort(rarmPost)
    , m_messagesWantedToGetFromRarm( {
                                     RMO_VOI_TRACK_SETTING_MESSAGE,//1 Сообщение о проритетности трассы
                                     VOI_RMO_TRACK_DELETE_MESSAGE,//106 Сообщение об удалении трассы от вторички
                                     RMO_VOI_TARGET_POSITION_MESSAGE, //3 Сообщение с параметрами ЦУ
                                     RMO_VOI_TARGET_DELETE_MESSAGE, //4 Сообщение с удалением ЦУ
                                     RMO_VOI_DRIVE_TO_ROSITION_MESSAGE,//6 Сообщение с данными куда смотреть антенне
                                     RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE,//9 Сообщение с сектором биологической защиты
                                     RMO_OTHER_FORGET_ALL_DATA, //14 отчиска всех
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

    if (IsRarmConnected()) {
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

    while ((messageSize - 1) <= (m_gettingMessageArray.count() - in.device()->pos())) {
        headerHandling = true;
        lastPos = in.device()->pos();

        switch (messageId) {
        case RMO_VOI_TRACK_SETTING_MESSAGE: { //1 Сообщение о проритетности трассы
            if (m_repository->m_trackSettingsMesageSize != (messageSize - 1)) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" RMO_VOI_TRACK_SETTING_MESSAGE Размеры не соотвествующие:получили ") + QString::number(messageSize) + QStringLiteral(" и должно быть ")+ QString::number(m_repository->m_trackSettingsMesageSize));
                in.skipRawData(messageSize - 1);
            } else  {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили RMO_VOI_TRACK_SETTING_MESSAGE"));
                RMOTrackSetting settings;
                in.readRawData((char *) &settings, messageSize - 1);
                m_repository->AppendTrack(settings);
            }

            break;
        }

        case VOI_RMO_TRACK_DELETE_MESSAGE: { //106 Сообщение об удалении трассы от вторички
            if (m_repository->m_trackSettingsDeleteSize != (messageSize - 1)) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" VOI_RMO_TRACK_DELETE_MESSAGE Размеры не соотвествующие:получили ") + QString::number(messageSize) + QStringLiteral(" и должно быть ")+ QString::number(m_repository->m_trackSettingsDeleteSize));
                in.skipRawData(messageSize - 1);
            } else  {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили VOI_RMO_TRACK_DELETE_MESSAGE"));
                DeleteTrackMessage deleteTrack;
                in.readRawData((char *) &deleteTrack, messageSize - 1);
                m_repository->DeleteTrack(deleteTrack.aimID);
            }

            break;
        }

        case RMO_VOI_TARGET_POSITION_MESSAGE: { //3 Сообщение с параметрами ЦУ
            if (m_repository->m_targetPositionMessageSize != (messageSize - 1)) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" RMO_VOI_TARGET_POSITION_MESSAGE Размеры не соотвествующие:получили ") + QString::number(messageSize) + QStringLiteral(" и должно быть ")+ QString::number(m_repository->m_targetPositionMessageSize));
                in.skipRawData(messageSize - 1);
            } else  {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили RMO_VOI_TARGET_POSITION_MESSAGE"));
                RMOTargetPositionMessage targetPosition;
                in.readRawData((char *) &targetPosition, messageSize - 1);
                m_repository->AppendTargetPosition(targetPosition);
            }

            break;
        }

        case RMO_VOI_TARGET_DELETE_MESSAGE: { //4 Сообщение с удалением ЦУ
            if (m_repository->m_targetPositionDeleteSize != (messageSize - 1)) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" RMO_VOI_TARGET_DELETE_MESSAGE Размеры не соотвествующие:получили ") + QString::number(messageSize) + QStringLiteral(" и должно быть ")+ QString::number(m_repository->m_targetPositionDeleteSize));
                in.skipRawData(messageSize - 1);
            } else  {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили RMO_VOI_TARGET_DELETE_MESSAGE"));
                RMOTargetDeleteMessage targetDelete;
                in.readRawData((char *) &targetDelete, messageSize - 1);
                m_repository->DeleteTargetPosition(targetDelete.id);
            }

            break;
        }

        case RMO_VOI_DRIVE_TO_ROSITION_MESSAGE: { //6 Cообщение с данными куда смотреть антенне
            if (m_repository->m_driveToPositionMessageSize != (messageSize - 1)) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" RMO_VOI_DRIVE_TO_ROSITION_MESSAGE Размеры не соотвествующие:получили ") + QString::number(messageSize) + QStringLiteral(" и должно быть ")+ QString::number(m_repository->m_driveToPositionMessageSize));
                in.skipRawData(messageSize - 1);
            } else  {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили RMO_VOI_DRIVE_TO_ROSITION_MESSAGE"));
                m_dataCollected = true;//Боченев: считать что если оно есть, то данные полные
                RMODriveToPositionMessage message;
                in.readRawData((char *) &message, messageSize - 1);
                m_repository->SetDriveToPosition(message);
            }

            break;
        }

        case RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE: { //9 Cообщение с сектором биологической защиты для ВОИ
            if (m_repository->m_defenceSectorMessageSize != (messageSize - 1)) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE Размеры не соотвествующие:получили ") + QString::number(messageSize) + QStringLiteral(" и должно быть ")+ QString::number(m_repository->m_defenceSectorMessageSize));
                in.skipRawData(messageSize - 1);
            } else  {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE"));
                RMOBioDefenceSectorMessage message;
                in.readRawData((char *) &message, messageSize - 1);
                m_repository->EditBioDefenceSectorList(message);
            }

            break;
        }

        case RMO_OTHER_FORGET_ALL_DATA: { //14 отчистка всех сообщений
            if (m_repository->m_deleteMessageSize != (messageSize - 1)) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" RMO_OTHER_FORGET_ALL_DATA Размеры не соотвествующие:получили ") + QString::number(messageSize) + QStringLiteral(" и должно быть ")+ QString::number(m_repository->m_deleteMessageSize));
                in.skipRawData(messageSize - 1);
            } else  {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили RMO_OTHER_FORGET_ALL_DATA"));
                m_dataCollected = false;
                m_repository->ClearAllArrays();
            }

            break;
        }

        case RARM_SYSTEM_CONTROL_MESSAGE: { // 255 Сообщение с состоянием подсистем
            if (m_repository->m_rarmSystemControlSize != (messageSize - 1)) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" RARM_SYSTEM_CONTROL_MESSAGE размеры не соотвествующие:получили ") + QString::number(messageSize) + QStringLiteral(" и должно быть ")+ QString::number(m_repository->m_rarmSystemControlSize));
                in.skipRawData(messageSize - 1);
            } else  {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получили RARM_SYSTEM_CONTROL_MESSAGE"));
                RARMSysControlMessage message;
                in.readRawData((char *) &message, messageSize - 1);
                CheckRmoAndVoiStates(message.sysControl);
            }
            break;
        }

        default: {
            Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получено сообщение ") + QString::number(messageId) + QStringLiteral("  не обрабатываем! Убери из списка или напиши обработку"));
            if (!IsRarmConnected()) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Рарм не подключен"));
                m_gettingMessageArray.clear();
                return;
            }

            if ((messageSize - 1) > (m_gettingMessageArray.size() - in.device()->pos())) {
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Отчищаем буфер сообщений"));
                m_gettingMessageArray.clear();
                return;
            } else {
                in.skipRawData(messageSize - 1);
                Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Пропустили") + QString::number(messageSize) + QStringLiteral(" байт"));
            }

            break;
        }
        }

        if ((m_gettingMessageArray.size() - in.device()->pos()) > 3) {
            lastPos = in.device()->pos();
            lastID = messageId;
            lastSize = messageSize;
            in >> messageSize;
            in >> messageId;
            headerHandling = false;
        }
    }

    if (!headerHandling) {
        in.device()->seek(in.device()->pos() - 3);
    }

    m_gettingMessageArray.remove(0, in.device()->pos());
}

void SocketToRarm::OnConnectionError(QAbstractSocket::SocketError socketError)
{
    if (socketError ==  QAbstractSocket::ConnectionRefusedError ||
            socketError ==  QAbstractSocket::HostNotFoundError ||
            socketError ==  QAbstractSocket::SocketTimeoutError ||
            socketError ==  QAbstractSocket::NetworkError) {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Пробуем получить соединение снова... ") + m_pTcpSocketToRarm->errorString());
        ReconnectToRarm();
    } else {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Ошибка: ") + m_pTcpSocketToRarm->errorString());
    }
}

void SocketToRarm::OnDisconnected()
{
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral("Отсоединились от РАРМ..."));
    disconnect(m_pTcpSocketToRarm, &QTcpSocket::readyRead, this, &SocketToRarm::OnGetMessage);
}

void SocketToRarm::OnConnectToRarm()
{
    m_gettingMessageArray.clear();
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral("Подкючаемся к рарму..."));
    QByteArray message;

    QDataStream dataStream(&message, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_3);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    qint16 sizeOfMessage = qint16((m_messagesWantedToGetFromRarm.size() + 2) * sizeof(quint8)); //messagesCount+MessageId+myId
    dataStream << sizeOfMessage;//размер
    dataStream << quint8(255);//ид сообщения
    dataStream << quint8(PROCESS_CONTROLLER);// мой ид

    for (const quint8 &messageIdToGet : m_messagesWantedToGetFromRarm) {
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

bool SocketToRarm::IsRarmConnected() const
{
    return (m_pTcpSocketToRarm->state() == QTcpSocket::ConnectedState);
}

void SocketToRarm::CheckRmoAndVoiStates(int systemIds)
{
    if (m_needToSend && m_dataCollected) {
        if (VoiRmoWorksBitsMask==(systemIds & VoiRmoWorksBitsMask)) {
            m_needToSend = false;
            SendRarmSyncMessage();
        }
    }
}

QByteArray SocketToRarm::CreateSyncMessage()
{
    QByteArray syncMessage;
    QDataStream out(&syncMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(0);
    out << quint8(150);//id сообщения которое я отправляю

    const QList<RMOTrackSetting> trackSettingList = m_repository->GetTrackList(); //1

    for (const RMOTrackSetting &trackSetting: trackSettingList) {
        out << (quint8)m_repository->m_trackSettingsMesageSize;
        out << (quint8)m_repository->m_trackSettingMessageId;//ид подсообщениий
        out.writeRawData(reinterpret_cast<const char *>(&(trackSetting)), m_repository->m_trackSettingsMesageSize);
    }

    const QList<RMOTargetPositionMessage> targetPositionList = m_repository->GetAllTargetPositionsList();

    for (const RMOTargetPositionMessage &targetPosition: targetPositionList) {
        out << (quint16)m_repository->m_targetPositionMessageSize;
        out << (quint8)m_repository->m_targetPositionMessageId;
        out.writeRawData(reinterpret_cast<const char *>(&(targetPosition)), m_repository->m_targetPositionMessageSize);
    }

    out << (quint16)m_repository->m_driveToPositionMessageSize;
    out << (quint8)m_repository->m_driveToPositionMessageId;
    out.writeRawData(reinterpret_cast<const char *>(&m_repository->GetRMODriveToPositionMessage()), m_repository->m_driveToPositionMessageSize);

    const QList<RMOBioDefenceSectorMessage> rmoBioDefenceSectorList = m_repository->GetRMOBioDefenceSectorList();

    for (const RMOBioDefenceSectorMessage &bioDefenceSector: rmoBioDefenceSectorList) {
        out << (quint16)m_repository->m_defenceSectorMessageSize;
        out << (quint8)m_repository->m_defenceSectorMessageId;
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
