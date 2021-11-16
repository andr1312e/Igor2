#ifndef SERVER_MESSAGESREPOSITORY_H
#define SERVER_MESSAGESREPOSITORY_H
#include <QList>

#include "Server/DataMessage.h"

#define messageIdSize sizeof(quint8)


class MessagesRepository
{
public:
   MessagesRepository();
   ~MessagesRepository();

   //    const auto idSize=sizeof(quint8);

   QList<RMOTrackSetting> *m_trackSettingList;
   const quint8 m_trackSettingMessageId = MESSAGES_ID::RMO_VOI_TRACK_SETTING_MESSAGE;
   const quint8 m_trackSettingsMesageSize = messageIdSize + sizeof(RMOTrackSetting);
   void appendTrack(RMOTrackSetting newSettings);//1 Сообщение о проритетности трассы

   const quint16 m_trackSettingsDeleteSize = messageIdSize + sizeof(DeleteTrackMessage);
   void deleteTrack(quint32 aimID);//106 Сообщение об удалении трассы от вторички
   void clearAllTraks();


   QList<RMOTargetPositionMessage> *m_targetPositionsList;
   const quint16 m_targetPositionMessageSize = messageIdSize + sizeof(RMOTargetPositionMessage);
   const quint8 m_targetPositionMessageId = MESSAGES_ID::RMO_VOI_TARGET_POSITION_MESSAGE;

   void appendTargetPosition(RMOTargetPositionMessage newSettings);//3 Сообщение с параметрами ЦУ
   const quint16 m_targetPositionDeleteSize = sizeof(RMOTargetDeleteMessage);
   void deleteTargetPosition(quint8 id);//4 Удаление ЦУ
   void clearAllTargetPositions();


   RMODriveToPositionMessage m_driveToPositionMessage;
   const quint16 m_driveToPositionMessageSize = messageIdSize + sizeof(RMODriveToPositionMessage);
   const quint8 m_driveToPositionMessageId = MESSAGES_ID::RMO_VOI_DRIVE_TO_ROSITION_MESSAGE;

   void setDriveToPosition(RMODriveToPositionMessage message);//6 Сообщение с данными куда смотреть антенне


   QList<RMOBioDefenceSectorMessage> *m_defenceSectorMessageList;
   const quint16 m_defenceSectorMessageSize = messageIdSize + sizeof(RMOBioDefenceSectorMessage);
   const quint8 m_defenceSectorMessageId = MESSAGES_ID::RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE;
   void editBioDefenceSectorList(RMOBioDefenceSectorMessage newSettings);//9 Сообщение с сектором биологической защиты
   void clearAllBioDefence();

   const quint16 m_rarmSystemControlSize = messageIdSize + sizeof(RARMSysControlMessage);
   const quint16 m_deleteMessageSize = messageIdSize + sizeof (RMOForgetAllDataMessage);
   void clearAllArrays();
};

#endif // SERVER_MESSAGESREPOSITORY_H
