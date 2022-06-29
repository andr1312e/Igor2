#ifndef SERVICES_DESKTOPSERVICE_H
#define SERVICES_DESKTOPSERVICE_H

#include <QDir>
#include <QStandardItemModel>
#include <QString>

#include "Services/Terminals/terminal.h"
#include "Services/Sql/isqlservice.h"

class DesktopService
{
protected:
    explicit DesktopService(ISqlDatabaseService *sqlDatabaseService);
    ~DesktopService();

public:
    QStandardItemModel *GetModel() const;

protected:
    DesktopEntity MoveFilesToProgramFolder(const DesktopEntity &entity);

    void CheckPath(const QString &pathToDesktop);
    void CreateIconWithData(const QString &userDesktopPath, const DesktopEntity &entity);
    void DeleteIcon(const QString &pathToDesktop, const QString &desktopName);
private:
    QString CopyExecFile(const QString &pathToDesktopExec);
    QString CopyIconFile(const QString &pathToDesktopIcon);

    void CreateEntity(const QString &userDesktopPath, const QString &iconName);
    const QString CreateIconData(const DesktopEntity &entity) const;
    void WriteIconDataToFile(const QString &userDesktopPath,
                             const QString &iconName, const QString &userName);
protected:
    const QString GetUserDesktopPath(const QString &userName) const;

    QString GetFullAstraDefaultIconImagePath(QStringView iconName) const;
    void FillModel(QList<DesktopEntity> listOfDesktopEntities);
protected:
    Terminal *const m_terminal;
    ISqlDatabaseService *const m_sqlDatabaseService;

    QStandardItemModel *const m_fileModel;

    const QString m_destinationFolder;
    const QString m_desktopName;
    const QString m_homeFolderPathName;
    const QString m_iconType;
    const QString m_astraIconPath;
    const QString m_defaulIconPath = "apps/utilities-terminal.png";
    const QString m_fileIconPath = "/usr/share/icons/hicolor/128x128/apps/ark.png";
};

#endif // SERVICES_DESKTOPSERVICE_H
