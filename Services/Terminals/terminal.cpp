#include "terminal.h"

Terminal* Terminal::m_terminal = Q_NULLPTR;

Terminal *Terminal::GetTerminal()
{
    if(m_terminal==Q_NULLPTR)
    {
        m_terminal=new Terminal();
    }
    return m_terminal;
}

Terminal::Terminal()
   : m_process(new QProcess(Q_NULLPTR))
   , m_terminalCommand(QStringLiteral("/bin/sh"))
{
   m_consoleCommand.append(QStringLiteral("-c"));
   m_consoleCommand.append(QString());
}

Terminal::~Terminal()
{
   delete m_process;
}

QString Terminal::RunConsoleCommandSync(const QString &command, const char *calledFunc)
{
   m_consoleCommand.last() = command;
   m_process->start(m_terminalCommand, m_consoleCommand);
   m_process->waitForFinished(-1);
   const QString processError(m_process->readAllStandardError());
   const QString processOutput(m_process->readAllStandardOutput());
   if (processError.isEmpty())
   {
       m_process->close();
       return processOutput;
   }
   else
   {
      const QString errorString(QString("Работа невозможна метод: %1 выдал ошибку при комманде %2 . Cообщение об ошибке %3").arg(calledFunc).arg(command).arg(processError));
      qFatal("%s", errorString.toUtf8().constData());
   }
}

void Terminal::RunConsoleCommandWithWarnings(const QString &command, const char *calledFunc)
{
    m_consoleCommand.last() = command;
    m_process->start(m_terminalCommand, m_consoleCommand);
    m_process->waitForFinished(-1);
}

void Terminal::RunConsoleCommandSync(const QString &command, QString &outputInfo, QString &errorInfo, const char *calledFunc)
{
    m_consoleCommand.last() = command;
    m_process->start(m_terminalCommand, m_consoleCommand);
    m_process->waitForFinished(-1);
    outputInfo = m_process->readAllStandardOutput();
    errorInfo = m_process->readAllStandardError();
    m_process->close();
}

QStringList Terminal::GetAllUsersList(const char* calledFunc)
{
    const QString getAllUsersTextCommand = CreateGetAllUsersCommand();
    const QString allUsers=RunConsoleCommandSync(getAllUsersTextCommand, calledFunc);
    QStringList usersList=allUsers.split('\n');
    usersList.removeLast();
    return usersList;
}

bool Terminal::IsDirExists(const QString folderPath, const char* calledFunc, bool hasRoot)
{
    QStringList listofSubFoldersToPath = folderPath.split('/');

    listofSubFoldersToPath.removeAll(QString());
    if (listofSubFoldersToPath.isEmpty()) {
       qFatal("Работа невозможна метод: %s выдал пуcтой путь. Путь %s", calledFunc, folderPath.toUtf8().constData());
    } else {
       QString currentPath('/');

       for (const QString &pathPart: qAsConst(listofSubFoldersToPath)) {
          if (QStringLiteral("Desktop")==pathPart) {
             const QStringList files = GetFileList(currentPath, calledFunc, hasRoot);
             const QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

             if (!(files.contains((pathPart) + '@')) && !(folders.contains((pathPart) + '/'))) {
                return false;
             }
          } else {
             const QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

             if (!folders.contains((pathPart) + '/')) {
                return false;
             }
          }

          currentPath = currentPath + pathPart + '/';
       }

       return true;
    }
}

bool Terminal::IsDirNotExists(const QString folderPath, const char* calledFunc, bool hasRoot)
{
   QStringList listofSubFoldersToPath = folderPath.split('/');

   if (listofSubFoldersToPath.isEmpty()) {
      qFatal("Работа невозможна метод: %s выдал пуcтой путь. Путь %s", calledFunc, folderPath.toUtf8().constData());
   } else {
      listofSubFoldersToPath.removeAll(QString());
      QString currentPath('/');

      for (const QString &subFolderPath:qAsConst(listofSubFoldersToPath)) {
         if (QStringLiteral("Desktop")==subFolderPath) {
            const QStringList files = GetFileList(currentPath, calledFunc, hasRoot);
            const QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

            if (!(files.contains((subFolderPath) + '@')) && !(folders.contains((subFolderPath) + '/'))) {
               return true;
            }
         } else {
            const QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

            if (!folders.contains((subFolderPath) + '/')) {
               return true;
            }
         }

         currentPath = currentPath + subFolderPath + '/';
      }

      return false;
   }
}

bool Terminal::IsFileExists(const QString filePath, const char* calledFunc, bool hasRoot)
{
   const int backSlashPos = filePath.lastIndexOf('/', filePath.count() - 2);
   QString folderPath = filePath.left(backSlashPos);
   const QString fileName = filePath.mid(backSlashPos + 1);
   QStringList files = GetFileList(folderPath, calledFunc, hasRoot);
   while(files.count()==1 && files.first().endsWith('@') && !folderPath.endsWith('/'))
   {
      folderPath.push_back('/');
      files = GetFileList(folderPath, calledFunc, hasRoot);
   }
   if (files.contains(fileName) || files.contains(fileName+'*')) {
      return true;
   } else {
      return false;
   }
}

bool Terminal::IsFileNotExists(const QString filePath, const char* calledFunc, bool hasRoot)
{
   const int backSlashPos = filePath.lastIndexOf('/', filePath.count() - 2);
   const QString folderPath = filePath.left(backSlashPos);
   const QString fileName = filePath.mid(backSlashPos + 1);
   const QStringList files = GetFileList(folderPath, calledFunc, hasRoot);

   if (files.contains(fileName)) {
      return false;
   } else {
      return true;
   }
}

void Terminal::CheckAndCreatePathToElement(const QString &path, const char* calledFunc, bool hasRoot)
{
   QStringList listofSubFoldersToPath = FunctionsWithStrings::GetListofSubFoldersFromPath(path, Q_FUNC_INFO);
   QString currentPath('/');
   listofSubFoldersToPath.removeLast();

   for (const QString pathPart:qAsConst(listofSubFoldersToPath)) {
      if (QStringLiteral("Desktop") == pathPart) {
         const QStringList files = GetFileList(currentPath, calledFunc, hasRoot);
         const QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

         if (!(files.contains((pathPart) + '@')) && !(folders.contains((pathPart) + '/'))) {
            CreateFolder(currentPath + pathPart + '/', calledFunc, hasRoot);
         }
      } else {
         const QStringList folders = GetFolderList(currentPath, calledFunc, hasRoot);

         if (!folders.contains((pathPart) + '/')) {
            CreateFolder(currentPath + pathPart + '/', calledFunc, hasRoot);
         }
      }

      currentPath = currentPath + pathPart + '/';
   }
}


void Terminal::CreateFolder(const QString &folderPath, const char* callerFunc, bool hasRoot)
{
   const QString createFolderCommand = CreateCreateFolderCommand(folderPath, hasRoot);
   RunConsoleCommandSync(createFolderCommand, callerFunc);
}

void Terminal::CreateFile(const QString filePath, const char* calledFunc, bool hasRoot)
{
   const QString createIconCommand = CreateCreateFileCommand(filePath, hasRoot);
   RunConsoleCommandSync(createIconCommand, calledFunc);
}

QStringList Terminal::GetFolderList(const QString &folderPath, const char* calledFunc, bool hasRoot)
{
   const QString getFoldersListCommand = CreateGetFolderListCommand(folderPath, hasRoot);
   const QString folders = RunConsoleCommandSync(getFoldersListCommand, calledFunc);
   return folders.split('\n');
}

QStringList Terminal::GetFileList(const QString &folderPath, const char* calledFunc, bool hasRoot)
{
   const QString getFileListCommand = CreateGetFileListCommand(folderPath, hasRoot);
   const QString files = RunConsoleCommandSync(getFileListCommand, calledFunc);
   QStringList filesList = files.split('\n');
   filesList.removeLast();
   return filesList;
}

QString Terminal::GetFileText(const QString filePath, const char *calledFunc, bool hasRoot)
{
   const  QString getFileTextCommand = CreateGetFileTextCommand(filePath, hasRoot);
   return RunConsoleCommandSync(getFileTextCommand, calledFunc);
}

void Terminal::AppendTextToFileSudo(const QString &text, const QString &filePath, const char *calledFunc)
{
    const QString appendTextToFileCommand=CreateAppendTextToFileCommandSudo(text, filePath);
    RunConsoleCommandSync(appendTextToFileCommand, calledFunc);
}

void Terminal::WriteTextToFileSudo(const QString &text, const QString &filePath, const char* calledFunc)
{
   const QString writeTextToFileCommand = CreateWriteTextToFileCommandSudo(text, filePath);
   RunConsoleCommandSync(writeTextToFileCommand, calledFunc);
}

void Terminal::DeleteFileSudo(const QString filePath, const char* calledFunc)
{
   const  QString deleteFileCommand = CreateDeleteFileCommandSudo(filePath);
   RunConsoleCommandSync(deleteFileCommand, calledFunc);
}

void Terminal::ClearFolderSudo(const QString &folderPath, const char* calledFunc)
{
   const QString deleteAllFilesCommand = CreateClearFolderCommandSudo(folderPath);
   RunConsoleCommandSync(deleteAllFilesCommand, calledFunc);
}

void Terminal::ClearFileSudo(const QString &filePath, const char* calledFunc)
{
   const QString clearFileCommand = CreateClearFileCommandSudo(filePath);
   RunConsoleCommandSync(clearFileCommand, calledFunc);
}

void Terminal::CopyFileSudo(const QString source, const QString &destination, const char* calledFunc)
{
   const QString copyFileCommand = CreateCopyFileCommandSudo(source, destination);
   RunConsoleCommandSync(copyFileCommand, calledFunc);
}

void Terminal::CopyFolderSudo(const QString &source, const QString &destination, const char *calledFunc)
{
    const QString copyFolderCommand= CreateCopyFolderCommandSudo(source, destination);
    RunConsoleCommandSync(copyFolderCommand, calledFunc);
}

QStringList Terminal::GetAllProcessList(const char* calledFunc)
{
    const QString processListCommand=CreateGettAllProcessListCommand();
    const QString processes=RunConsoleCommandSync(processListCommand, calledFunc);
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

void Terminal::KillProcess(const QString &processName, const char* calledFunc)
{
    const QString killProcessCommand=CreateKillProcessCommandSudo(processName);
    RunConsoleCommandSync(killProcessCommand, calledFunc);
}

QStringList Terminal::GetAllInstalledPackageNames(const char* calledFunc)
{
    const QString allInstalledPackagesNamesCommand=CreateGettingAllInstalledPackagesNamesListCommandSudo();
    const QString packagesInstalled=RunConsoleCommandSync(allInstalledPackagesNamesCommand, calledFunc);
    QStringList packageInstalledList=packagesInstalled.split('\n');
    packageInstalledList.removeLast();
    return packageInstalledList;
}

QStringList Terminal::GetAllNotInstalledPackageNames(const char* calledFunc)
{
    const QString allNotInstalledPackagesNamesCommand=CreateGettingAllNotInstalledPackageNamesListCommandSudo();
    const QString packagesNotInstalled=RunConsoleCommandSync(allNotInstalledPackagesNamesCommand, calledFunc);
    QStringList packageNotInstalledList=packagesNotInstalled.split('\n');
    packageNotInstalledList.removeLast();
    return packageNotInstalledList;
}

void Terminal::InstallPackageSudo(const QString &packageName, const char* calledFunc)
{
    const QString installCommand=CreateInstallPackageCommandSudo(packageName);
    RunConsoleCommandWithWarnings(installCommand, calledFunc);//это костыль астра кидает ворнинги (из за каких то системных либ)
}

const QString Terminal::CreateGetAllUsersCommand() const
{
    return QStringLiteral("awk -F: '{print $1 \":\" $3}' /etc/passwd");
}

QString Terminal::CreateCreateFolderCommand(const QString &folderPath, bool hasRoot) const
{
   if (hasRoot) {
      return QStringLiteral("sudo mkdir '") + folderPath + '\'';
   } else {
      return QStringLiteral("mkdir '") + folderPath + '\'';
   }
}

QString Terminal::CreateCreateFileCommand(const QString &filePath, bool hasRoot) const
{
   if (hasRoot) {
      return QStringLiteral("sudo touch '") + filePath + '\'';
   } else {
      return QStringLiteral("touch '") + filePath + '\'';
   }
}

QString Terminal::CreateGetFolderListCommand(const QString &folderPath, bool hasRoot) const
{
   if (hasRoot) {
      return QStringLiteral("sudo ls '") + folderPath + QStringLiteral("' -F -1 | grep /$ -");
   } else {
      return QStringLiteral("ls '") + folderPath + QStringLiteral("' -F -1 | grep /$ -");
   }
}

QString Terminal::CreateGetFileListCommand(const QString &folderPath, bool hasRoot) const
{
   if (hasRoot) {
      return QStringLiteral("sudo ls '") + folderPath + QStringLiteral("' -F -1 | grep /$ -v");
   } else {
      return QStringLiteral("ls '") + folderPath + QStringLiteral("' -F -1 | grep /$ -v");
   }
}

QString Terminal::CreateGetFileTextCommand(const QString &filePath, bool hasRoot) const
{
   if (hasRoot) {
      return QStringLiteral("sudo cat '") + filePath + '\'';
   } else {
      return QStringLiteral("cat '") + filePath + '\'';
   }
}

QString Terminal::CreateAppendTextToFileCommandSudo(const QString &text, const QString &filePath) const
{
    return QStringLiteral("sudo sh -c \"echo -e \"'")+ text +QStringLiteral("'\" >> '")+filePath+QStringLiteral("'\"");
}

QString Terminal::CreateWriteTextToFileCommandSudo(const QString &text, const QString &filePath) const
{
    return QStringLiteral("sudo sh -c 'echo \"") + text + QStringLiteral("\" >> ") + filePath + '\'';
}

QString Terminal::CreateDeleteFileCommandSudo(const QString &filePath) const
{
   return QStringLiteral("sudo rm '") + filePath + '\'';
}

QString Terminal::CreateClearFileCommandSudo(const QString &filePath) const
{
   return QStringLiteral("sudo truncate -s 0 '") + filePath + '\'';
}

QString Terminal::CreateClearFolderCommandSudo(const QString &folderPath) const
{
   return QStringLiteral("sudo rm -rf ") + folderPath + '*';
}

QString Terminal::CreateDeleteEmptyFolderCommandSudo(const QString &folderPath) const
{
   return QStringLiteral("sudo rmdir -p ") + folderPath;
}

QString Terminal::CreateCopyFileCommandSudo(const QString &source, const QString &destination) const
{
    return QStringLiteral("sudo cp '") + source + QStringLiteral("' '") + destination + '\'';
}

QString Terminal::CreateCopyFolderCommandSudo(const QString &source, const QString &destination) const
{
    return QStringLiteral("sudo cp -R ")+ source + QStringLiteral("' '") + destination + '\'';
}

QString Terminal::CreateGettAllProcessListCommand() const
{
    return QStringLiteral("ps -eo comm");
}

QString Terminal::CreateKillProcessCommandSudo(const QString &processName) const
{
    return QStringLiteral("sudo killall ") +processName;
}

QString Terminal::CreateKillProcessCommandSudo(const QString &code, const QString &processId) const
{
    return QStringLiteral("sudo kill ")+code+' '+processId;
}

QString Terminal::CreateGettingAllInstalledPackagesNamesListCommandSudo() const
{
    return QStringLiteral("apt-mark showmanual");
}

QString Terminal::CreateGettingAllNotInstalledPackageNamesListCommandSudo() const
{
    return  QStringLiteral("aptitude -F %p search '!~i'");
}

QString Terminal::CreateInstallPackageCommandSudo(const QString &packageName) const
{
    return QStringLiteral("sudo apt-get -qq install -y --install-suggests ")+packageName;
}
