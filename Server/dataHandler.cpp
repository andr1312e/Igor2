#include "Server/dataHandler.h"
#include <iterator>

SocketToRarm::SocketToRarm(const QString rarmAdress, QObject *parent)
    : QObject(parent)
    , m_rarmAdress(rarmAdress)
{
    initRepository();
    initAdditionalVariables();
    createConnections();
    startTimer();
}

SocketToRarm::~SocketToRarm()
{
    delete m_repository;
    if(isRarmConnected())
    {
        stopRarmConnect();
    }
    delete m_timeToConnectRarm;
    delete m_pTcpSocketToRarm;
    delete m_messagesWantedToGetFromRarm;
}

void SocketToRarm::programFall()
{
    m_needToSend=true;
}

void SocketToRarm::initRepository()
{
    m_repository=new MessagesRepository();
}

void SocketToRarm::initAdditionalVariables()
{
    m_pTcpSocketToRarm = new QTcpSocket(this);
    m_timeToConnectRarm = new QTimer(this);
    m_timeToConnectRarm->start(true);

    m_timeToConnectRarm->setTimerType(Qt::VeryCoarseTimer);
    m_needToSend=false;
    m_dataContains=false;
    m_messagesWantedToGetFromRarm=new QVector<quint8>{
            RMO_VOI_TRACK_SETTING_MESSAGE,//1 Сообщение о проритетности трассы
            VOI_RMO_TRACK_DELETE_MESSAGE,//106 Сообщение об удалении трассы от вторички
            RMO_VOI_TARGET_POSITION_MESSAGE, //3 Сообщение с параметрами ЦУ
            RMO_VOI_TARGET_DELETE_MESSAGE, //4 Сообщение с удалением ЦУ
            RMO_VOI_DRIVE_TO_ROSITION_MESSAGE,//6 Сообщение с данными куда смотреть антенне
            RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE,//9 Сообщение с сектором биологической защиты
            RMO_OTHER_FORGET_ALL_DATA, //14 отчиска всех
            RARM_SYSTEM_CONTROL_MESSAGE,//253 Сообщение с состоянием подсистем
    };
}

void SocketToRarm::createConnections()
{
    connect(m_pTcpSocketToRarm, &QTcpSocket::connected, this, &SocketToRarm::connectToRarm);
    connect(m_pTcpSocketToRarm, &QTcpSocket::disconnected, this, &SocketToRarm::disconnected);
    connect(m_timeToConnectRarm, &QTimer::timeout, this, &SocketToRarm::checkConnection);
}

void SocketToRarm::checkRmoAndVoiStates(int systemIds)
{
    if (m_needToSend && m_dataContains)
    {
        if ((systemIds & VoiRmoWorksBitsMask)==VoiRmoWorksBitsMask)
        {
            m_needToSend=false;
            sendRarmSyncMessage();
        }
    }
}

QByteArray SocketToRarm::createSyncMessage()
{
    QByteArray syncMessage;
    QDataStream out(&syncMessage, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out.setByteOrder(QDataStream::LittleEndian);
    out.setFloatingPointPrecision(QDataStream::SinglePrecision);

    out << qint16(0);
    out << quint8(150);//id сообщения которое я отправляю

    QList<RMOTrackSetting> *trackSettings=m_repository->m_trackSettingList;//1
    for (QList<RMOTrackSetting>::iterator it=trackSettings->begin(); it!=trackSettings->end(); ++it)
    {
        out << m_repository->m_trackSettingsMesageSize;
        out << m_repository->m_trackSettingMessageId;//ид подсообщениий
        out.writeRawData(reinterpret_cast<const char *>(&(*it)), m_repository->m_trackSettingsMesageSize);
    }

    QList<RMOTargetPositionMessage> *targetPositions=m_repository->m_targetPositionsList;
    for (QList<RMOTargetPositionMessage>::iterator it=targetPositions->begin(); it!=targetPositions->end(); ++it)
    {
        out << m_repository->m_targetPositionMessageSize;
        out << m_repository->m_targetPositionMessageId;
        out.writeRawData(reinterpret_cast<const char *>(&(*it)), m_repository->m_targetPositionMessageSize);
    }

    out << m_repository->m_driveToPositionMessageSize;
    out << m_repository->m_driveToPositionMessageId;
    out.writeRawData(reinterpret_cast<const char *>(&m_repository->m_driveToPositionMessage), m_repository->m_driveToPositionMessageSize);

    QList<RMOBioDefenceSectorMessage> *defends=m_repository->m_defenceSectorMessageList;
    for (QList<RMOBioDefenceSectorMessage>::iterator it=defends->begin(); it!=defends->end(); ++it)
    {
        out << m_repository->m_defenceSectorMessageSize;
        out << m_repository->m_defenceSectorMessageId;
        out.writeRawData(reinterpret_cast<const char *>(&(*it)), m_repository->m_defenceSectorMessageSize);
    }
    out.device()->seek(0);//перемещаемся в ноль что бы записать размер на 0 позицию
    out << quint16(syncMessage.size() - sizeof(quint16));//пишем размер
    return syncMessage;
}

void SocketToRarm::sendRarmSyncMessage()
{
    m_pTcpSocketToRarm->write(createSyncMessage());
    m_pTcpSocketToRarm->flush();
}

void SocketToRarm::connectToRarm()
{
    gettingMessageArray.clear();

    qDebug() << "IncomingDataHandler::slotConnected Подкючаемся к рарму";
    QByteArray message;

    QDataStream dataStream(&message, QIODevice::WriteOnly);
    dataStream.setVersion(QDataStream::Qt_5_3);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    qint16 sizeOfMessage=qint16((m_messagesWantedToGetFromRarm->count()+2)*sizeof(quint8));//messagesCount+MessageId+myId
    dataStream << sizeOfMessage;//размер
    dataStream << quint8(255);//ид сообщения
    dataStream << quint8(PROCESS_CONTROLLER);// мой ид
    for (QVector<quint8>::ConstIterator it=m_messagesWantedToGetFromRarm->cbegin(); it!=m_messagesWantedToGetFromRarm->cend(); ++it)
    {
        dataStream << *it;
    }
    m_pTcpSocketToRarm->write(message);
    m_pTcpSocketToRarm->flush();
    connect(m_pTcpSocketToRarm, &QTcpSocket::readyRead, this, &SocketToRarm::getMessage);
}

void SocketToRarm::checkConnection()
{
//    if (m_pTcpSocketToRarm->state()!=QAbstractSocket::ConnectedState)
//    {
//        m_pTcpSocketToRarm->close();
//    }
    qDebug() << "Время проверить поключение";
    if (!isRarmConnected()) {
        reconnectToRarm();
        qDebug() << "Переподключаемся";
    }
}

void SocketToRarm::getMessage()
{
    gettingMessageArray.append(m_pTcpSocketToRarm->readAll());

    QDataStream in(&gettingMessageArray, QIODevice::ReadOnly);
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
    while ((messageSize - 1) <= (gettingMessageArray.size() - in.device()->pos())) {
        headerHandling = true;
        lastPos = in.device()->pos();
        switch(messageId) {
        case RMO_VOI_TRACK_SETTING_MESSAGE: //1 Сообщение о проритетности трассы
        {
            if (m_repository->m_trackSettingsMesageSize != (messageSize - 1)){
                qDebug() << "RMO_VOI_TRACK_SETTING_MESSAGE размеры не соотвествующие:получили " <<messageSize << " и должно быть " <<  m_repository->m_trackSettingsMesageSize;
                in.skipRawData(messageSize - 1);
            }
            else  {
                RMOTrackSetting settings;
                in.readRawData((char*) &settings, messageSize - 1);
                m_repository->appendTrack(settings);
                qDebug() << "получили RMO_VOI_TRACK_SETTING_MESSAGE";
            }
            break;
        }
        case VOI_RMO_TRACK_DELETE_MESSAGE: //106 Сообщение об удалении трассы от вторички
        {
            if (m_repository->m_trackSettingsDeleteSize != (messageSize - 1)){
                qDebug() << "VOI_RMO_TRACK_DELETE_MESSAGE размеры не соотвествующие:получили " <<messageSize << " и должно быть " <<  m_repository->m_trackSettingsDeleteSize;
                in.skipRawData(messageSize - 1);
            }
            else  {
                DeleteTrackMessage deleteTrack;
                in.readRawData((char*) &deleteTrack, messageSize - 1);
                m_repository->deleteTrack(deleteTrack.aimID);
                qDebug() << "получили VOI_RMO_TRACK_DELETE_MESSAGE";
            }
            break;
        }
        case RMO_VOI_TARGET_POSITION_MESSAGE: //3 Сообщение с параметрами ЦУ
        {
            if (m_repository->m_targetPositionMessageSize != (messageSize - 1)){
                qDebug() << "RMO_VOI_TARGET_POSITION_MESSAGE размеры не соотвествующие:получили " <<messageSize << " и должно быть " <<  m_repository->m_targetPositionMessageSize;
                in.skipRawData(messageSize - 1);
            }
            else  {
                RMOTargetPositionMessage targetPosition;
                in.readRawData((char*) &targetPosition, messageSize - 1);
                m_repository->appendTargetPosition(targetPosition);
                qDebug() << "получили RMO_VOI_TARGET_POSITION_MESSAGE";
            }
            break;
        }
        case RMO_VOI_TARGET_DELETE_MESSAGE: //4 Сообщение с удалением ЦУ
        {
            if (m_repository->m_targetPositionDeleteSize != (messageSize - 1)){
                qDebug() << "RMO_VOI_TARGET_DELETE_MESSAGE размеры не соотвествующие:получили " <<messageSize << " и должно быть " <<  sizeof(RMO_VOI_TARGET_POSITION_MESSAGE);
                in.skipRawData(messageSize - 1);
            }
            else  {
                RMOTargetDeleteMessage targetDelete;
                in.readRawData((char*) &targetDelete, messageSize - 1);
                m_repository->deleteTargetPosition(targetDelete.id);
                qDebug() << "получили RMO_VOI_TARGET_DELETE_MESSAGE";
            }
            break;
        }
        case RMO_VOI_DRIVE_TO_ROSITION_MESSAGE://6 Cообщение с данными куда смотреть антенне
        {
            if (m_repository->m_driveToPositionMessageSize != (messageSize - 1)){
                qDebug() << "RMO_VOI_DRIVE_TO_ROSITION_MESSAGE размеры не соотвествующие:получили " <<messageSize << " и должно быть " <<  m_repository->m_driveToPositionMessageSize;
                in.skipRawData(messageSize - 1);
            }
            else  {
                m_dataContains=true;
                RMODriveToPositionMessage message;
                in.readRawData((char*) &message, messageSize - 1);
                m_repository->setDriveToPosition(message);
                qDebug() << "получили RMO_VOI_DRIVE_TO_ROSITION_MESSAGE";
            }
            break;
        }
        case RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE: //9 Cообщение с сектором биологической защиты для ВОИ
        {
            if (m_repository->m_defenceSectorMessageSize != (messageSize - 1)){
                qDebug() << "RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE размеры не соотвествующие:получили " <<messageSize << " и должно быть " <<  m_repository->m_defenceSectorMessageSize;
                in.skipRawData(messageSize - 1);
            }
            else  {
                RMOBioDefenceSectorMessage message;
                in.readRawData((char*) &message, messageSize - 1);
                m_repository->editBioDefenceSectorList(message);
                qDebug() << "получили RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE";
            }
            break;
        }
        case RMO_OTHER_FORGET_ALL_DATA://14 отчистка всех сообщений
        {
            if (m_repository->m_deleteMessageSize != (messageSize - 1)){
                qDebug() << "RMO_OTHER_FORGET_ALL_DATA размеры не соотвествующие:получили " <<messageSize << " и должно быть " <<  m_repository->m_deleteMessageSize;
                in.skipRawData(messageSize - 1);
            }
            else  {
                m_dataContains=false;
                m_repository->clearAllArrays();
                qDebug() << "получили RMO_OTHER_FORGET_ALL_DATA";
            }
            break;
        }
        case RARM_SYSTEM_CONTROL_MESSAGE:// 255 Сообщение с состоянием подсистем
        {
            if (m_repository->m_rarmSystemControlSize != (messageSize - 1)){
                qDebug() << "RARM_SYSTEM_CONTROL_MESSAGE размеры не соотвествующие:получили " <<messageSize << " и должно быть " <<  m_repository->m_rarmSystemControlSize;
                in.skipRawData(messageSize - 1);
            }
            else  {
                RARMSysControlMessage message;
                in.readRawData((char*) &message, messageSize - 1);
                checkRmoAndVoiStates(message.sysControl);
                qDebug() << "получили RARM_SYSTEM_CONTROL_MESSAGE";
            }
            break;
        }
        default: {
            qDebug() << "Получено сообщение " << messageId << "  не обрабатываем! Убери из списка или напиши обработку";
            if (!isRarmConnected()){
                qDebug() << "Рарм не подключен";
                gettingMessageArray.clear();
                return;
            }

            if ((messageSize - 1) > (gettingMessageArray.size() - in.device()->pos())) {
                gettingMessageArray.clear();
                qDebug() << "Отчищаем буфер сообщений";
                return;
            }
            else {
                in.skipRawData(messageSize - 1);
                qDebug() << "Пропустили " <<messageSize << " байт";
            }

            break;
        }
        }
        if ((gettingMessageArray.size() - in.device()->pos()) > 3) {
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
    gettingMessageArray.remove(0, in.device()->pos());
}

void SocketToRarm::connectionError(QAbstractSocket::SocketError socketError)
{
    if (socketError ==  QAbstractSocket::ConnectionRefusedError ||
            socketError ==  QAbstractSocket::HostNotFoundError ||
            socketError ==  QAbstractSocket::SocketTimeoutError ||
            socketError ==  QAbstractSocket::NetworkError) {
        qDebug() << "Пробуем получить соединение снова..." << socketError;
        reconnectToRarm();
    }
    else
        qDebug() << "Ошибка: " << socketError;
}

void SocketToRarm::disconnected()
{
    qDebug() << "Отсоединились от РАРМ...";
    disconnect(m_pTcpSocketToRarm, &QTcpSocket::readyRead, this, &SocketToRarm::getMessage);
}
