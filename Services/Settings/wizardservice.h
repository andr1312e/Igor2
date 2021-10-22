#ifndef WIZSERV_H
#define WIZSERV_H

#include <QObject>

#include "roleappsandstartupsettingsrepository.h"
#include "usersettingsrepository.h"

#include "Admin_GUI/Views/Wizards/actions.h"

#include "Services/Settings/programfilesstate.h"
#include "Structs/programstruct.h"
#include "Structs/userstruct.h"

class WizardService : public QObject
{
   Q_OBJECT
public:
   explicit WizardService(ProgramFilesState state, QString &currentUserName, QString currentUserId, QStringList validSettingsPaths, QStringList defaultValues, Terminal *terminal, QObject *parent);

   ~WizardService();

   bool CheckAndParseBackupFile(QString &backupPath);

   bool HasBackup();

   bool HasOldData();

   void GetDataFromUserRepository(bool isOldData, QString &FCS, QString &rank, QVector<User> &userList);

   int GetUserCountFromUserRepository(bool isOldData);

   void GetDataFromDesktopRepository(int roleIndex, bool isOldData, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs);

   int GetUserCountFromDesktopRepository(int roleIndex, bool isOldData);

   void getUsersList();

   void SetActionWithUserRepository(const QString &actionWithUserRepository);

   QString &GetActionWithUserRepository();

   void SetActionWithRoleRepository(int roleIndex, const QString &actionWithRoleRepository);

   const QString &GetActionWithRoleRepository(int roleIndex);

   void ApplySettings();

private:

   QString m_actionWithUserRepository;

   QStringList m_actionWithRolesRepository = {"", "", "", ""};

   UsersDataWizardRepository *m_oldDataCurrentUserWizardRepositry;

   UsersDataWizardRepository *m_backupDataUserWizardRepositry;

   RolesAndStartupsWizardRepository *m_oldDataRolesAndStartupsWizardRepository;

   RolesAndStartupsWizardRepository *m_backupDataRolesAndStartupsWizardRepository;

private:

   QStringList m_backupXmlNodesList;

Q_SIGNALS:

   void backupIsCorrect(bool value);

   void setFCSForm(bool isOldData, QString &adminFCS, QString &adminRank);

   void finished();

private:

   void SetOldRepositoriesData(ProgramFilesState state, QString &currentUserName, QStringList &validSettingsPaths,  Terminal *terminal);

   void TryToSetCurrentUserOldsFcsAndRank(QStringList &validSettingsPaths);

   void TryToSetOldExecsAndDesktopFiles(QStringList &validSettingsPaths);

   void ParseBackupFile(QDomDocument &backupXMLDocument);

private:

   void ApplySettingsWithUserRepository();

   void ApplySettingsWithRolesRepository();

};

#endif // WIZSERV_H
