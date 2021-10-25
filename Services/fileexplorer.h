#ifndef SERVICES_FILEEXPLORER_H
#define SERVICES_FILEEXPLORER_H

#include <QList>
#include <QProcess>
#include <QStandardItemModel>
#include <QString>

#include <Structs/programstruct.h>

#include <Services/Terminals/terminal.h>

class FileExplorer
{
public:
   FileExplorer(Terminal *terminal);

   ~FileExplorer();

   void SetPath(QString &path);

   void AddIcon(const QString &exec, const QString &iconPath, const QString &iconName);

   void DeleteIcon(const QString &iconName);

   void SetDefaultIcons(const QString &role);

   void SetDefaultIconsToUser(const QString &role, const QString &userName);

   QStandardItemModel *GetModel();

   Terminal *GetTerminal();

private:
   void updateIconListDataAndModel();

   void updateIconsList();

   void updateModel();

private:
   Terminal *m_terminal;

   QString m_path;

   QList<DesktopEntity> *m_filesList;

   QStandardItemModel *m_model;

   const QString m_desktopName = "/Desktop/";

   const QString m_homeName = "/home/";

   const QString m_iconType = ".desktop";

private:
   QStringList getAllDesktopEntities();

   void appendIconInfoToList(const QString &programName,
                             const QString &iconInfo);

   void appendFileInfoToList(const QString &fileName);

   bool isIcon(const QString &entityName) const;

   void createIconFile(const QString &iconName);

   QString createIconProperties(const QString &exec, const QString &imagePath,
                                const QString &iconName);

   void writeIconPropertiesToFile(const QString &iconProperties,
                                  const QString &iconName);
};

#endif // SERVICES_FILEEXPLORER_H
