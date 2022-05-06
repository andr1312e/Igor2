#ifndef SERVICES_SETTINGS_WIZARDSERVICE_H
#define SERVICES_SETTINGS_WIZARDSERVICE_H

#include <QDir>
#include <QApplication>
#include <QStringList>

#include "Services/Sql/isqlservice.h"

#include "Admin_GUI/Wizard/Repositories/roleappsandstartupsettingsrepository.h"
#include "Admin_GUI/Wizard/Repositories/usersettingsrepository.h"

#include "Admin_GUI/Wizard/Structs/programfilesstate.h"
#include "Admin_GUI/Wizard/Services/dependenciesserivce.h"
#include "Admin_GUI/Wizard/Services/filesandfoldersserivce.h"

#include "Admin_GUI/Wizard/Views/actions.h"

#include "Structs/programstruct.h"
#include "Structs/userstruct.h"

enum DbWizardDataType{
    OldData = 0,
    BackupData = 1
};

class WizardService : public QObject
{
    Q_OBJECT
public:
    explicit WizardService(const QString &rlsTiFolder, const LoadingState &state, LinuxUserService *service, ISqlDatabaseService *iSqlDataBaseService, QObject *parent);
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
    bool CheckAndParseBackupFile(const QString &backupPath);
    const QString &GetBackupFilePath() const;
    void ClearBackup();

    bool HasUserData(DbWizardDataType dataType) const;
    bool HasAnyRolesData(DbWizardDataType dataType) const;
    bool HasRoleIdAnyData(DbWizardDataType dataType, int roleId) const;

    void GetDataToViewFromUserRepository(DbWizardDataType dataType, QString &FCS, QList<User> &userList) const;
    int GetUserCountFromUserRepository(DbWizardDataType dataType) const;

    void GetDataToViewFromDesktopRepository(DbWizardDataType dataType, int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs);
    int GetAppsCountFromDesktopRepository(DbWizardDataType dataType, int roleIndex) const;

    void SetActionWithUserRepository(int action);
    int GetActionIndexWithUserRepository() const;
    void SetActionWithRoleRepository(int roleIndex, int actionIndex);
    int GetActionWithRoleRepository(int roleIndex) const;
    const QStringList &GetAllDependenciesList() const;

private:
    RolesAndStartupsWizardRepository* GetRolesRepositoryType(DbWizardDataType dataType) const;
    UsersDataWizardRepository* GetUserReposiotry(DbWizardDataType dataType) const;
    void GetExsistsRepositoriesData(const LoadingState &state);
    void GetExsistsUsersListFromDb();
    void GetExsistsExecsAndDesktopFilesFromDb();
    void ParseBackupFile(const QString &backupPath, const QDomNodeList &backupNodeLists);

public:
    void ApplyWizardActions();
private:
    int GetAllActionsCount() const;
    void ApplySettingsWithUserRepository(int actionIndexWithUserRepository, UsersDataWizardRepository *backupRepository, UsersDataWizardRepository *oldRepository);
    void ApplySettingsWithRolesRepository(const QVarLengthArray<int, 4> &actionsWithRoleRepository, RolesAndStartupsWizardRepository *backupRepository);
    void ApplySettingsWithDependencies();
    void CopyFilesFromRoleToFolder(const QString &sourceFolder, const QStringList &programs);

private:
    Terminal* const m_terminal;
    DependenciesService * const m_dependenciesService;
    FilesAndFoldersSerivce * const m_filesAndFoldersSerivce;
private:
    const QString m_rlsTiFolder;

    QString m_backupFilePath;
    int m_indexActionWithUserRepository;

    QVarLengthArray<int, 4> m_actionWithRolesRepository;

    UsersDataWizardRepository* const m_oldDataCurrentUserWizardRepositry;
    UsersDataWizardRepository* const m_backupDataUserWizardRepositry;

    RolesAndStartupsWizardRepository* const m_oldDataRolesAndStartupsWizardRepository;
    RolesAndStartupsWizardRepository* const m_backupDataRolesAndStartupsWizardRepository;

    ISqlDatabaseService* const m_iSqlDatabaseService;

private:
    const QString m_globalTagName;
    const QStringList m_backupCorrectTagsList = {QStringLiteral("USERS"), QStringLiteral("FIRSTROLE"), QStringLiteral("SECONDROLE"), QStringLiteral("THIRDROLE"), QStringLiteral("FOURTHROLE"), QStringLiteral("DEPENDENCIES"), QStringLiteral("FILES"), QStringLiteral("FOLDERS")};

};

#endif // SERVICES_SETTINGS_WIZARDSERVICE_H
