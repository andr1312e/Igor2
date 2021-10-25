#include "fileexplorer.h"
#include <QDebug>

FileExplorer::FileExplorer(Terminal *terminal)
   : m_terminal(terminal)
   , m_filesList(new QList<DesktopEntity>())
   , m_model(new QStandardItemModel()) {}

FileExplorer::~FileExplorer()
{
   delete m_filesList;
   delete m_model;
}

void FileExplorer::SetPath(QString &path)
{
   m_path = path;
   updateIconListDataAndModel();
}

void FileExplorer::AddIcon(const QString &exec, const QString &iconPath,
                           const QString &iconName)
{
   QString fileName(iconName);
   fileName.replace(' ', '_');
   createIconFile(fileName);
   const QString iconText = createIconProperties(exec, iconPath, iconName);
   writeIconPropertiesToFile(iconText, fileName);
   updateIconListDataAndModel();
}

void FileExplorer::DeleteIcon(const QString &iconName)
{
   m_terminal->DeleteFileSudo(m_path + iconName, "FileExplorer::deleteIcon");
   updateIconListDataAndModel();
}

void FileExplorer::SetDefaultIcons(const QString &role)
{
   m_terminal->ClearFolderSudo(m_path, "FileExplorer::deleteAllFiles");
   QStringList files = m_terminal->GetFileList("/home/user/RLS_TI/" + role,
                       "FileExplorer::setDefaultIcons");

   for (int i = files.count() - 1; i >= 0; i--) {
      if (files.at(i).contains('/')) {
         files.removeAt(i);
      }
   }

   for (QStringList::const_iterator it = files.cbegin(); it != files.cend();
         ++it) {
      m_terminal->CopyFileSudo("/home/user/RLS_TI/" + role + "/" + *it, m_path,
                               "FileExplorer::setDefaultIcons");
   }

   updateIconListDataAndModel();
}

void FileExplorer::SetDefaultIconsToUser(const QString &role,
      const QString &userName)
{
   QString userDesktopPath = m_homeName + userName + m_desktopName;

   if (m_terminal->IsDirNotExists(userDesktopPath, "FileExplorer::setDefaultIconsToUser")) {
      m_terminal->ClearFolderSudo(userDesktopPath, "FileExplorer::setDefaultIconsToUser");
   }

   QStringList files = m_terminal->GetFileList("/home/user/RLS_TI/" + role,
                       "FileExplorer::setDefaultIcons");
   files.removeAll("");

   for (QStringList::const_iterator it = files.cbegin(); it != files.cend();
         ++it) {
      QString entity = *it;

      if (entity.at(entity.count() - 1) == '@') {
         entity.remove(entity.count() - 1, 1);
      }

      m_terminal->CopyFileSudo("/home/user/RLS_TI/" + role + "/" + entity,
                               QString(m_homeName + userName + m_desktopName),
                               "FileExplorer::setDefaultIcons");
   }

   updateIconListDataAndModel();
}

QStandardItemModel *FileExplorer::GetModel()
{
   return m_model;
}

Terminal *FileExplorer::GetTerminal()
{
   return m_terminal;
}

void FileExplorer::updateIconListDataAndModel()
{
   updateIconsList();
   updateModel();
}

void FileExplorer::updateIconsList()
{
   QStringList allUserDesktopIconsAndFiles = getAllDesktopEntities();
   m_filesList->clear();

   for (QList<QString>::const_iterator entityName =
            allUserDesktopIconsAndFiles.cbegin();
         entityName != allUserDesktopIconsAndFiles.cend(); ++entityName) {
      QString entity = *entityName;

      if (entity.at(entity.count() - 1) == '@' ||
            entity.at(entity.count() - 1) == '*') {
         entity.remove(entity.count() - 1, 1);
      }

      if (isIcon(entity)) {
         QString entityInfo = m_terminal->GetFileText(
                                 m_path + entity, "FileExplorer::updateIconsList");
         appendIconInfoToList(entity, entityInfo);
      } else {
         appendFileInfoToList(entity);
      }
   }
}

void FileExplorer::updateModel()
{
   m_model->clear();

   for (QList<DesktopEntity>::iterator it = m_filesList->begin();
         it != m_filesList->end(); ++it) {
      QStandardItem *item = new QStandardItem();
      DesktopEntity icon = *it;
      item->setData(QVariant::fromValue(icon), Qt::UserRole + 1);
      m_model->appendRow(item);
   }
}

QStringList FileExplorer::getAllDesktopEntities()
{
   m_terminal->IsDirNotExists(m_path,
                              "FileExplorer::getAllDesktopEntities");
   QStringList allEntites = m_terminal->GetFileList(
                               m_path, "FilesAndIconManager::getAllDesktopEntities-1");
   allEntites.removeAll("");
   return allEntites;
}

void FileExplorer::appendIconInfoToList(const QString &programName,
                                        const QString &iconInfo)
{
   DesktopEntity program;
   program.name = programName;
   program.type = "Приложение";
   QStringList iconInfoList = iconInfo.split('\n');
   iconInfoList.removeLast();

   for (QList<QString>::const_iterator iconInfoLine = iconInfoList.cbegin();
         iconInfoLine != iconInfoList.cend(); ++iconInfoLine) {
      QString parametr = *iconInfoLine;

      if (parametr.startsWith("Exec")) {
         int index = parametr.indexOf("=");
         program.exec = parametr.mid(index + 1);
      } else {
         if (parametr.startsWith("Icon")) {
            int index = parametr.indexOf("=");
            program.icon = parametr.mid(index + 1);
         }
      }
   }

   m_filesList->append(program);
}

void FileExplorer::appendFileInfoToList(const QString &fileName)
{
   DesktopEntity file;
   file.name = fileName;
   file.icon = "";
   file.exec = m_path + "/" + fileName;
   file.type = "Файл";
   m_filesList->append(file);
}

bool FileExplorer::isIcon(const QString &entityName) const
{
   if (entityName.endsWith(".desktop")) {
      return true;
   } else {
      return false;
   }
}

void FileExplorer::createIconFile(const QString &iconName)
{
   QString pathToDesktopIcon = m_path + iconName + ".desktop";

   if (m_terminal->IsFileNotExists(pathToDesktopIcon, "FilesAndIconManager::createIconFile")) {
      m_terminal->CreateFile(pathToDesktopIcon, "FilesAndIconManager::createIconFile", true);
   } else {
      m_terminal->ClearFileSudo(pathToDesktopIcon, "FilesAndIconManager::createIconFile");
   }
}

QString FileExplorer::createIconProperties(const QString &exec,
      const QString &imagePath,
      const QString &iconName)
{
   QString iconText = "[Desktop Entry]\nType=Application\nExec='" + exec + "'\n";
   iconText += "Name= " + iconName + " \nName[ru]= " + iconName +
               " \nIcon= " + imagePath + " \n";
   return iconText;
}

void FileExplorer::writeIconPropertiesToFile(const QString &iconProperties,
      const QString &iconName)
{
   m_terminal->WriteTextToFileSudo(iconProperties.toLatin1(),
                                   m_path + iconName + m_iconType,
                                   "FileExplorer::writeIconPropertiesToFile");
}
