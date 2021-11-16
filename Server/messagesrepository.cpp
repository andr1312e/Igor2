#include "messagesrepository.h"

MessagesRepository::MessagesRepository()
{
   m_trackSettingList = new QList<RMOTrackSetting>();
   m_targetPositionsList = new QList<RMOTargetPositionMessage>();
   m_defenceSectorMessageList = new QList<RMOBioDefenceSectorMessage>();
}

MessagesRepository::~MessagesRepository()
{
   delete m_defenceSectorMessageList;
   delete m_targetPositionsList;
   delete m_trackSettingList;
}

void MessagesRepository::appendTrack(RMOTrackSetting newSettings)
{
   m_trackSettingList->append(newSettings);
}

void MessagesRepository::deleteTrack(quint32 aimID)
{
   for (QList<RMOTrackSetting>::iterator it = m_trackSettingList->begin(); it != m_trackSettingList->end(); ++it) {

      if ((quint32)it->aimID == aimID) {
         it--;
         m_trackSettingList->erase(it);
         return;
      }
   }
}

void MessagesRepository::clearAllTraks()
{
   m_trackSettingList->clear();
}

void MessagesRepository::appendTargetPosition(RMOTargetPositionMessage newSettings)
{
   if (newSettings.isUpdate == 1) {
      for (QList<RMOTargetPositionMessage>::iterator it = m_targetPositionsList->begin(); it != m_targetPositionsList->end(); ++it) {
         if (it->id == newSettings.id) {
            it->sTimeMeasurement = newSettings.sTimeMeasurement;
            it->azimuth[0] = newSettings.azimuth[0];
            it->azimuth[1] = newSettings.azimuth[1];
            it->ugm[0] = newSettings.ugm[0];
            it->ugm[1] = newSettings.ugm[1];
            it->dist[0] = newSettings.dist[0];
            it->dist[1] = newSettings.dist[1];
            it->speed[0] = newSettings.speed[0];
            it->speed[1] = newSettings.speed[1];
            it->work_mode = newSettings.work_mode;
            it->set_mode[0] = newSettings.set_mode[0];
            it->set_mode[1] = newSettings.set_mode[1];
            it->freq_rays[0] = newSettings.freq_rays[0];
            it->freq_rays[1] = newSettings.freq_rays[1];
            it->DSP = newSettings.DSP;
            it->out_dsp = newSettings.out_dsp;
            it->count_answer = newSettings.count_answer;
            it->answer[0] = newSettings.answer[0];
            it->answer[1] = newSettings.answer[1];
            it->answer[2] = newSettings.answer[2];
            it->answer[3] = newSettings.answer[3];
            it->answer[4] = newSettings.answer[4];
            it->answer[5] = newSettings.answer[5];
            it->answer[6] = newSettings.answer[6];
            it->answer[7] = newSettings.answer[7];
            it->answer[8] = newSettings.answer[8];
            it->answer[9] = newSettings.answer[9];
            it->look_priority = newSettings.look_priority;
         }

         return;
      }
   } else {
      m_targetPositionsList->push_back(newSettings);
   }
}

void MessagesRepository::deleteTargetPosition(quint8 id)
{
   for (QList<RMOTargetPositionMessage>::iterator it = m_targetPositionsList->begin(); it != m_targetPositionsList->end(); ++it) {

      if (it->id == id) {
         it--;
         m_targetPositionsList->erase(it);
         return;
      }
   }
}

void MessagesRepository::clearAllTargetPositions()
{
   m_targetPositionsList->clear();
}

void MessagesRepository::setDriveToPosition(RMODriveToPositionMessage message)
{
   m_driveToPositionMessage.Um = message.Um;
   m_driveToPositionMessage.Az = message.Az;
   m_driveToPositionMessage.exist = message.exist;
   m_driveToPositionMessage.sTimeMeasurement = message.sTimeMeasurement;
}

void MessagesRepository::editBioDefenceSectorList(RMOBioDefenceSectorMessage newSettings)
{
   if (newSettings.method == 1) { //удаление
      for (QList<RMOBioDefenceSectorMessage>::iterator it = m_defenceSectorMessageList->begin(); it != m_defenceSectorMessageList->end(); ++it) {
         it--;
         m_defenceSectorMessageList->erase(it);
         return;
      }
   } else { //добавление или изменение
      for (QList<RMOBioDefenceSectorMessage>::iterator it = m_defenceSectorMessageList->begin(); it != m_defenceSectorMessageList->end(); ++it) {
         if (it->id == newSettings.id) {
            it->azimuth[0] = newSettings.azimuth[0];
            it->azimuth[1] = newSettings.azimuth[1];
            return;
         }
      }

      m_defenceSectorMessageList->push_back(newSettings);
   }
}

void MessagesRepository::clearAllBioDefence()
{
   m_defenceSectorMessageList->clear();
}

void MessagesRepository::clearAllArrays()
{
   clearAllBioDefence();
   clearAllTargetPositions();
   clearAllTraks();
}
