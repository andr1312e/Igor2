#include "messagesrepository.h"

MessagesRepository::MessagesRepository()
{
}

MessagesRepository::~MessagesRepository()
{
}

void MessagesRepository::AppendTrack(RMOTrackSetting newSettings)
{
    m_trackSettingList.push_front(newSettings);
}

void MessagesRepository::DeleteTrack(const int myAimID)
{
    QLinkedList<RMOTrackSetting>::iterator it = std::begin(m_trackSettingList);

    while (it != std::end(m_trackSettingList))
    {
        if (myAimID == it->aimID)
        {
            m_trackSettingList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void MessagesRepository::ClearAllTraks()
{
    m_trackSettingList.clear();
}

const QLinkedList<RMOTrackSetting> &MessagesRepository::GetTrackList()
{
    return m_trackSettingList;
}

void MessagesRepository::AppendTargetPosition(RMOTargetPositionMessage newPositionMessage)
{
    if (newPositionMessage.isUpdate == 1) {
        for (RMOTargetPositionMessage &message: m_targetPositionsList) {
            if (message.id == newPositionMessage.id) {
                message.sTimeMeasurement = newPositionMessage.sTimeMeasurement;
                message.azimuth[0] = newPositionMessage.azimuth[0];
                message.azimuth[1] = newPositionMessage.azimuth[1];
                message.ugm[0] = newPositionMessage.ugm[0];
                message.ugm[1] = newPositionMessage.ugm[1];
                message.dist[0] = newPositionMessage.dist[0];
                message.dist[1] = newPositionMessage.dist[1];
                message.speed[0] = newPositionMessage.speed[0];
                message.speed[1] = newPositionMessage.speed[1];
                message.work_mode = newPositionMessage.work_mode;
                message.set_mode = newPositionMessage.set_mode;
                message.freq_rays = newPositionMessage.freq_rays;
                message.DSP = newPositionMessage.DSP;
                message.out_dsp = newPositionMessage.out_dsp;
                message.count_answer = newPositionMessage.count_answer;
                message.answer[0] = newPositionMessage.answer[0];
                message.answer[1] = newPositionMessage.answer[1];
                message.answer[2] = newPositionMessage.answer[2];
                message.answer[3] = newPositionMessage.answer[3];
                message.answer[4] = newPositionMessage.answer[4];
                message.answer[5] = newPositionMessage.answer[5];
                message.answer[6] = newPositionMessage.answer[6];
                message.answer[7] = newPositionMessage.answer[7];
                message.answer[8] = newPositionMessage.answer[8];
                message.answer[9] = newPositionMessage.answer[9];
                message.look_priority = newPositionMessage.look_priority;
                return;
            }
        }
    } else {
        m_targetPositionsList.push_back(newPositionMessage);
    }
}

void MessagesRepository::DeleteTargetPosition(quint8 id)
{
    QLinkedList<RMOTargetPositionMessage>::iterator it = std::begin(m_targetPositionsList);

    while (it != std::end(m_targetPositionsList))
    {
        if (id == it->id)
        {
            m_targetPositionsList.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void MessagesRepository::ClearAllTargetPositions()
{
    m_targetPositionsList.clear();
}

const QLinkedList<RMOTargetPositionMessage> &MessagesRepository::GetAllTargetPositionsList()
{
    return m_targetPositionsList;
}

void MessagesRepository::SetDriveToPosition(RMODriveToPositionMessage message)
{
    m_driveToPositionMessage.Um = message.Um;
    m_driveToPositionMessage.Az = message.Az;
    m_driveToPositionMessage.exist = message.exist;
    m_driveToPositionMessage.sTimeMeasurement = message.sTimeMeasurement;
}

const RMODriveToPositionMessage &MessagesRepository::GetRMODriveToPositionMessage()
{
    return m_driveToPositionMessage;
}

void MessagesRepository::EditBioDefenceSectorList(RMOBioDefenceSectorMessage newSettings)
{
    if (1 == newSettings.method)
    { //удаление
        m_defenceSectorMessageList.clear();
    }
    else
    { //добавление или изменение
        for (RMOBioDefenceSectorMessage &message: m_defenceSectorMessageList)
        {
            if (message.id == newSettings.id) {
                message.azimuth[0] = newSettings.azimuth[0];
                message.azimuth[1] = newSettings.azimuth[1];
                return;
            }
        }
        m_defenceSectorMessageList.push_back(newSettings);
    }
}

void MessagesRepository::ClearAllBioDefence()
{
    m_defenceSectorMessageList.clear();
}

const QLinkedList<RMOBioDefenceSectorMessage> &MessagesRepository::GetRMOBioDefenceSectorList()
{
    return m_defenceSectorMessageList;
}

void MessagesRepository::ClearAllArrays()
{
    ClearAllBioDefence();
    ClearAllTargetPositions();
    ClearAllTraks();
}
