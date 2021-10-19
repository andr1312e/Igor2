#ifndef USERSETTINGSREPOSITORY_H
#define USERSETTINGSREPOSITORY_H
#include <QString>
#include <QFile>
#include <QDomDocument>
#include <QStringLiteral>

class CurrentUserWizardRepository
{

public:

    explicit CurrentUserWizardRepository(QString &curerntUserName);

    ~CurrentUserWizardRepository();

    const QString& getCurrentUserName() const;

    bool hasData() const;

    QString& GetCurrentUserFCS();

    QString& GetCurrentUserRank();

    int GetUsersCount() const;

    void setFCSAndRolesFromDb(QString &pathToUserDb);

    void getFCSAndRolesFromXml(QDomElement &usersNode);

private:

    const QString m_curerntUserName;

    bool m_hasData;

    int m_usersCount;

    QString m_userFCS;

    QString m_userRank;

};

#endif // USERSETTINGSREPOSITORY_H
