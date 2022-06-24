#ifndef SERVICES_SETTINGS_WIZARDSERVICE_H
#define SERVICES_SETTINGS_WIZARDSERVICE_H

#include <QDir>
#include <QApplication>
#include <QStringList>

#include "Logging/logger.h"

#include "Services/Sql/isqlservice.h"

#include "Admin_GUI/RestoreWizard/Repositories/roleappsandstartupsettingsrepository.h"
#include "Admin_GUI/RestoreWizard/Repositories/usersettingsrepository.h"

#include "Admin_GUI/RestoreWizard/Structs/programfilesstate.h"
#include "Admin_GUI/RestoreWizard/Services/dependenciesserivce.h"
#include "Admin_GUI/RestoreWizard/Services/filesandfoldersserivce.h"
#include "Admin_GUI/RestoreWizard/Services/enviromentalvariables.h"

#include "Admin_GUI/RestoreWizard/Views/actionsRestore.h"

#include "Structs/programstruct.h"
#include "Structs/user.h"

enum class DbWizardDataType
{
    OldData = 0,
    BackupData = 1
};

class WizardService : public QObject
{
    Q_OBJECT
public:
    explicit WizardService(const QString &rlsTiFolder, LoadingStates states, LinuxUserService *service, ISqlDatabaseService *iSqlDataBaseService, QObject *parent);
    ~WizardService();
private:
    void ConnectObjects();
Q_SIGNALS:
    void ToSendBackupParceError(const QString &error);
    void ToCurrentActionDoing(const QString &packageName);
    void ToSetActionsCount(int actionsCount);
public Q_SLOTS:
    void OnChangeInstallState(bool state);
public:
    const QString &GetCurrentUserId() const noexcept;
    const QString &GetCurrentUserName() const noexcept;
public:
    bool CheckAndParseBackupFile(const QString &backupPath);
    const QString &GetBackupFilePath() const noexcept;
    void ClearBackup();

    const QString &GetBackupVersion() const noexcept;

    bool HasUserData(DbWizardDataType dataType) const;
    bool HasAnyRolesData(DbWizardDataType dataType) const;
    bool HasRoleIdAnyData(DbWizardDataType dataType, int roleIndex) const;

    void GetDataToViewFromUserRepository(DbWizardDataType dataType, QString &FCS, QList<User> &userList) const;
    int GetUserCountFromUserRepository(DbWizardDataType dataType) const;

    void GetDataToViewFromDesktopRepository(DbWizardDataType dataType, int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs);
    int GetAppsCountFromDesktopRepository(DbWizardDataType dataType, int roleIndex) const;

    void UpdateBackupUserFCS(const QString &userId, const QString &userName, const QString &fcs);
    void UpdateBackupUserFCS(const QString &userId, const QString &userName, int userRole);

    void UpdateAdminFCS(const QString &adminFCS, DbWizardDataType type);

    void SetActionWithUserRepository(int action);
    int GetActionIndexWithUserRepository() const noexcept;
    void SetActionWithRoleRepository(int roleIndex, int actionIndex);
    int GetActionWithRoleRepository(int roleIndex) const;
    const QStringList &GetAllDependenciesList() const;

private:
    RolesAndStartupsWizardRepository *GetRolesRepositoryType(DbWizardDataType dataType) const noexcept;
    UsersDataWizardRepository *GetUserReposiotry(DbWizardDataType dataType) const noexcept;
    void GetExsistsRepositoriesData(LoadingStates state);
    void GetExsistsUsersListFromDb();
    void GetExsistsExecsAndDesktopFilesFromDb();
    void ParseBackupFile(const QString &backupPath, const QDomNodeList &backupNodeLists);

public:
    void ApplyWizardActions();
private:
    int GetAllActionsCount() const;
    void ApplySettingsWithUserRepository(int actionIndexWithUserRepository, UsersDataWizardRepository *backupRepository, UsersDataWizardRepository *oldRepository);
    void ApplySettingsWithRolesRepository(const QVarLengthArray<int, 4> &actionsWithRoleRepository, RolesAndStartupsWizardRepository *backupRepository);
    void CopyFilesFromRoleToFolder(const QString &sourceFolder, const QStringList &programs);

private:
    Terminal *const m_terminal;
    DependenciesService *const m_dependenciesService;
    FilesAndFoldersSerivce *const m_filesAndFoldersSerivce;
    EnviromentalVariablesService *const m_enviromentalVariablesService;
private:
    const QString m_rlsTiFolder;
    const QString m_currentUserId;
    const QString m_currentUserName;

    QString m_backupFilePath;
    QString m_backupVersion;
    int m_indexActionWithUserRepository;

    QVarLengthArray<int, 4> m_actionWithRolesRepository;

    UsersDataWizardRepository *const m_oldDataCurrentUserWizardRepositry;
    UsersDataWizardRepository *const m_backupDataUserWizardRepositry;

    RolesAndStartupsWizardRepository *const m_oldDataRolesAndStartupsWizardRepository;
    RolesAndStartupsWizardRepository *const m_backupDataRolesAndStartupsWizardRepository;

    ISqlDatabaseService *const m_iSqlDatabaseService;

private:
    const QString m_globalTagName;
    const QStringList m_backupCorrectTagsList = {QLatin1Literal("USERS"), QLatin1Literal("FIRSTROLE"), QLatin1Literal("SECONDROLE"), QLatin1Literal("THIRDROLE"), QLatin1Literal("FOURTHROLE"), QLatin1Literal("DEPENDENCIES"), QLatin1Literal("FILES"), QLatin1Literal("FOLDERS"), QLatin1Literal("PROFILES")};

};

#endif // SERVICES_SETTINGS_WIZARDSERVICE_H
