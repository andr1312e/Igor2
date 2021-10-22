#include "wizardservice.h"

WizardService::WizardService(ProgramFilesState state, QString &currentUserName, QString currentUserId, QStringList validSettingsPaths, QStringList defaultValues, Terminal *terminal, QObject *parent)
   : QObject(parent)
   , m_oldDataCurrentUserWizardRepositry(new UsersDataWizardRepository(currentUserName))
   , m_backupDataUserWizardRepositry(new UsersDataWizardRepository(currentUserName))
   , m_oldDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(terminal))
   , m_backupDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(terminal))
   , m_backupXmlNodesList(
{ "USERS", "FIRSTROLE", "SECONDROLE", "THIRDROLE", "FOURTHROLE"
})
{
   SetOldRepositoriesData(state, currentUserName, validSettingsPaths, terminal);
}

WizardService::~WizardService()
{
   delete m_oldDataCurrentUserWizardRepositry;
   delete m_backupDataUserWizardRepositry;
   delete m_oldDataRolesAndStartupsWizardRepository;
   delete m_backupDataRolesAndStartupsWizardRepository;
}

void WizardService::SetOldRepositoriesData(ProgramFilesState state, QString &currentUserName, QStringList &validSettingsPaths, Terminal *terminal)
{
   switch (state) {
      case CantRun:
         qFatal("Недопустимый код");

      case NoFiles:
         break;

      case NoRoleData:
         TryToSetCurrentUserOldsFcsAndRank(validSettingsPaths);
         break;

      case NoUserDb:
         TryToSetOldExecsAndDesktopFiles(validSettingsPaths);
         break;

      case Fine:
         qFatal("Недопустимый код");
   }
}

void WizardService::TryToSetCurrentUserOldsFcsAndRank(QStringList &validSettingsPaths)
{
   m_oldDataCurrentUserWizardRepositry->SetFCSAndRolesFromDb(validSettingsPaths.first());
}

void WizardService::TryToSetOldExecsAndDesktopFiles(QStringList &validSettingsPaths)
{
   m_oldDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromFile(validSettingsPaths.front(), validSettingsPaths.last());
}

bool WizardService::CheckAndParseBackupFile(QString &backupPath)
{
   QFile m_backupFile;
   m_backupFile.setFileName(backupPath);

   if (m_backupFile.open(QIODevice::ReadOnly)) {
      QByteArray ar = m_backupFile.readAll();
      m_backupFile.close();
      QDomDocument m_backupXMLDocument;
      m_backupXMLDocument.setContent(ar);
      QDomElement settings = m_backupXMLDocument.firstChildElement();

      if (settings.tagName() == "settings") {
         QDomNodeList list = settings.childNodes();
         QStringList tagList;

         for (int i = 0; i < settings.childNodes().count(); i++) {
            tagList.append(list.at(i).toElement().tagName());
         }

         if (tagList == m_backupXmlNodesList) {
            ParseBackupFile(m_backupXMLDocument);
            return true;
         }
      }

      return false;
   }
}

bool WizardService::HasBackup()
{
   return m_backupDataUserWizardRepositry->HasData() || m_backupDataRolesAndStartupsWizardRepository->hasData();
}

bool WizardService::HasOldData()
{
   return m_oldDataCurrentUserWizardRepositry->HasData();;
}

void WizardService::ParseBackupFile(QDomDocument &backupXMLDocument)
{
   QDomElement settings = backupXMLDocument.firstChildElement();
   QDomNodeList nodeList = settings.childNodes();
   QDomElement users = nodeList.at(0).toElement();
   m_backupDataUserWizardRepositry->GetFCSAndRolesFromXml(users);

   for (int i = 1; i < 5; i++) {
      QDomElement role = nodeList.at(i).toElement();
      m_backupDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromBackup(i - 1, role);
   }
}

void WizardService::ApplySettingsWithUserRepository()
{
   if (m_actionWithUserRepository == userWizardPageComboBoxBackupAndOldDataActions.at(0)) {
      //m_backupDataUserWizardRepositry->WriteToFile(nullptr, )//basic sett
   } else {
      if (m_actionWithUserRepository == userWizardPageComboBoxBackupAndOldDataActions.at(1)) {
         //m_backupDataUserWizardRepositry->WriteToFile(nullptr, ) basic
      } else {
         //m_backupDataUserWizardRepositry->WriteToFile(nullptr, ) current
      }
   }
}

void WizardService::ApplySettingsWithRolesRepository()
{
   for (int i = 0; i < m_actionWithRolesRepository.count(); i++) {

   }


}

void WizardService::GetDataFromUserRepository(bool isOldData, QString &FCS, QString &rank, QVector<User> &userList)
{
   UsersDataWizardRepository *currentRepository;

   if (isOldData) {
      currentRepository = m_oldDataCurrentUserWizardRepositry;
   } else {
      currentRepository = m_backupDataUserWizardRepositry;
   }

   if (currentRepository->HasData()) {
      FCS = currentRepository->GetCurrentUserFCS();
      rank = currentRepository->GetCurrentUserRank();
      userList = currentRepository->GetUsersList();
   }
}

int WizardService::GetUserCountFromUserRepository(bool isOldData)
{
   if (isOldData) {
      return m_oldDataCurrentUserWizardRepositry->GetUserCount();
   } else {
      return m_backupDataUserWizardRepositry->GetUserCount();
   }
}

void WizardService::GetDataFromDesktopRepository(int roleIndex, bool isOldData, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs)
{
   RolesAndStartupsWizardRepository *currentRepository;

   if (isOldData) {
      currentRepository = m_oldDataRolesAndStartupsWizardRepository;
   } else {
      currentRepository = m_backupDataRolesAndStartupsWizardRepository;
   }

   if (currentRepository->hasData()) {
      currentRepository->RetunRoleDesktopsAndStartups(roleIndex, roleDesktops, roleExecs);
   }
}

int WizardService::GetUserCountFromDesktopRepository(int roleIndex, bool isOldData)
{
   if (isOldData) {
      return m_oldDataRolesAndStartupsWizardRepository->GetRoleDesktopsAppCount(roleIndex);
   } else {
      return m_backupDataRolesAndStartupsWizardRepository->GetRoleDesktopsAppCount(roleIndex);
   }
}

void WizardService::SetActionWithUserRepository(const QString &actionWithUserRepository)
{
   m_actionWithUserRepository = actionWithUserRepository;
}

QString &WizardService::GetActionWithUserRepository()
{
   return m_actionWithUserRepository;
}

void WizardService::SetActionWithRoleRepository(int roleIndex, const QString &actionWithRoleRepository)
{
   m_actionWithRolesRepository[roleIndex] = actionWithRoleRepository;
}

const QString &WizardService::GetActionWithRoleRepository(int roleIndex)
{
   return m_actionWithRolesRepository.at(roleIndex);
}

void WizardService::ApplySettings()
{
   ApplySettingsWithUserRepository();
}
