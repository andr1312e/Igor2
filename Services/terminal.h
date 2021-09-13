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

    void checkAndCreateFolder(const QString folderPath, const QString calledFulc);

    void checkAndCreateFile(const QString filePath, const QString calledFunc);

    void createFile(const QString filePath, const QString calledFunc);

    void createFolder(const QString &folderPath, const QString callerFunc);

    QStringList getFileList(const QString &folderPath, QString calledFulc);

    QStringList getFolderList(const QString &folderPath, QString calledFulc);

    QString getFileText(const QString filePath, const QString calledFulc);

    QString getFileTextNoSU(const QString filePath, const QString calledFulc);

    void writeTextToFile(const QString &text, const QString filePath,const QString calledFunc);

    void deleteFile(const QString filePath, const QString calledFulc);

    void clearFile(const QString &filePath, const QString calledFunc);

    void clearFolder(const QString &folderPath, const QString calledFunc);

    void copyFile(const QString filePath, const QString &destination, const QString calledFunc);

private:

    QProcess *m_process;

    QStringList *m_consoleCommand;

};

#endif // SERVICES_TERMINAL_H
