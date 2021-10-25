#ifndef SERVICES_TERMINALS_TERMINAL_H
#define SERVICES_TERMINALS_TERMINAL_H
#include <QProcess>
#include <QString>
#include <QDebug>

#include "functionswithstrings.h"

class Terminal
{
public:

   Terminal();

   ~Terminal();

   QString RunConsoleCommand(const QString &command, QString calledFunc);

   bool IsDirNotExists(const QString folderPath, const QString calledFunc, bool hasRoot = true);

   bool IsFileExists(const QString filePath, const QString calledFunc, bool hasRoot = true);

   bool IsFileNotExists(const QString filePath, const QString calledFunc, bool hasRoot = true);

   void CheckAndCreatePathToElement(const QString path, const QString calledFunc, bool hasRoot);

   void CreateFolder(const QString &folderPath, const QString callerFunc, bool hasRoot = true);

   void CreateFile(const QString filePath, const QString calledFunc, bool hasRoot = true);

   QStringList GetFolderList(const QString &folderPath, QString calledFulc, bool hasRoot = true);

   QStringList GetFileList(const QString &folderPath, QString calledFulc, bool hasRoot = true);

   QString GetFileText(const QString filePath, const QString calledFunc, bool hasRoot = true);

   void WriteTextToFileSudo(const QByteArray &text, const QString filePath, const QString calledFunc);

   void DeleteFileSudo(const QString filePath, const QString calledFunc);

   void ClearFolderSudo(const QString &folderPath, const QString calledFunc);

   void ClearFileSudo(const QString &filePath, const QString calledFunc);

   void CopyFileSudo(const QString source, const QString &destination, const QString calledFunc);

private:

   QString CreateCreateFolderCommand(const QString &folderPath, bool hasRoot = true);

   QString CreateCreateFileCommand(const QString &filePath, bool hasRoot = true);

   QString CreateGetFolderListCommand(const QString &folderPath, bool hasRoot = true);

   QString CreateGetFileListCommand(const QString &folderPath, bool hasRoot = true);

   QString CreateGetFileTextCommand(const QString &filePath, bool hasRoot = true);

   QString CreateWriteTextToFileCommandSudo(const QString &filePath, const QByteArray &text);

   QString CreateDeleteFileCommandSudo(const QString &filePath);

   QString CreateClearFileCommandSudo(const QString &filePath);

   QString CreateClearFolderCommandSudo(const QString &folderPath);

   QString CreateDeleteEmptyFolderCommandSudo(const QString &folderPath);

   QString CreateCopyFileCommandSudo(const QString &source, const QString &destination);

private:

   QProcess *m_process;

   QStringList *m_consoleCommand;

};

#endif // SERVICES_TERMINALS_TERMINAL_H
