#include "terminal.h"

Terminal::Terminal()
   : m_process(new QProcess())
   , m_consoleCommand(new QStringList())
{
   m_consoleCommand->append("-c");
   m_consoleCommand->append("");
}

Terminal::~Terminal()
{
   delete m_consoleCommand;
   delete m_process;
}

QString Terminal::RunConsoleCommand(const QString &command, const QString calledFunc)
{
   m_consoleCommand->last() = command;
   m_process->start("/bin/sh", *m_consoleCommand);
   m_process->waitForFinished(-1);
   QString processError = m_process->readAllStandardError();
   QString processOutput = m_process->readAllStandardOutput();

   if (processError != "") {
      qInfo() << processError;
      qFatal("Работа невозможна метод: %s выдал ошибку при комманде %s . Cообщение об ошибке %s", calledFunc.toLatin1().constData(), command.toLatin1().constData(), processError.toLatin1().constData());
   }

   m_process->close();
   return processOutput;
}

QStringList Terminal::GetAllUsersList(const QString calledFunc)
{
    const QString getAllUsersTextCommand = CreateGetAllUsersCommand();
    const QString allUsers=RunConsoleCommand(getAllUsersTextCommand, calledFunc);
    QStringList usersList=allUsers.split('\n');
    usersList.removeLast();
    return usersList;
}

bool Terminal::IsDirExists(const QString folderPath, const QString calledFunc, bool hasRoot)
{
    QStringList listofSubFoldersToPath = folderPath.split('/');

    if (listofSubFoldersToPath.isEmpty()) {
       qFatal("Работа невозможна метод: %s выдал пуcтой путь. Путь %s", calledFunc.toLatin1().constData(), folderPath.toLatin1().constData());
    } else {
       listofSubFoldersToPath.removeAll("");
       QString currentPath = "/";

       for (QStringList::iterator it = listofSubFoldersToPath.begin(); it != listofSubFoldersToPath.end(); ++it) {
          if (*it == "Desktop") {
             QStringList files = GetFileList(currentPath, calledFunc, hasRoot);
             QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

             if (!(files.contains((*it) + "@")) && !(folders.contains((*it) + "/"))) {
                return false;
             }
          } else {
             QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

             if (!folders.contains((*it) + "/")) {
                return false;
             }
          }

          currentPath = currentPath + *it + "/";
       }

       return true;
    }
}

bool Terminal::IsDirNotExists(const QString folderPath, const QString calledFunc, bool hasRoot)
{
   QStringList listofSubFoldersToPath = folderPath.split('/');

   if (listofSubFoldersToPath.isEmpty()) {
      qFatal("Работа невозможна метод: %s выдал пуcтой путь. Путь %s", calledFunc.toLatin1().constData(), folderPath.toLatin1().constData());
   } else {
      listofSubFoldersToPath.removeAll("");
      QString currentPath = "/";

      for (QStringList::iterator it = listofSubFoldersToPath.begin(); it != listofSubFoldersToPath.end(); ++it) {
         if (*it == "Desktop") {
            QStringList files = GetFileList(currentPath, calledFunc, hasRoot);
            QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

            if (!(files.contains((*it) + "@")) && !(folders.contains((*it) + "/"))) {
               return true;
            }
         } else {
            QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

            if (!folders.contains((*it) + "/")) {
               return true;
            }
         }

         currentPath = currentPath + *it + "/";
      }

      return false;
   }
}

bool Terminal::IsFileExists(const QString filePath, const QString calledFunc, bool hasRoot)
{
   int backSlashPos = filePath.lastIndexOf('/', filePath.count() - 2);
   QString folderPath = filePath.left(backSlashPos);
   QString fileName = filePath.mid(backSlashPos + 1);
   QStringList files = GetFileList(folderPath, calledFunc, hasRoot);
   while(files.count()==1 && files.first().endsWith('@') && !folderPath.endsWith('/'))
   {
      folderPath.push_back('/');
      files = GetFileList(folderPath, calledFunc, hasRoot);
   }
   if (files.contains(fileName) || files.contains(fileName+"*")) {
      return true;
   } else {
      return false;
   }
}

bool Terminal::IsFileNotExists(const QString filePath, const QString calledFunc, bool hasRoot)
{
   int backSlashPos = filePath.lastIndexOf('/', filePath.count() - 2);
   QString folderPath = filePath.left(backSlashPos);
   QString fileName = filePath.mid(backSlashPos + 1);
   QStringList files = GetFileList(folderPath, calledFunc, hasRoot);

   if (files.contains(fileName)) {
      return false;
   } else {
      return true;
   }
}

void Terminal::CheckAndCreatePathToElement(const QString &path, const QString calledFunc, bool hasRoot)
{
   QStringList listofSubFoldersToPath = FunctionsWithStrings::GetListofSubFoldersFromPath(path, "Terminal::CreatePathToFolderWithFolder");
   QString currentPath = "/";
   listofSubFoldersToPath.removeLast();

   for (QStringList::iterator it = listofSubFoldersToPath.begin(); it != listofSubFoldersToPath.end(); ++it) {
      if (*it == "Desktop") {
         QStringList files = GetFileList(currentPath, calledFunc, hasRoot);
         QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

         if (!(files.contains((*it) + "@")) && !(folders.contains((*it) + "/"))) {
            CreateFolder(currentPath + *it + "/", calledFunc, hasRoot);
         }
      } else {
         QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

         if (!folders.contains((*it) + "/")) {
            CreateFolder(currentPath + *it + "/", calledFunc, hasRoot);
         }
      }

      currentPath = currentPath + *it + "/";
   }
}


void Terminal::CreateFolder(const QString &folderPath, const QString callerFunc, bool hasRoot)
{
   QString createFolderCommand = CreateCreateFolderCommand(folderPath, hasRoot);
   RunConsoleCommand(createFolderCommand, callerFunc);
}

void Terminal::CreateFile(const QString filePath, const QString calledFunc, bool hasRoot)
{
   QString createIconCommand = CreateCreateFileCommand(filePath, hasRoot);
   RunConsoleCommand(createIconCommand, calledFunc);
}

QStringList Terminal::GetFolderList(const QString &folderPath, QString calledFunc, bool hasRoot)
{
   QString getFoldersListCommand = CreateGetFolderListCommand(folderPath, hasRoot);
   QString folders = RunConsoleCommand(getFoldersListCommand, calledFunc);
   return folders.split('\n');
}

QStringList Terminal::GetFileList(const QString &folderPath, QString calledFunc, bool hasRoot)
{
   QString getFileListCommand = CreateGetFileListCommand(folderPath, hasRoot);
   QString files = RunConsoleCommand(getFileListCommand, calledFunc);
   QStringList filesList = files.split('\n');
   filesList.removeLast();
   return filesList;
}

QString Terminal::GetFileText(const QString filePath, const QString calledFunc, bool hasRoot)
{
   QString getFileTextCommand = CreateGetFileTextCommand(filePath, hasRoot);
   return RunConsoleCommand(getFileTextCommand, calledFunc);
}

void Terminal::WriteTextToFileSudo(const QByteArray &text, const QString filePath, const QString calledFunc)
{
   QString writeTextToFileCommand = CreateWriteTextToFileCommandSudo(filePath, text);
   RunConsoleCommand(writeTextToFileCommand, calledFunc);
}

void Terminal::DeleteFileSudo(const QString filePath, const QString calledFunc)
{
   QString deleteFileCommand = CreateDeleteFileCommandSudo(filePath);
   RunConsoleCommand(deleteFileCommand, calledFunc);
}

void Terminal::ClearFolderSudo(const QString &folderPath, const QString calledFunc)
{
   QString deleteAllFilesCommand = CreateClearFolderCommandSudo(folderPath);
   RunConsoleCommand(deleteAllFilesCommand, calledFunc);
}

void Terminal::ClearFileSudo(const QString &filePath, const QString calledFunc)
{
   QString clearFileCommand = CreateClearFileCommandSudo(filePath);
   RunConsoleCommand(clearFileCommand, calledFunc);
}

void Terminal::CopyFileSudo(const QString source, const QString &destination, const QString calledFunc)
{
   QString copyFileCommand = CreateCopyFileCommandSudo(source, destination);
   RunConsoleCommand(copyFileCommand, calledFunc);
}

QStringList Terminal::GetAllProcessList(const QString calledFunc)
{
    QString processListCommand=CreateGettAllProcessListCommand();
    QString processes=RunConsoleCommand(processListCommand, calledFunc);
    QStringList processList=processes.split('\n');
    processList.removeLast();
    for (QString &process: processList)
    {
        int index=process.indexOf('/');
        if(index >0)
        {
            process.truncate(index);
        }
    }
    return processList;
}

const QString Terminal::CreateGetAllUsersCommand() const
{
    return "awk -F: '{print $1 \":\" $3}' /etc/passwd";
}

QString Terminal::CreateCreateFolderCommand(const QString &folderPath, bool hasRoot) const
{
   if (hasRoot) {
      return "sudo mkdir '" + folderPath + "'";
   } else {
      return "mkdir '" + folderPath + "'";
   }
}

QString Terminal::CreateCreateFileCommand(const QString &filePath, bool hasRoot) const
{
   if (hasRoot) {
      return "sudo touch '" + filePath + "'";
   } else {
      return "touch '" + filePath + "'";
   }
}

QString Terminal::CreateGetFolderListCommand(const QString &folderPath, bool hasRoot) const
{
   if (hasRoot) {
      return "sudo ls '" + folderPath + "' -F -1 | grep /$ -";
   } else {
      return "ls '" + folderPath + "' -F -1 | grep /$ -";
   }
}

QString Terminal::CreateGetFileListCommand(const QString &folderPath, bool hasRoot) const
{
   if (hasRoot) {
      return "sudo ls '" + folderPath + "' -F -1 | grep /$ -v";
   } else {
      return "ls '" + folderPath + "' -F -1 | grep /$ -v";
   }
}

QString Terminal::CreateGetFileTextCommand(const QString &filePath, bool hasRoot) const
{
   if (hasRoot) {
      return "sudo cat '" + filePath + "'";
   } else {
      return "cat '" + filePath + "'";
   }
}

QString Terminal::CreateWriteTextToFileCommandSudo(const QString &filePath, const QByteArray &text) const
{
   return "sudo sh -c 'echo \"" + text + "\" >> " + filePath + "'";
}

QString Terminal::CreateDeleteFileCommandSudo(const QString &filePath) const
{
   return "sudo rm '" + filePath + "'";
}

QString Terminal::CreateClearFileCommandSudo(const QString &filePath) const
{
   return "sudo truncate -s 0 '" + filePath + "'";
}

QString Terminal::CreateClearFolderCommandSudo(const QString &folderPath) const
{
   return "sudo rm -rf " + folderPath + "*";
}

QString Terminal::CreateDeleteEmptyFolderCommandSudo(const QString &folderPath) const
{
   return "sudo rmdir -p " + folderPath;
}

QString Terminal::CreateCopyFileCommandSudo(const QString &source, const QString &destination) const
{
    return "sudo cp '" + source + "' '" + destination + "'";
}

QString Terminal::CreateGettAllProcessListCommand() const
{
    return "ps -eo comm";
}
