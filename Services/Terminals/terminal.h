#ifndef SERVICES_TERMINALS_TERMINAL_H
#define SERVICES_TERMINALS_TERMINAL_H
#include <QProcess>
#include <QString>
#include <QDebug>

#include "functionswithstrings.h"

class Terminal
{
public:
    static Terminal* GetTerminal();
     ~Terminal();
private:
   Terminal();

public:
   QString RunConsoleCommand(const QString &command, const char *calledFunc);

   QStringList GetAllUsersList(const char *calledFunc);

   bool IsDirExists(const QString folderPath, const char *calledFunc, bool hasRoot = true);

   bool IsDirNotExists(const QString folderPath, const char *calledFunc, bool hasRoot = true);

   bool IsFileExists(const QString filePath, const char *calledFunc, bool hasRoot = true);

   bool IsFileNotExists(const QString filePath, const char *calledFunc, bool hasRoot = true);

   void CheckAndCreatePathToElement(const QString &path, const char *calledFunc, bool hasRoot);

   void CreateFolder(const QString &folderPath, const char *callerFunc, bool hasRoot = true);

   void CreateFile(const QString filePath, const char *calledFunc, bool hasRoot = true);

   QStringList GetFolderList(const QString &folderPath, const char *calledFulc, bool hasRoot = true);

   QStringList GetFileList(const QString &folderPath, const char *calledFulc, bool hasRoot = true);

   QString GetFileText(const QString filePath, const char *calledFunc, bool hasRoot = true);

   void WriteTextToFileSudo(const QByteArray &text, const QString filePath, const char* calledFunc);

   void DeleteFileSudo(const QString filePath, const char *calledFunc);

   void ClearFolderSudo(const QString &folderPath, const char *calledFunc);

   void ClearFileSudo(const QString &filePath, const char *calledFunc);

   void CopyFileSudo(const QString source, const QString &destination, const char *calledFunc);

   QStringList GetAllProcessList(const char *calledFunc);

   void KillProcess(const QString &processName, const char *calledFunc);

   QStringList GetAllInstalledPackageNames(const char *calledFunc);

   QStringList GetAllNotInstalledPackageNames(const char *calledFunc);

   void InstallPackageSudo(const QString &packageName, const char *calledFunc);

private:

   const QString CreateGetAllUsersCommand() const;

   QString CreateCreateFolderCommand(const QString &folderPath, bool hasRoot = true) const;

   QString CreateCreateFileCommand(const QString &filePath, bool hasRoot = true) const;

   QString CreateGetFolderListCommand(const QString &folderPath, bool hasRoot = true) const;

   QString CreateGetFileListCommand(const QString &folderPath, bool hasRoot = true) const;

   QString CreateGetFileTextCommand(const QString &filePath, bool hasRoot = true) const;

   QString CreateWriteTextToFileCommandSudo(const QString &filePath, const QByteArray &text) const;

   QString CreateDeleteFileCommandSudo(const QString &filePath) const;

   QString CreateClearFileCommandSudo(const QString &filePath) const;

   QString CreateClearFolderCommandSudo(const QString &folderPath) const;

   QString CreateDeleteEmptyFolderCommandSudo(const QString &folderPath) const;

   QString CreateCopyFileCommandSudo(const QString &source, const QString &destination) const;

   QString CreateGettAllProcessListCommand() const;

   QString CreateKillProcessCommandSudo(const QString &processName) const;

   QString CreateGettingAllInstalledPackagesNamesListCommandSudo() const;

   QString CreateGettingAllNotInstalledPackageNamesListCommandSudo() const;

   QString CreateInstallPackageCommandSudo(const QString &packageName) const;

private:

   static Terminal* m_terminal;

   QProcess* const m_process;

   QStringList m_consoleCommand;

   const QString m_terminalCommand;

   const int m_commandMilliesecondsTimeOut=10000;

};

#endif // SERVICES_TERMINALS_TERMINAL_H
