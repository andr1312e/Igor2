#ifndef SERVICES_SETTINGS_WIZARDSERVICE_H
#define SERVICES_SETTINGS_WIZARDSERVICE_H

#include <QDir>
#include <QGuiApplication>


#include "Services/Sql/isqlservice.h"

#include "Services/Settings/roleappsandstartupsettingsrepository.h"
#include "Services/Settings/usersettingsrepository.h"
#include "Services/Settings/programfilesstate.h"
#include "Services/Settings/dependenciesserivce.h"

#include "Admin_GUI/Views/Wizards/actions.h"

#include "Structs/programstruct.h"
#include "Structs/userstruct.h"

class WizardService : public QObject
{
    Q_OBJECT
public:
    explicit WizardService(const QString &rlsTiFolder,const LoadingState &state, LinuxUserService *service, ISqlDatabaseService *iSqlDataBaseService, QObject *parent);
    ~WizardService();
public Q_SLOTS:
    void SetActionWithUserRepository(const QString &actionWithUserRepository);

public:
    bool CheckAndParseBackupFile(const QString &backupPath);
    bool HasUserData(bool isOldData) const;
    bool HasAnyRolesData(bool isOldData) const;
    bool HasRoleIdAnyData(bool isOldData, quint8 roleId) const;

    void GetDataToViewFromUserRepository(const bool isOldData, QString &FCS, QString &rank, QList<User> &userList);
    int GetUserCountFromUserRepository(const bool isOldData) const;

    void GetDataToViewFromDesktopRepository(const int roleIndex, const bool isOldData, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs);
    int GetAppsCountFromDesktopRepository(const int roleIndex, const bool isOldData) const;


    const QString &GetActionWithUserRepository() const;
    void SetActionWithRoleRepository(const int roleIndex, const QString &actionWithRoleRepository);
    const QString &GetActionWithRoleRepository(const int roleIndex) const;
    const QStringList &GetAllDependenciesList() const;

    void ApplyWizardActions();

private:
    void GetExsistsRepositoriesData(const LoadingState &state);
    void GetExsistsUsersListFromDb();
    void GetExsistsExecsAndDesktopFilesFromDb();
    void ParseBackupFile(const QString &backupPath,const QDomDocument &backupXMLDocument);

private:
    void ApplySettingsWithUserRepository(const QString &actionWithUserRepository, UsersDataWizardRepository *backupRepository, UsersDataWizardRepository *oldRepository);
    void ApplySettingsWithRolesRepository(const QStringList &actionsWithRoleRepository, RolesAndStartupsWizardRepository *backupRepository);
    void ApplySettingsWithDependencies();
    void CopyFilesFromRoleToFolder(const QString &sourceFolder, const QStringList programs);

private:
    Terminal* const m_terminal;
    QSharedPointer<DependenciesService> m_dependenciesService;
private:
    const QString m_rlsTiFolder;

    QString m_backupFolderPath;
    QString m_actionWithUserRepository;

    QStringList m_actionWithRolesRepository;



    UsersDataWizardRepository* const m_oldDataCurrentUserWizardRepositry;
    UsersDataWizardRepository* const m_backupDataUserWizardRepositry;

    RolesAndStartupsWizardRepository* const m_oldDataRolesAndStartupsWizardRepository;
    RolesAndStartupsWizardRepository* const m_backupDataRolesAndStartupsWizardRepository;

    ISqlDatabaseService* const m_iSqlDatabaseService;

private:
    const QString m_globalTagName="settings";
    const QStringList m_backupCorrectTagsList = {"USERS", "FIRSTROLE", "SECONDROLE", "THIRDROLE", "FOURTHROLE", "DEPENDENCIES"};

};

#endif // SERVICES_SETTINGS_WIZARDSERVICE_H
