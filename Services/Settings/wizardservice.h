#ifndef SERVICES_SETTINGS_WIZARDSERVICE_H
#define SERVICES_SETTINGS_WIZARDSERVICE_H

#include "Services/isqlservice.h"

#include "Services/Settings/roleappsandstartupsettingsrepository.h"
#include "Services/Settings/usersettingsrepository.h"
#include "Services/Settings/programfilesstate.h"

#include "Admin_GUI/Views/Wizards/actions.h"

#include "Structs/programstruct.h"
#include "Structs/userstruct.h"

class WizardService : public QObject
{
    Q_OBJECT
public:

    explicit WizardService(const QString &rlsTiFolder, LoadingState state, LinuxUserService *service, ISqlDatabaseService *iSqlDataBaseService, QObject *parent);

    ~WizardService();

Q_SIGNALS:

    void ToWizardFinished();

public Q_SLOTS:

    void OnSetActionWithUserRepository(const QString &actionWithUserRepository);

public:

    bool CheckAndParseBackupFile(const QString &backupPath);

    bool HasUserBackup() const;

    bool HasProgramBackUp() const;

    bool HasUserOldData() const;

    bool HastProgramOldData() const;

    void GetDataFromUserRepository(const bool isOldData, QString &FCS, QString &rank, QList<User> &userList);

    int GetUserCountFromUserRepository(const bool isOldData) const;

    void GetDataFromDesktopRepository(const int roleIndex, const bool isOldData, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs);

    int GetUserCountFromDesktopRepository(const int roleIndex, const bool isOldData);

    QString &GetActionWithUserRepository();

    void SetActionWithRoleRepository(const int roleIndex, const QString &actionWithRoleRepository);

    const QString &GetActionWithRoleRepository(const int roleIndex);

    void ApplyWizardActions();

private:

    void GetExsistsRepositoriesData(LoadingState &state);

    void GetExsistsUsersListFromDb();

    void GetExsistsExecsAndDesktopFilesFromDb();

    void ParseBackupFile(const QString &backupPath, QDomDocument &backupXMLDocument);

private:

    void ApplySettingsWithUserRepository(const QString &actionWithUserRepository, UsersDataWizardRepository *backupRepository, UsersDataWizardRepository *oldRepository);

    void ApplySettingsWithRolesRepository(const QStringList &actionsWithRoleRepository, RolesAndStartupsWizardRepository *backupRepository, RolesAndStartupsWizardRepository *oldRepository);

    void CopyFilesFromRoleToFolder(const QString &sourceFolder, const QStringList programs);

private:

    Terminal *m_terminal;

private:

    const QString m_rlsTiFolder;

    QString m_backupFolder;

    QString m_actionWithUserRepository;

    QStringList m_actionWithRolesRepository;

    UsersDataWizardRepository *m_oldDataCurrentUserWizardRepositry;

    UsersDataWizardRepository *m_backupDataUserWizardRepositry;

    RolesAndStartupsWizardRepository *m_oldDataRolesAndStartupsWizardRepository;

    RolesAndStartupsWizardRepository *m_backupDataRolesAndStartupsWizardRepository;

    ISqlDatabaseService *m_iSqlDatabaseService;

private:

    const QStringList m_backupCorrectTagsList = {"USERS", "FIRSTROLE", "SECONDROLE", "THIRDROLE", "FOURTHROLE"};

};

#endif // SERVICES_SETTINGS_WIZARDSERVICE_H
