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

   explicit WizardService(ProgramState state, const  QString &currentUserName, const  QString &currentUserId, QStringList &validSettingsPaths, const QStringList &defaultValues, Terminal *terminal, QObject *parent);

   ~WizardService();

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

   void ApplyWizard();

public Q_SLOTS:

   void SetActionWithUserRepository(const QString &actionWithUserRepository);

private:

   QStringList m_validSettingsPaths;

   const QStringList m_defaultSettingsValues;

   Terminal *m_terminal;

private:

   QString m_actionWithUserRepository;

   QStringList m_actionWithRolesRepository = {"", "", "", ""};

   UsersDataWizardRepository *m_oldDataCurrentUserWizardRepositry;

   UsersDataWizardRepository *m_backupDataUserWizardRepositry;

   RolesAndStartupsWizardRepository *m_oldDataRolesAndStartupsWizardRepository;

   RolesAndStartupsWizardRepository *m_backupDataRolesAndStartupsWizardRepository;

private:

   const QStringList m_backupCorrectTagsList = {"USERS", "FIRSTROLE", "SECONDROLE", "THIRDROLE", "FOURTHROLE"};

Q_SIGNALS:

   void BackupIsCorrect(bool value);

   void SetFCSForm(bool isOldData, QString &adminFCS, QString &adminRank);

   void Finished();

private:

   void SetOldRepositoriesData(ProgramState state, QStringList &validSettingsPaths,  Terminal *terminal);

   void TryToSetCurrentUserOldsFcsAndRank(QStringList &validSettingsPaths);

   void TryToSetOldExecsAndDesktopFiles(QStringList &validSettingsPaths);

   void ParseBackupFile(QDomDocument &backupXMLDocument);

private:

   void ApplySettingsWithUserRepository();

   void ApplySettingsWithRolesRepository();

};

#endif // SERVICES_SETTINGS_WIZARDSERVICE_H
