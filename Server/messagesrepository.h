#ifndef SERVER_MESSAGESREPOSITORY_H
#define SERVER_MESSAGESREPOSITORY_H
#include <QLinkedList>

#include "Server/DataMessage.h"

class MessagesRepository
{
public:
    explicit MessagesRepository();
    ~MessagesRepository();
private:
    const quint8 m_messageIdSize = (quint8)(sizeof(quint8));
public:
    const quint8 m_trackSettingMessageId = MESSAGES_ID::RMO_VOI_TRACK_SETTING_MESSAGE;
    const quint8 m_trackSettingsMesageSize = m_messageIdSize + sizeof(RMOTrackSetting);
    void AppendTrack(RMOTrackSetting newSettings);//1 Сообщение о проритетности трассы

    const quint16 m_trackSettingsDeleteSize = m_messageIdSize + sizeof(DeleteTrackMessage);
    void DeleteTrack(const int myAimID);//106 Сообщение об удалении трассы от вторички
    void ClearAllTraks();
    const QLinkedList<RMOTrackSetting> &GetTrackList();

public:
    const quint16 m_targetPositionMessageSize = m_messageIdSize + sizeof(RMOTargetPositionMessage);
    const quint8 m_targetPositionMessageId = MESSAGES_ID::RMO_VOI_TARGET_POSITION_MESSAGE;

    void AppendTargetPosition(RMOTargetPositionMessage newSettings);//3 Сообщение с параметрами ЦУ
    const quint16 m_targetPositionDeleteSize = sizeof(RMOTargetDeleteMessage);
    void DeleteTargetPosition(quint8 id);//4 Удаление ЦУ
    void ClearAllTargetPositions();
    const QLinkedList<RMOTargetPositionMessage> &GetAllTargetPositionsList();

public:
    const quint16 m_driveToPositionMessageSize = m_messageIdSize + sizeof(RMODriveToPositionMessage);
    const quint8 m_driveToPositionMessageId = MESSAGES_ID::RMO_VOI_DRIVE_TO_ROSITION_MESSAGE;

    void SetDriveToPosition(RMODriveToPositionMessage message);//6 Сообщение с данными куда смотреть антенне
    const RMODriveToPositionMessage &GetRMODriveToPositionMessage();

public:
    const quint16 m_defenceSectorMessageSize = m_messageIdSize + sizeof(RMOBioDefenceSectorMessage);
    const quint8 m_defenceSectorMessageId = MESSAGES_ID::RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE;

    void EditBioDefenceSectorList(RMOBioDefenceSectorMessage newSettings);//9 Сообщение с сектором биологической защиты
    void ClearAllBioDefence();
    const QLinkedList<RMOBioDefenceSectorMessage> &GetRMOBioDefenceSectorList();

public:
    const quint16 m_rarmSystemControlSize = m_messageIdSize + sizeof(RARMSysControlMessage);
    const quint16 m_deleteMessageSize = m_messageIdSize + sizeof (RMOForgetAllDataMessage);
    void ClearAllArrays();
private:
    QLinkedList<RMOTrackSetting> m_trackSettingList;
    QLinkedList<RMOTargetPositionMessage> m_targetPositionsList;
    RMODriveToPositionMessage m_driveToPositionMessage;
    QLinkedList<RMOBioDefenceSectorMessage> m_defenceSectorMessageList;

};

#endif // SERVER_MESSAGESREPOSITORY_H
