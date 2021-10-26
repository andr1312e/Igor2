#include "wizardservice.h"

WizardService::WizardService(ProgramState state, const QString &currentUserName, const QString &currentUserId, QStringList &validSettingsPaths, const QStringList &defaultSettingsValues, Terminal *terminal, QObject *parent)
   : QObject(parent)
   , m_validSettingsPaths(validSettingsPaths)
   , m_defaultSettingsValues(defaultSettingsValues)
   , m_terminal(terminal)
   , m_oldDataCurrentUserWizardRepositry(new UsersDataWizardRepository(currentUserName, currentUserId, terminal))
   , m_backupDataUserWizardRepositry(new UsersDataWizardRepository(currentUserName, currentUserId, terminal))
   , m_oldDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(terminal))
   , m_backupDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(terminal))
{
   SetOldRepositoriesData(state, validSettingsPaths, terminal);
}

WizardService::~WizardService()
{
   delete m_oldDataCurrentUserWizardRepositry;
   delete m_backupDataUserWizardRepositry;
   delete m_oldDataRolesAndStartupsWizardRepository;
   delete m_backupDataRolesAndStartupsWizardRepository;
}

void WizardService::SetOldRepositoriesData(ProgramState state, QStringList &validSettingsPaths, Terminal *terminal)
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
   m_oldDataCurrentUserWizardRepositry->SetFCSAndRolesFromFile(validSettingsPaths.first());
}

void WizardService::TryToSetOldExecsAndDesktopFiles(QStringList &validSettingsPaths)
{
   m_oldDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromFile(validSettingsPaths.front(), validSettingsPaths.last());
}

bool WizardService::CheckAndParseBackupFile(const QString &backupPath)
{
   QByteArray arr = m_terminal->GetFileText(backupPath, "WizardService::CheckAndParseBackupFile", true).toUtf8();
   QDomDocument m_backupXMLDocument;

   if (m_backupXMLDocument.setContent(arr)) {
      QDomElement settings = m_backupXMLDocument.firstChildElement();

      if (settings.tagName() == "settings") {
         QDomNodeList list = settings.childNodes();
         QStringList tagList;

         for (int i = 0; i < settings.childNodes().count(); i++) {
            tagList.append(list.at(i).toElement().tagName());
         }

         if (tagList == m_backupCorrectTagsList) {
            ParseBackupFile(m_backupXMLDocument);
            return true;
         }
      }
   }

   return false;
}

bool WizardService::HasBackup() const
{
   return m_backupDataUserWizardRepositry->HasData() || m_backupDataRolesAndStartupsWizardRepository->HasData();
}

bool WizardService::HasOldData() const
{
   return m_oldDataCurrentUserWizardRepositry->HasData() || m_oldDataRolesAndStartupsWizardRepository->HasData();
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
      m_backupDataUserWizardRepositry->WriteUserRepositoryToFile(m_defaultSettingsValues.at(0), true);
   } else {
      if (m_actionWithUserRepository == userWizardPageComboBoxBackupAndOldDataActions.at(1)) {
         m_backupDataUserWizardRepositry->WriteUserRepositoryToFile(m_defaultSettingsValues.at(0), false);
      } else {
         m_oldDataCurrentUserWizardRepositry->WriteUserRepositoryToFile(m_validSettingsPaths.at(0), false);
      }
   }
}

void WizardService::ApplySettingsWithRolesRepository()
{
   for (int i = 0; i < 4; i++) {
      if (m_actionWithRolesRepository.at(0) == m_rolesWizardPageComboBoxBackupAndOldDataActions.at(0)) {
         RolesAndStartupsWizardRepository *rep = m_backupDataRolesAndStartupsWizardRepository;
      } else {
         if (m_actionWithRolesRepository.at(0) == m_rolesWizardPageComboBoxBackupAndOldDataActions.at(1)) {
            m_backupDataRolesAndStartupsWizardRepository->SaveRoleDesktops(m_defaultSettingsValues.at(1), i);
            m_backupDataRolesAndStartupsWizardRepository->SaveRoleStartups(m_defaultSettingsValues.at(2), i);
         }
      }
   }
}

void WizardService::GetDataFromUserRepository(const bool isOldData, QString &FCS, QString &rank, QVector<User> &userList)
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

int WizardService::GetUserCountFromUserRepository(const bool isOldData) const
{
   if (isOldData) {
      return m_oldDataCurrentUserWizardRepositry->GetUserCount();
   } else {
      return m_backupDataUserWizardRepositry->GetUserCount();
   }
}

void WizardService::GetDataFromDesktopRepository(const int roleIndex, const bool isOldData, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs)
{
   RolesAndStartupsWizardRepository *currentRepository;

   if (isOldData) {
      currentRepository = m_oldDataRolesAndStartupsWizardRepository;
   } else {
      currentRepository = m_backupDataRolesAndStartupsWizardRepository;
   }

   if (currentRepository->HasData()) {
      currentRepository->RetunRoleDesktopsAndStartups(roleIndex, roleDesktops, roleExecs);
   }
}

int WizardService::GetUserCountFromDesktopRepository(const int roleIndex, const bool isOldData)
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

void WizardService::SetActionWithRoleRepository(const int roleIndex, const QString &actionWithRoleRepository)
{
   m_actionWithRolesRepository[roleIndex] = actionWithRoleRepository;
}

const QString &WizardService::GetActionWithRoleRepository(const int roleIndex)
{
   return m_actionWithRolesRepository.at(roleIndex);
}

void WizardService::ApplyWizard()
{
   ApplySettingsWithUserRepository();
   ApplySettingsWithRolesRepository();
}
