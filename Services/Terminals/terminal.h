#ifndef SERVICES_TERMINALS_TERMINAL_H
#define SERVICES_TERMINALS_TERMINAL_H
#include <QProcess>
#include <QString>
#include <QDebug>

#include "functionswithstrings.h"

class Terminal
{
public:
    static Terminal *GetTerminal();
    ~Terminal();
private:
    explicit Terminal();

public:
    QString RunConsoleCommandSync(const QString &command, const char *calledFunc);

    void RunConsoleCommandAsync(const QString &command);

    void RunConsoleCommandSync(const QString &command, QString &outputInfo, QString &errorInfo);

public:

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

    void AppendTextToFileSudo(const QString &text, const QString &filePath, const char *calledFunc);

    void WriteTextToFileSudo(const QString &text, const QString &filePath, const char *calledFunc);

    void DeleteEmptyFolderSudo(const QString &folderPath, const char *calledFunc);

    void DeleteFolderSudo(const QString &folderPath, const char *calledFunc);

    void DeleteFileSudo(const QString filePath, const char *calledFunc);

    void ClearFolderSudo(const QString &folderPath, const char *calledFunc);

    void ClearFileSudo(const QString &filePath, const char *calledFunc);

    void CopyFileSudo(const QString source, const QString &destination, const char *calledFunc);

    void CopyFolderSudo(const QString &source, const QString &destination, const char *calledFunc);

    void SetPermissionToExecuteSudo(const QString &filePath, const char *calledFunc);

    QStringList GetAllProcessList(const char *calledFunc);

    void KillProcess(const QString &processName, const char *calledFunc);

    QStringList GetAllInstalledPackageNames(const char *calledFunc);

    QStringList GetAllNotInstalledPackageNames(const char *calledFunc);

    void InstallPackageSudo(const QString &packageName, QString &outpiutInfo, QString &errorInfo);

private:

    const QString CreateGetAllUsersCommand() const noexcept;

    QString CreateCreateFolderCommand(const QString &folderPath, bool hasRoot = true) const noexcept;

    QString CreateCreateFileCommand(const QString &filePath, bool hasRoot = true) const noexcept;

    QString CreateGetFolderListCommand(const QString &folderPath, bool hasRoot = true) const noexcept;

    QString CreateGetFileListCommand(const QString &folderPath, bool hasRoot = true) const noexcept;

    QString CreateGetFileTextCommand(const QString &filePath, bool hasRoot = true) const noexcept;

    QString CreateAppendTextToFileCommandSudo(const QString &text, const QString &filePath) const noexcept;

    QString CreateWriteTextToFileCommandSudo(const QString &text, const QString &filePath) const noexcept;

    QString CreateDeleteFileCommandSudo(const QString &filePath) const noexcept;

    QString CreateClearFileCommandSudo(const QString &filePath) const noexcept;

    QString CreateClearFolderCommandSudo(const QString &folderPath) const noexcept;

    QString CreateDeleteEmptyFolderCommandSudo(const QString &folderPath) const noexcept;

    QString CreateDeleteFolderCommandSudo(const QString &folderPath) const noexcept;

    QString CreateCopyFileCommandSudo(const QString &source, const QString &destination) const noexcept;

    QString CreateCopyFolderCommandSudo(const QString &source, const QString &destination) const noexcept;

    QString CreatePermissionToExecuteCommandSudo(const QString &filePath) const noexcept;

    QString CreateGettAllProcessListCommand() const noexcept;

    QString CreateKillProcessCommandSudo(const QString &processName) const noexcept;

    QString CreateKillProcessCommandSudo(const QString &code, const QString &processId) const noexcept;

    QString CreateGettingAllInstalledPackagesNamesListCommandSudo() const noexcept;

    QString CreateGettingAllNotInstalledPackageNamesListCommandSudo() const noexcept;

    QString CreateInstallPackageCommandSudo(const QString &packageName) const noexcept;

private:

    static Terminal *m_terminal;

    QProcess *const m_process;

    QStringList m_consoleCommand;

    const QString m_terminalCommand;

};

#endif // SERVICES_TERMINALS_TERMINAL_H
