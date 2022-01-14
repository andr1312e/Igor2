#ifndef IDESKTOPSERVICE_H
#define IDESKTOPSERVICE_H
class QStandardItemModel;

#include <QString>

#include "Services/Terminals/terminal.h"
#include "Services/Sql/isqlservice.h"

class DesktopService {
protected:
    DesktopService(ISqlDatabaseService *sqlDatabaseService);
    ~DesktopService();

public:
    QStandardItemModel *GetModel();
    Terminal *GetTerminal();

protected:
    DesktopEntity MoveFilesToProgramFolder(const DesktopEntity &entity);

    void CheckPath(const QString &pathToDesktop);
    void CreateIconWithData(const QString &userDesktopPath, const DesktopEntity &entity);
    void DeleteIcon(const QString &pathToDesktop, const QString &desktopName);
private:
    QString CopyExecFile(const QString &pathToDesktopExec);
    QString CopyIconFile(const QString &pathToDesktopIcon);

    void CreateIcon(const QString &userDesktopPath, const QString &iconName);
    const QString CreateIconData(const DesktopEntity &entity) const;
    void WriteIconDataToFile(const QString &userDesktopPath,
                             const QString &iconName, const QString &userName);
protected:
    const QString GetUserDesktopPath(const QString &userName) const;

    QString GetFullAstraDefaultIconImagePath(const QString &iconName);
    void UpdateModel(const QList<DesktopEntity> &listOfDesktopEntities);
protected:
    Terminal *const m_terminal;
    ISqlDatabaseService *const m_sqlDatabaseService;

    QList<DesktopEntity> m_filesList;
    QStandardItemModel *const m_fileModel;

    const QString m_destinationFolder="/usr/RLS_TI/";

    QString m_path;

    const QString m_desktopName = "/Desktop/";
    const QString m_homeFolderPathName = "/home/";
    const QString m_iconType = ".desktop";
    const QString m_astraIconPath="/usr/share/icons/fly-astra-flat/192x192/";
    const QString m_defaulIconPath="apps/utilities-terminal.png";
    const QString m_fileIconPath="/usr/share/icons/hicolor/128x128/apps/ark.png";
};

#endif // IDESKTOPSERVICE_H
