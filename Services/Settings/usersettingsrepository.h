#ifndef USERSETTINGSREPOSITORY_H
#define USERSETTINGSREPOSITORY_H
#include <QDomDocument>

#include "Services/Terminals/terminal.h"

class CurrentUserWizardRepository
{
public:
    CurrentUserWizardRepository(QString &currentUserId, QString &currentUserName, Terminal *terminal);

    ~CurrentUserWizardRepository();

    QString getCurrentUserId();

    QString getCurrentUserName();

    void setUserData(QDomDocument *doc);

    void setUserFCS(const QString &FCS);

     void setUserRank(const QString &rank);

private:

    Terminal *m_terminal;

    QString m_currentUserId;

    QString m_currentUserName;

    QString m_currentUserFCS;

    QString m_currentUserRank;

    QDomDocument *m_usersRepository;
};

#endif // USERSETTINGSREPOSITORY_H
