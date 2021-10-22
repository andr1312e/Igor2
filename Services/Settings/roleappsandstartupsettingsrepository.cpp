#include "roleappsandstartupsettingsrepository.h"

RolesAndStartupsWizardRepository::RolesAndStartupsWizardRepository(Terminal *terminal)
   : m_terminal(terminal)
   , m_firstRoleDesktopsIcons(new QList<DesktopEntity>())
   , m_secondRoleDesktopsIcons(new QList<DesktopEntity>())
   , m_thirdRoleDesktopsIcons(new QList<DesktopEntity>())
   , m_fourthRoleDesktopsIcons(new QList<DesktopEntity>())
{

}

RolesAndStartupsWizardRepository::~RolesAndStartupsWizardRepository()
{
   delete m_firstRoleDesktopsIcons;
   delete m_secondRoleDesktopsIcons;
   delete m_thirdRoleDesktopsIcons;
   delete m_fourthRoleDesktopsIcons;
}

bool RolesAndStartupsWizardRepository::hasData()
{
   return m_hasData;
}

void RolesAndStartupsWizardRepository::RetunRoleDesktopsAndStartups(int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups)
{
   switch (roleIndex) {
      case 0: {
         roleDesktops = *m_firstRoleDesktopsIcons;
         startups = m_firstRoleStartup;
         break;
      }

      case 1: {
         roleDesktops = *m_secondRoleDesktopsIcons;
         startups = m_secondRoleStartup;
         break;
      }

      case 2: {
         roleDesktops = *m_thirdRoleDesktopsIcons;
         startups = m_thirdRoleStartup;
         break;
      }

      case 3: {
         roleDesktops = *m_fourthRoleDesktopsIcons;
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

void RolesAndStartupsWizardRepository::SetRoleDesktopsAndStartupsFromBackup(int roleIndex, QDomElement &backupNode)
{
   QDomElement desktops = backupNode.firstChildElement();
   qDebug() << desktops.tagName();
   QDomElement startups = backupNode.lastChildElement();
   qDebug() << startups.tagName();
   SetRoleDesktopFromXml(roleIndex, desktops);
   SetRoleStartupsFromXml(roleIndex, startups);
}

int RolesAndStartupsWizardRepository::GetRoleDesktopsAppCount(int roleIndex)
{
   switch (roleIndex) {
      case 0: {
         return m_firstRoleDesktopsIcons->count();
      }

      case 1: {
         return m_secondRoleDesktopsIcons->count();
      }

      case 2: {
         return m_thirdRoleDesktopsIcons->count();
      }

      case 3: {
         return m_fourthRoleDesktopsIcons->count();
      }

      default: {
         qFatal("Роли только 4");
      }
   }
}

void RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile(QString &pathToDesktops)
{
   QString currentStartupFileFullPath;

   for (int i = 0; i < Roles.count(); i++) {
      QString currentStartupFileFullPath = pathToDesktops + Roles.at(i) + "/";
      QDir dir(currentStartupFileFullPath);

      if (dir.exists()) {
         QStringList list = m_terminal->getFileList(currentStartupFileFullPath, "RolesAndStartupsWizardRepository::SetRoleDesktopsFromFile", true);

         for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
            DesktopEntity desktopEntity;
            QString entityInfo = m_terminal->getFileText(currentStartupFileFullPath + *it, "FileExplorer::updateIconsList");
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
}

void RolesAndStartupsWizardRepository::SetRoleStartupsFromFile(QString &pathToStartupsFolder)
{
   QFile file;
   QString currentStartupFileFullPath;

   for (int i = 0; i < Roles.count(); i++) {
      currentStartupFileFullPath = pathToStartupsFolder + Roles.at(i) + ".start";
      file.setFileName(currentStartupFileFullPath);

      if (file.open(QIODevice::ReadOnly)) {
         while (!file.atEnd()) {
            AppendRoleStartups(i, file.readLine());
         }

         file.close();
      }
   }
}

void RolesAndStartupsWizardRepository::SetRoleDesktopFromXml(int roleIndex, QDomElement &desktops)
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

void RolesAndStartupsWizardRepository::SetRoleStartupsFromXml(int roleIndex, QDomElement &startups)
{
   if (startups.tagName() == "startups") {
      QDomNodeList startupsList = startups.childNodes();

      for (int i = 0; i < startupsList.count(); i++) {
         QDomElement startup = startupsList.at(i).toElement();

         if (startup.tagName() != "startupApp") {
            break;
         } else {
            AppendRoleStartups(roleIndex, startup.attribute("exec"));
         }
      }

      m_hasData = true;
   }
}

void RolesAndStartupsWizardRepository::AppendEnittyToRoleDesktops(int roleIndex, DesktopEntity &desktopEntity)
{
   switch (roleIndex) {
      case 0: {
         m_firstRoleDesktopsIcons->append(desktopEntity);
         break;
      }

      case 1: {
         m_secondRoleDesktopsIcons->append(desktopEntity);
         break;
      }

      case 2: {
         m_thirdRoleDesktopsIcons->append(desktopEntity);
         break;
      }

      case 3: {
         m_fourthRoleDesktopsIcons->append(desktopEntity);
         break;
      }

      default: {
         qFatal("Роли только 4");
      }
   }
}

void RolesAndStartupsWizardRepository::AppendRoleStartups(int roleIndex, QString startup)
{
   switch (roleIndex) {
      case 0: {
         m_firstRoleStartup.append(startup);
         break;
      }

      case 1: {
         m_secondRoleStartup.append(startup);
         break;
      }

      case 2: {
         m_thirdRoleStartup.append(startup);
         break;
      }

      case 3: {
         m_fourthRoleStartup.append(startup);
         break;
      }

      default: {
         qFatal("Роли только 4");
      }
   }
}
