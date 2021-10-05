#include "terminal.h"
#include <QDebug>

Terminal::Terminal()
    : m_process(new QProcess())
    , m_consoleCommand(new QStringList())
{
    m_consoleCommand->append("-c");
    m_consoleCommand->append("-c");
}

Terminal::~Terminal()
{
    delete m_consoleCommand;
    delete m_process;
}

QString Terminal::runConsoleCommand(const QString &command, const QString calledFulc)
{
    m_consoleCommand->last()=command;
    m_process->start("/bin/sh", *m_consoleCommand);
    m_process->waitForFinished(-1);
    QString processError=m_process->readAllStandardError();
    QString processOutput=m_process->readAllStandardOutput();
    if (processError!="")
    {
        qInfo()<< processError;
        qFatal("Работа невозможна метод: " + calledFulc.toLocal8Bit() + " выдал ошибку при комманде " + command.toLocal8Bit() + " . Cообщение об ошибке " +processError.toLocal8Bit());
    }
    m_process->close();
    return processOutput;
}

void Terminal::checkAndCreateFolder(const QString folderPath, const QString calledFulc, bool hasRoot)
{
    QStringList listToPath=folderPath.split('/');
    if (listToPath.isEmpty())
    {
        qFatal("Работа невозможна метод: " + calledFulc.toLocal8Bit() + " выдал пуcтой путь. Путь " + folderPath.toLocal8Bit() );
    }
    else
    {
        listToPath.removeAll("");
        QString startPath="/";
        for (QStringList::iterator it=listToPath.begin(); it!=listToPath.end(); ++it)
        {
            if (*it=="Desktop")
            {
                QStringList files=getFileList(startPath, calledFulc, hasRoot);
                QStringList folders=getFolderList(startPath, calledFulc, hasRoot);
                if (!(files.contains((*it)+"@")) && !(folders.contains((*it)+"/")))
                {
                    createFolder(startPath+*it+"/", calledFulc, hasRoot);
                }
            }
            else
            {
                QStringList folders=getFolderList(startPath, calledFulc, hasRoot);
                if (!folders.contains((*it)+"/"))
                {
                    createFolder(startPath+*it+"/", calledFulc, hasRoot);
                }
            }
            startPath=startPath+*it+"/";
        }
    }
}

void Terminal::checkAndCreateFile(const QString filePath, const QString calledFunc, bool hasRoot)
{
    int backSlashPos=filePath.lastIndexOf('/', filePath.count()-2);
    QString folderPath=filePath.left(backSlashPos);
    QString fileName=filePath.mid(backSlashPos+1);
    QStringList files=getFileList(folderPath, calledFunc, hasRoot);
    if (!files.contains(fileName))
    {
        createFile(filePath, calledFunc, hasRoot);
    }
}

void Terminal::createFile(const QString filePath, const QString calledFunc, bool hasRoot)
{
    QString createIconCommand=createCreateFileCommand(filePath, hasRoot);
    runConsoleCommand(createIconCommand, calledFunc);
}

void Terminal::createFolder(const QString &folderPath,const QString callerFunc, bool hasRoot)
{
    QString createFolderCommand=createCreateFolderCommand(folderPath, hasRoot);
    runConsoleCommand(createFolderCommand, callerFunc);
}

QStringList Terminal::getFileList(const QString &folderPath, QString calledFulc, bool hasRoot)
{
    QString getFileListCommand=createGetFileListCommand(folderPath, hasRoot);
    QString files = runConsoleCommand(getFileListCommand, calledFulc);
    return files.split('\n');
}

QStringList Terminal::getFolderList(const QString &folderPath, QString calledFulc, bool hasRoot)
{
    QString getFoldersListCommand=createGetFolderListCommand(folderPath, hasRoot);
    QString folders = runConsoleCommand(getFoldersListCommand, calledFulc);
    return folders.split('\n');
}

QString Terminal::getFileText(const QString filePath, const QString calledFulc, bool hasRoot)
{
    QString getFileTextCommand=createGetFileTextCommand(filePath, hasRoot);
    return runConsoleCommand(getFileTextCommand, calledFulc);
}

void Terminal::writeTextToFileSudo(const QString &text, const QString filePath, const QString calledFunc)
{
    QString writeTextToFileCommand=createWriteTextToFileCommandSudo(filePath, text);
    runConsoleCommand(writeTextToFileCommand, calledFunc);
}

void Terminal::deleteFileSudo(const QString filePath, const QString calledFulc)
{
    QString deleteFileCommand=createDeleteFileCommandSudo(filePath);
    runConsoleCommand(deleteFileCommand, calledFulc);
}

void Terminal::clearFileSudo(const QString &filePath, const QString calledFunc)
{
    QString clearFileCommand=createClearFileCommandSudo(filePath);
    runConsoleCommand(clearFileCommand, calledFunc);
}

void Terminal::clearFolderSudo(const QString &folderPath, const QString calledFunc)
{
    QString deleteAllFilesCommand=createClearFolderCommandSudo(folderPath);
    runConsoleCommand(deleteAllFilesCommand, calledFunc);
}

void Terminal::copyFileSudo(const QString source, const QString &destination, const QString calledFunc)
{
    QString copyFileCommand=createCopyFileCommandSudo(source, destination);
    runConsoleCommand(copyFileCommand, calledFunc);
}

QString Terminal::createCreateFolderCommand(const QString &folderPath, bool hasRoot)
{
    if (hasRoot)
    {
        return "sudo mkdir '" + folderPath + "'";
    }
    else
    {
        return "mkdir '" + folderPath + "'";
    }
}

QString Terminal::createCreateFileCommand(const QString &filePath, bool hasRoot)
{
    if (hasRoot)
    {
        return "sudo touch '" + filePath + "'";
    }
    else
    {
        return "touch '" + filePath + "'";
    }
}

QString Terminal::createGetFolderListCommand(const QString &folderPath, bool hasRoot)
{
    if (hasRoot)
    {
        return "sudo ls '" + folderPath + "' -F -1 | grep /$ -";
    }
    else
    {
        return "ls '" + folderPath + "' -F -1 | grep /$ -";
    }
}

QString Terminal::createGetFileListCommand(const QString &folderPath, bool hasRoot)
{
    if (hasRoot)
    {
        return "sudo ls '" + folderPath + "' -F -1 | grep /$ -v";
    }
    else
    {
        return "ls '" + folderPath + "' -F -1 | grep /$ -v";
    }
}

QString Terminal::createGetFileTextCommand(const QString &filePath, bool hasRoot)
{
    if (hasRoot)
    {
        return "sudo cat '" + filePath+ "'";
    }
    else
    {
        return "cat '" + filePath+ "'";
    }
}

QString Terminal::createWriteTextToFileCommandSudo(const QString &filePath, const QString &text)
{
    return "sudo sh -c 'echo \"" + text + "\" >> " +filePath +"'";
}

QString Terminal::createDeleteFileCommandSudo(const QString &filePath)
{
    return "sudo rm '" + filePath +"'";
}

QString Terminal::createClearFileCommandSudo(const QString &filePath)
{
    return "sudo truncate -s 0 '" +filePath+"'";
}

QString Terminal::createClearFolderCommandSudo(const QString &folderPath)
{
    return "sudo rm -rf " + folderPath + "*";
}

QString Terminal::createCopyFileCommandSudo(const QString &source, const QString &destination)
{
    return "sudo cp '" +source + "' '" + destination + "'";
}
