#ifndef SERVICES_USERDESKTOPSERIVCE_H
#define SERVICES_USERDESKTOPSERIVCE_H

#include <QDir>
#include <QList>
#include <QProcess>
#include <QString>
#include <QStandardItemModel>

#include "Services/idesktopservice.h"

#include "Structs/programstruct.h"
#include "Services/Terminals/terminal.h"


class UserDesktopService : public DesktopService
{
public:
    explicit UserDesktopService(ISqlDatabaseService *sqlDatabaseService);
    ~UserDesktopService();

public:
    void GetAllUserDesktops(const QString &userName);
    void AddIconToUser(const QString &userName, const DesktopEntity &entity);
    void DeleteIconToUser(const QString &userName, const QString &iconName);
    void DeleteAllIconsToUser(int roleId, const QString &userName);
private:
    void UpdateIconListDataAndModelFromUserDesktop(const QString &userDesktopPath);
    QList<DesktopEntity> UpdateIconsListFromUserDesktop(const QString &userDesktopPath);
    QStringList GetAllDesktopEntities(const QString &userDesktopPath);
    DesktopEntity ParseAndAppendIconInfoToList(const QString &programName, const QString &iconInfo) const;
    bool IsIcon(const QString &entityName) const noexcept;
};

#endif // SERVICES_USERDESKTOPSERIVCE_H
