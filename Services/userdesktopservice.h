#ifndef SERVICES_FILEEXPLORER_H
#define SERVICES_FILEEXPLORER_H

#include <QDir>
#include <QList>
#include <QProcess>
#include <QString>
#include <QStandardItemModel>

#include "Services/idesktopservice.h"

#include "Structs/programstruct.h"

#include "Services/Terminals/terminal.h"
#include "Services/Sql/isqlservice.h"

class UserDesktopService : public DesktopService
{
public:
    UserDesktopService(Terminal *terminal, ISqlDatabaseService *sqlDatabaseService);
    ~UserDesktopService();

public:
    void GetAllUserDesktops(const QString &userName);
    void AddIconToUser(const QString &userName, const DesktopEntity &entity);
    void DeleteIconToUser(const QString &userName, const QString &iconName);
    void DeleteAllIconsToUser(const quint8 &roleId, const QString &userName);
private:
    void UpdateIconListDataAndModelFromUserDesktop(const QString &userDesktopPath);
    void UpdateIconsListFromUserDesktop(const QString &userDesktopPath);
    QStringList GetAllDesktopEntities(const QString &userDesktopPath);
    void ParseAndAppendIconInfoToList(const QString &programName,
                              const QString &iconInfo);
    void ParseAndAppendFileInfoToList(const QString &fileName);
    bool IsIcon(const QString &entityName) const;

};

#endif // SERVICES_FILEEXPLORER_H
