#ifndef SERVICES_SETTINGS_WIZARDSERVICE_H
#define SERVICES_SETTINGS_WIZARDSERVICE_H

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

    explicit WizardService(LoadingState state, const  QString &currentUserName, const  QString &currentUserId, QStringList &validSettingsPaths, const QStringList &defaultValues, Terminal *terminal, QObject *parent);

    ~WizardService();

Q_SIGNALS:

    void ToSetFCSForm(bool isOldData, QString &adminFCS, QString &adminRank);

    void ToWizardFinished();

public Q_SLOTS:

    void OnSetActionWithUserRepository(const QString &actionWithUserRepository);

public:

    bool CheckAndParseBackupFile(const QString &backupPath);

    bool HasBackup() const;

    bool HasOldData() const;

    void GetDataFromUserRepository(const bool isOldData, QString &FCS, QString &rank, QVector<User> &userList);

    int GetUserCountFromUserRepository(const bool isOldData) const;

    void GetDataFromDesktopRepository(const int roleIndex, const bool isOldData, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs);

    int GetUserCountFromDesktopRepository(const int roleIndex, const bool isOldData);

    QString &GetActionWithUserRepository();

    void SetActionWithRoleRepository(const int roleIndex, const QString &actionWithRoleRepository);

    const QString &GetActionWithRoleRepository(const int roleIndex);

    QStringList ApplyWizard();

    QStringList &GetResultSettingsPaths();

private:

    void SetOldRepositoriesData(LoadingState &state, QStringList &validSettingsPaths);

    void TryToSetCurrentUserOldsFcsAndRank(QStringList &validSettingsPaths);

    void TryToSetOldExecsAndDesktopFiles(QStringList &validSettingsPaths);

    void ParseBackupFile(QDomDocument &backupXMLDocument);

private:

    const QString &ApplySettingsWithUserRepositoryAndReturnPath(const QString &actionWithUserRepository, const QStringList &validSettingsPaths, const QStringList &defautSettingsValues, UsersDataWizardRepository *backupRepository, UsersDataWizardRepository *oldRepository);

    const QStringList ApplySettingsWithRolesRepository(const QStringList &actionsWithRoleRepository, const QStringList &validSettingsPaths, const QStringList &defautSettingsValues, RolesAndStartupsWizardRepository *backupRepository, RolesAndStartupsWizardRepository *oldRepository);

    const QStringList GetPathsFromRolesAndExecs(const QStringList &actionsWithRoleRepository, const QStringList &validSettinsPath, const QStringList &defaultSettingsPaths) const;

private:

    QStringList m_validSettingsPaths;

    const QStringList m_defaultSettingsValues;

    QStringList m_resultSettingsPaths;

    Terminal *m_terminal;

private:

    QString m_actionWithUserRepository;

    QStringList m_actionWithRolesRepository;

    UsersDataWizardRepository *m_oldDataCurrentUserWizardRepositry;

    UsersDataWizardRepository *m_backupDataUserWizardRepositry;

    RolesAndStartupsWizardRepository *m_oldDataRolesAndStartupsWizardRepository;

    RolesAndStartupsWizardRepository *m_backupDataRolesAndStartupsWizardRepository;

private:

    const QStringList m_backupCorrectTagsList = {"USERS", "FIRSTROLE", "SECONDROLE", "THIRDROLE", "FOURTHROLE"};

};

#endif // SERVICES_SETTINGS_WIZARDSERVICE_H
