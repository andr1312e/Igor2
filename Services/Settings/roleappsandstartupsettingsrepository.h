#ifndef SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
#define SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
#include <QStringList>
#include <QDomDocument>

#include "Services/Terminals/terminal.h"
#include "Structs/programstruct.h"
#include "Structs/userstruct.h"

class RolesAndStartupsWizardRepository
{
public:

   explicit RolesAndStartupsWizardRepository(Terminal *terminal);

   ~RolesAndStartupsWizardRepository();

   bool HasData();

   void RetunRoleDesktopsAndStartups(const int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups);

   void SetRoleDesktopsAndStartupsFromFile(QString &pathToDesktopsFolder, QString &pathToStartupsFolder);

   void SetRoleDesktopsAndStartupsFromBackup(const int &roleIndex, QDomElement &backupNode);

   int GetRoleDesktopsAppCount(const int roleIndex);

   void SaveRoleDesktops(const QString &pathToDesktopsFolder, const int roleIndex);

   void SaveRoleStartups(const QString &pathToStarupsFolder, const int roleIndex);

private:

   void SetRoleDesktopsFromFile(QString &pathToDesktopsFolder);

   void SetRoleStartupsFromFile(QString &pathToStartupsFolder);

   void SetRoleDesktopFromXml(const int roleIndex, QDomElement &desktops);

   void SetRoleStartupsFromXml(const int roleIndex, QDomElement &startups);

   void AppendRoleStartups(const int roleIndex, QStringList &startupList);

   void AppendEnittyToRoleDesktops(const int roleIndex, DesktopEntity &desktopEntity);

private:

   QList<DesktopEntity> &GetDesktopsByIndex(const int roleIndex);

   QStringList &GetStatupsByIndex(const int roleIndex);

   QString CreateIconProperties(const QString &exec, const QString &imagePath, const QString &iconName);

private:

   bool m_hasData;

   Terminal *m_terminal;

   QStringList m_firstRoleStartup;
   QStringList m_secondRoleStartup;
   QStringList m_thirdRoleStartup;
   QStringList m_fourthRoleStartup;

   QList<DesktopEntity> m_firstRoleDesktopsIcons;
   QList<DesktopEntity> m_secondRoleDesktopsIcons;
   QList<DesktopEntity> m_thirdRoleDesktopsIcons;
   QList<DesktopEntity> m_fourthRoleDesktopsIcons;
};

#endif // SERVICES_SETTINGS_ROLESANDSTARTUPSWIZARDREPOSITORY_H
