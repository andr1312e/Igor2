#include "roleappsandstartupsettingsrepository.h"

RolesAndStartupsWizardRepository::RolesAndStartupsWizardRepository(Terminal *terminal)
   : m_terminal(terminal)
{

}

RolesAndStartupsWizardRepository::~RolesAndStartupsWizardRepository()
{

}

bool RolesAndStartupsWizardRepository::HasData()
{
   return m_hasData;
}

void RolesAndStartupsWizardRepository::RetunRoleDesktopsAndStartups(const int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups)
{
   switch (roleIndex) {
      case 0: {
         roleDesktops = m_firstRoleDesktopsIcons;
         startups = m_firstRoleStartup;
         break;
      }

      case 1: {
         roleDesktops = m_secondRoleDesktopsIcons;
         startups = m_secondRoleStartup;
         break;
      }

      case 2: {
         roleDesktops = m_thirdRoleDesktopsIcons;
         startups = m_thirdRoleStartup;
         break;
      }

      case 3: {
         roleDesktops = m_fourthRoleDesktopsIcons;
         startups = m_fourthRoleStartup;
         break;
      }

      default: {
         qFatal("Роли только 4");
      }
   }
}

void RolesAndStartupsWizardRepository::SetRoleDesktopsAndStartupsFromFile(QString &pathToDesktopsFolder, QString &pathToStartupsFolder)
{
   SetRoleDesktopsFromFile(pathToDesktopsFolder);
   SetRoleStartupsFromFile(pathToStartupsFolder);
}

void RolesAndStartupsWizardRepository::SetRoleDesktopsAndStartupsFromBackup(const int &roleIndex, QDomElement &backupNode)
{
   QDomElement desktops = backupNode.firstChildElement();
   QDomElement startups = backupNode.lastChildElement();
   SetRoleDesktopFromXml(roleIndex, desktops);
   SetRoleStartupsFromXml(roleIndex, startups);
}

int RolesAndStartupsWizardRepository::GetRoleDesktopsAppCount(const int roleIndex)
{
   switch (roleIndex) {
      case 0: {
         return m_firstRoleDesktopsIcons.count();
      }

      case 1: {
         return m_secondRoleDesktopsIcons.count();
      }

      case 2: {
         return m_thirdRoleDesktopsIcons.count();
      }

      case 3: {
         return m_fourthRoleDesktopsIcons.count();
      }

      default: {
         qFatal("Роли только 4");
      }
   }
}

void RolesAndStartupsWizardRepository::SaveRoleDesktops(const QString &pathToDesktopsFolder, const int roleIndex)
{
   if (m_terminal->IsDirNotExists(pathToDesktopsFolder, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true)) {
      m_terminal->CheckAndCreatePathToElement(pathToDesktopsFolder, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true);
      m_terminal->CreateFolder(pathToDesktopsFolder, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true);
   }

   QString roleDesktopFolder = pathToDesktopsFolder + Roles.at(roleIndex);

   if (m_terminal->IsDirNotExists(roleDesktopFolder, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true)) {
      m_terminal->CreateFolder(roleDesktopFolder, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true);
   }

   QList<DesktopEntity> currentRoleDesktops(GetDesktopsByIndex(roleIndex));

   for (QList<DesktopEntity>::const_iterator it = currentRoleDesktops.cbegin(); it != currentRoleDesktops.cend(); ++it) {
      QString pathDesktopIcon = roleDesktopFolder + "/" + it->name;

      if (m_terminal->IsFileNotExists(pathDesktopIcon, "RolesAndStartupsWizardRepository::SaveRoleDesktops", true)) {
         m_terminal->CreateFile(pathDesktopIcon, "RolesAndStartupsWizardRepository::SaveRoleDesktops", true);
      } else {
         m_terminal->ClearFileSudo(pathDesktopIcon, "RolesAndStartupsWizardRepository::SaveRoleDesktops");
      }

      const QByteArray iconProperties (CreateIconProperties(it->exec, it->icon, it->name).toUtf8());
      m_terminal->WriteTextToFileSudo(iconProperties, pathDesktopIcon, "RolesAndStartupsWizardRepository::SaveRolesAndStartups");
   }
}

void RolesAndStartupsWizardRepository::SaveRoleStartups(const QString &pathToStarupsFolder, const int roleIndex)
{
   if (m_terminal->IsDirNotExists(pathToStarupsFolder, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true)) {
      m_terminal->CheckAndCreatePathToElement(pathToStarupsFolder, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true);
      m_terminal->CreateFolder(pathToStarupsFolder, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true);
   }

   QString roleStartupFile = pathToStarupsFolder + Roles.at(roleIndex) + ".startup";

   if (m_terminal->IsFileNotExists(roleStartupFile, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true)) {
      m_terminal->CreateFile(roleStartupFile, "RolesAndStartupsWizardRepository::SaveRolesAndStartups", true);
   } else {
      m_terminal->ClearFileSudo(roleStartupFile, "RolesAndStartupsWizardRepository::SaveRolesAndStartups");
   }

   QStringList currentRoleStartups(GetStatupsByIndex(roleIndex));
   QByteArray starups = currentRoleStartups.join("\n").toUtf8();
   m_terminal->WriteTextToFileSudo(starups, roleStartupFile, "RolesAndStartupsWizardRepository::SaveRolesAndStartups");

}

void RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile(QString &pathToDesktops)
{
   QString currentStartupFileFullPath;

   for (int i = 0; i < Roles.count(); i++) {
      QString currentStartupFileFullPath = pathToDesktops + Roles.at(i) + "/";

      if (m_terminal->IsDirNotExists(currentStartupFileFullPath, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true)) {
         m_terminal->CheckAndCreatePathToElement(currentStartupFileFullPath, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true);
         m_terminal->CreateFolder(currentStartupFileFullPath, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true);
      }

      QStringList list = m_terminal->GetFileList(currentStartupFileFullPath, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true);

      for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
         DesktopEntity desktopEntity;
         QString entityInfo = m_terminal->GetFileText(currentStartupFileFullPath + *it, "FileExplorer::updateIconsList");
         QStringList entityInfoList = entityInfo.split('\n');
         entityInfoList.removeLast();

         for (QList<QString>::const_iterator iconInfoLine = entityInfoList.cbegin(); iconInfoLine != entityInfoList.cend(); ++iconInfoLine) {
            QString parametr = *iconInfoLine;

            if (parametr.startsWith("Exec")) {
               int index = parametr.indexOf("=");
               desktopEntity.exec = parametr.mid(index + 1);
            } else {
               if (parametr.startsWith("Icon")) {
                  int index = parametr.indexOf("=");
                  desktopEntity.icon = parametr.mid(index + 1);
               }
            }
         }

         AppendEnittyToRoleDesktops(i, desktopEntity);
      }

   }
}

void RolesAndStartupsWizardRepository::SetRoleStartupsFromFile(QString &pathToStartupsFolder)
{

   if (m_terminal->IsDirNotExists(pathToStartupsFolder, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true)) {
      m_terminal->CheckAndCreatePathToElement(pathToStartupsFolder, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true);
      m_terminal->CreateFolder(pathToStartupsFolder, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true);
   }

   QString currentStartupFilePath;

   for (int i = 0; i < Roles.count(); i++) {
      currentStartupFilePath = pathToStartupsFolder + Roles.at(i) + ".start";

      if (m_terminal->IsFileExists(currentStartupFilePath, "RolesAndStartupsWizardRepository::SetRoleStartupsFromFile", true)) {
         m_terminal->CreateFile(currentStartupFilePath, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true);
      } else {
         QString startupsPaths = m_terminal->GetFileText(currentStartupFilePath, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true);
         QStringList startupsList = startupsPaths.split('\n');
         startupsList.removeLast();
         AppendRoleStartups(i, startupsList);
      }
   }
}

void RolesAndStartupsWizardRepository::SetRoleDesktopFromXml(const int roleIndex, QDomElement &desktops)
{
   if (desktops.tagName() == "desktops") {
      QDomNodeList desktopsList = desktops.childNodes();

      for (int i = 0; i < desktopsList.count(); i++) {
         QDomElement elem = desktopsList.at(i).toElement();

         if (elem.tagName() != "desktopApp") {
            break;
         } else {
            DesktopEntity desktopEntity;
            desktopEntity.exec = elem.attribute("exec");
            desktopEntity.name = elem.attribute("name");
            desktopEntity.icon = elem.attribute("icon");
            desktopEntity.type = elem.attribute("type");
            AppendEnittyToRoleDesktops(roleIndex, desktopEntity);
         }
      }

      m_hasData = true;
   }
}

void RolesAndStartupsWizardRepository::SetRoleStartupsFromXml(const int roleIndex, QDomElement &startups)
{
   QStringList startupList;

   if (startups.tagName() == "startups") {
      QDomNodeList startupsList = startups.childNodes();

      for (int i = 0; i < startupsList.count(); i++) {
         QDomElement startup = startupsList.at(i).toElement();

         if (startup.tagName() != "startupApp") {
            break;
         } else {
            startupList.append(startup.attribute("exec"));
         }
      }

      AppendRoleStartups(roleIndex, startupList);
      m_hasData = true;
   }
}

void RolesAndStartupsWizardRepository::AppendEnittyToRoleDesktops(const int roleIndex, DesktopEntity &desktopEntity)
{
   switch (roleIndex) {
      case 0: {
         m_firstRoleDesktopsIcons.append(desktopEntity);
         break;
      }

      case 1: {
         m_secondRoleDesktopsIcons.append(desktopEntity);
         break;
      }

      case 2: {
         m_thirdRoleDesktopsIcons.append(desktopEntity);
         break;
      }

      case 3: {
         m_fourthRoleDesktopsIcons.append(desktopEntity);
         break;
      }

      default: {
         qFatal("Роли только 4");
      }
   }
}

QList<DesktopEntity> &RolesAndStartupsWizardRepository::GetDesktopsByIndex(const int roleIndex)
{
   switch (roleIndex) {
      case 0: {
         return m_firstRoleDesktopsIcons;
         break;
      }

      case 1: {
         return m_secondRoleDesktopsIcons;
         break;
      }

      case 2: {
         return m_thirdRoleDesktopsIcons;
         break;
      }

      case 3: {
         return m_fourthRoleDesktopsIcons;
         break;
      }

      default: {
         qFatal("Роли только 4");
      }
   }
}

QStringList &RolesAndStartupsWizardRepository::GetStatupsByIndex(const int roleIndex)
{
   switch (roleIndex) {
      case 0: {
         return m_firstRoleStartup;
         break;
      }

      case 1: {
         return m_secondRoleStartup;
         break;
      }

      case 2: {
         return m_thirdRoleStartup;
         break;
      }

      case 3: {
         return m_fourthRoleStartup;
         break;
      }

      default: {
         qFatal("Роли только 4");
      }
   }
}

QString RolesAndStartupsWizardRepository::CreateIconProperties(const QString &exec, const QString &imagePath, const QString &iconName)
{
   QString iconText = "[Desktop Entry]\nType=Application\nExec='" + exec + "'\n";
   iconText += "Name= " + iconName + " \nName[ru]= " + iconName +
               " \nIcon= " + imagePath + " \n";
   return iconText;
}

void RolesAndStartupsWizardRepository::AppendRoleStartups(const int roleIndex, QStringList &startupList)
{
   switch (roleIndex) {
      case 0: {
         m_firstRoleStartup = startupList;
         break;
      }

      case 1: {
         m_secondRoleStartup = startupList;
         break;
      }

      case 2: {
         m_thirdRoleStartup = startupList;
         break;
      }

      case 3: {
         m_fourthRoleStartup = startupList;
         break;
      }

      default: {
         qFatal("Роли только 4");
      }
   }
}
