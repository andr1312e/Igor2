#ifndef SERVICES_SETTINGS_USERSETTINGSREPOSITORY_H
#define SERVICES_SETTINGS_USERSETTINGSREPOSITORY_H

#include <QList>
#include <QString>
#include <QDomElement>

#include "Structs/user.h"

#include "Services/Sql/isqlservice.h"
#include "Services/linuxuserservice.h"

class UsersDataWizardRepository
{

public:
    explicit UsersDataWizardRepository(LinuxUserService *service);
    ~UsersDataWizardRepository();

public:
    bool HasData() const noexcept;
    const QString &GetCurrentUserFCS() const;
    const QList<User> &GetUsersList() const;
    int GetUserCount() const noexcept;

    void GetUsersFromDb(ISqlDatabaseService *m_iSqlDatabaseService);
    void GetUsersFromXmL(const QDomElement &usersNode);

    void UpdateAdminFCS(const QString &fcs);
    void UpdateUser(const QString &userId, const QString &userName, const QString &fcs);
    void UpdateUser(const QString &userId, const QString &userName, int roleIndex);

    void SaveUsers(ISqlDatabaseService *m_iSqlDatabaseService, bool adminOnly);
    void Clear();

private:

    bool UserHasAllAtributes(const QDomElement &userDomElement) const noexcept;
    bool FindCurrentUserName(const QDomElement &userDomElement) const noexcept;
    void ParceCurrentUser(const QDomElement &currentUserDomElement) noexcept;
    void ParceUser(const QDomElement &userDomElement) noexcept;
    bool HasAdmin() noexcept;
    void UpdateAdminRole() noexcept;
    void GetCurrentUserFCS();

    int GetKorrektUserRole(const QDomElement &userDomElement) const;
    void JuxtaposeRepUserIdWithSystemUserId();
    void RemoveDublicates();
    void AppendAdminToCacheList();
    void WriteAllUsersToDatabase(ISqlDatabaseService *m_iSqlDatabaseService);
    void WriteAdminToDatabase(ISqlDatabaseService *m_iSqlDatabaseService);

private:
    LinuxUserService *const m_service;

private:
    QString m_currentUserFCS;
    QList<User> m_usersList;
};

#endif // SERVICES_SETTINGS_USERSETTINGSREPOSITORY_H
