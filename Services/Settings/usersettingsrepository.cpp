#include "usersettingsrepository.h"

CurrentUserWizardRepository::CurrentUserWizardRepository(QString &currentUserId, QString &currentUserName, Terminal *terminal)
    : m_terminal(terminal)
    , m_currentUserId(currentUserId)
    , m_currentUserName(currentUserName)
{

}

CurrentUserWizardRepository::~CurrentUserWizardRepository()
{

}

QString CurrentUserWizardRepository::getCurrentUserId()
{
    return m_currentUserId;
}

QString CurrentUserWizardRepository::getCurrentUserName()
{
    return m_currentUserName;
}

void CurrentUserWizardRepository::setUserData(QDomDocument *doc)
{

}

void CurrentUserWizardRepository::setUserFCS(const QString &FCS)
{
    m_currentUserFCS=FCS;
}

void CurrentUserWizardRepository::setUserRank(const QString &rank)
{
    m_currentUserRank=rank;
}
