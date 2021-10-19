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

    bool hasData();

    void RetunRoleDesktopsAndStartups(int roleNum, QList<DesktopEntity> &roleDesktops, QStringList &startups);

    void SetRoleDesktopsAndStartupsFromFile(QString &pathToDesktopsFolder, QString &pathToStartupsFolder);

    void SetRoleDesktopsAndStartupsFromBackup(int roleNum, QDomElement &backupNode);

private:

    void SetRoleDesktopsFromFile(QString &pathToDesktopsFolder);

    void SetRoleStartupsFromFile(QString &pathToStartupsFolder);

    void SetRoleDesktopFromXml(int roleNum, QDomElement &desktops);

    void SetRoleStartupsFromXml(int roleNum, QDomElement &startups);

    void AppendRoleStartups(int roleNum, QString startup);

    void AppendEnittyToRoleDesktops(int roleNum, DesktopEntity &desktopEntity);

private:

    bool m_hasData;

    Terminal *m_terminal;

    QStringList *m_firstRoleStartup;
    QStringList *m_secondRoleStartup;
    QStringList *m_thirdRoleStartup;
    QStringList *m_fourthRoleStartup;

    QList<DesktopEntity> *m_firstRoleDesktopsIcons;
    QList<DesktopEntity> *m_secondRoleDesktopsIcons;
    QList<DesktopEntity> *m_thirdRoleDesktopsIcons;
    QList<DesktopEntity> *m_fourthRoleDesktopsIcons;
};

#endif // WIZARDSERVICE_H
