#ifndef WIZARDSERVICE_H
#define WIZARDSERVICE_H
#include <QStringList>
#include <QDomDocument>
#include <QDebug>

#include "Services/Terminals/terminal.h"
#include "Structs/programstruct.h"
#include "Structs/userstruct.h"

class RolesAndStartupsWizardRepository
{
public:
   RolesAndStartupsWizardRepository(Terminal *terminal);

   ~RolesAndStartupsWizardRepository();

   bool HasData();

   void RetunRoleDesktopsAndStartups(int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &startups);

   void SetRoleDesktopsAndStartupsFromFile(QString &pathToDesktopsFolder, QString &pathToStartupsFolder);

   void SetRoleDesktopsAndStartupsFromBackup(int roleIndex, QDomElement &backupNode);

   int GetRoleDesktopsAppCount(int roleIndex);

   void SaveRoleDesktops(const QString &pathToDesktopsFolder, int roleIndex);

   void SaveRoleStartups(const QString &pathToStarupsFolder, int roleIndex);

private:

   void SetRoleDesktopsFromFile(QString &pathToDesktopsFolder);

   void SetRoleStartupsFromFile(QString &pathToStartupsFolder);

   void SetRoleDesktopFromXml(int roleIndex, QDomElement &desktops);

   void SetRoleStartupsFromXml(int roleIndex, QDomElement &startups);

   void AppendRoleStartups(int roleIndex, QString startup);

   void AppendEnittyToRoleDesktops(int roleNum, DesktopEntity &desktopEntity);

private:

   QList<DesktopEntity> &GetDesktopsByIndex(int roleIndex);

   QStringList &GetStatupsByIndex(int roleIndex);

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

#endif // WIZARDSERVICE_H
