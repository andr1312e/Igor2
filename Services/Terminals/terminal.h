#ifndef SERVICES_TERMINAL_H
#define SERVICES_TERMINAL_H
#include <QProcess>
#include <QFile>
#include <QDir>

class Terminal
{
public:
    
    Terminal();
    
    ~Terminal();

    QString runConsoleCommand(const QString &command, QString calledFulc);

    void checkAndCreateFolder(const QString folderPath, const QString calledFulc, bool hasRoot=true);

    void checkAndCreateFile(const QString filePath, const QString calledFunc, bool hasRoot=true);

    void createFile(const QString filePath, const QString calledFunc, bool hasRoot=true);

    void createFolder(const QString &folderPath, const QString callerFunc, bool hasRoot=true);

    QStringList getFileList(const QString &folderPath, QString calledFulc, bool hasRoot=true);

    QStringList getFolderList(const QString &folderPath, QString calledFulc, bool hasRoot=true);

    QString getFileText(const QString filePath, const QString calledFulc, bool hasRoot=true);

    void writeTextToFileSudo(const QString &text, const QString filePath,const QString calledFunc);

    void deleteFileSudo(const QString filePath, const QString calledFulc);

    void clearFileSudo(const QString &filePath, const QString calledFunc);

    void clearFolderSudo(const QString &folderPath, const QString calledFunc);

    void copyFileSudo(const QString source, const QString &destination, const QString calledFunc);

private:

    QString createCreateFolderCommand(const QString &folderPath, bool hasRoot=true);

    QString createCreateFileCommand(const QString &filePath, bool hasRoot=true);

    QString createGetFolderListCommand(const QString &folderPath, bool hasRoot=true);

    QString createGetFileListCommand(const QString &folderPath, bool hasRoot=true);

    QString createGetFileTextCommand(const QString &filePath, bool hasRoot=true);

    QString createWriteTextToFileCommandSudo(const QString &filePath, const QString &text);

    QString createDeleteFileCommandSudo(const QString &filePath);

    QString createClearFileCommandSudo(const QString &filePath);

    QString createClearFolderCommandSudo(const QString &folderPath);

    QString createCopyFileCommandSudo(const QString &source, const QString &destination);

private:

    QProcess *m_process;

    QStringList *m_consoleCommand;

};

#endif // SERVICES_TERMINAL_H
