#ifndef SERVICES_FILEEXPLORER_H
#define SERVICES_FILEEXPLORER_H

#include <QDir>
#include <QList>
#include <QProcess>
#include <QStandardItemModel>
#include <QString>

#include "Structs/programstruct.h"

#include "Services/Terminals/terminal.h"
#include "Services/isqlservice.h"

class DesktopPanelPresenter
{
public:
    DesktopPanelPresenter(Terminal *terminal, ISqlDatabaseService *sqlDatabaseService);
    ~DesktopPanelPresenter();
    QStandardItemModel *GetModel();
    Terminal *GetTerminal();
public:
    void CheckRoleDesktopTable(const quint8 &roleId);
    void GetAllRoleDesktops(const quint8 &roleId);
    void SetDefaultIconsToUser(const quint8 &oldRoleId, const quint8& newRoleId, const QString &userName);
    void AddIconToRole(const DesktopEntity &entity, const quint8 &roleId);
    void DeleteIconToRole(const QString iconName, const quint8 &roleId);
public:
    void GetAllUserDesktops(const QString &userName);
    void AddIconToUser(const DesktopEntity &entity, const QString &userName);
    void DeleteIconToUser(const QString &userName, const QString &iconName);
    void DeleteRoleIconFromUser(const QString &userName, const quint8 &roleId);
private:
    void WriteIconPropertiesToFile(const QString &iconProperties,
                                   const QString &iconName, const QString &userName);
private:
    void CheckUserPath(const QString &pathToDesktop);
    void UpdateIconListDataAndModel(const QString &userName);
    void UpdateIconsList(const QString &userName);
    void UpdateModel(const QList<DesktopEntity> &listOfDesktopEntities);
    QString GetFullIconPath(const QString &iconName);
private:
    const QString GetUserDesktopPath(const QString &userName) const;
    QStringList GetAllDesktopEntities(const QString &userName);
    void appendIconInfoToList(const QString &programName,
                              const QString &iconInfo);
    void appendFileInfoToList(const QString &fileName);
    bool IsIcon(const QString &entityName) const;
    void CreateIconFile(const QString userName, const QString &iconName);
    const QString CreateIconProperties(const DesktopEntity &entity) const;

    void CheckFolderToFileAndFileExists(const QString &iconName);

private:
    Terminal *m_terminal;
    ISqlDatabaseService *m_sqlDatabaseService;

    QList<DesktopEntity> *m_filesList;
    QStandardItemModel *m_fileModel;

    const QString m_destinationFolder="/usr/RLS_TI/";

    QString m_path;
private:
    const QString m_desktopName = "/Desktop/";
    const QString m_homeName = "/home/";
    const QString m_iconType = ".desktop";
    const QString m_astraIconPath="/usr/share/icons/fly-astra-flat/192x192/";
    const QString m_defaulIconPath="apps/utilities-terminal.png";
    const QString m_fileIconPath="/usr/share/icons/hicolor/128x128/apps/ark.png";
};

#endif // SERVICES_FILEEXPLORER_H
