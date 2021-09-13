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
    m_consoleCommand->operator [](1)=command;
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

void Terminal::checkAndCreateFolder(const QString folderPath, const QString calledFulc)
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
        for (QStringList::const_iterator it=listToPath.begin(); it!=listToPath.end(); ++it)
        {
            if (*it=="Desktop")
            {
                QStringList files=getFileList(startPath, calledFulc);
                QStringList folders=getFolderList(startPath, calledFulc);
                if (!(files.contains((*it)+"@")) && !(folders.contains((*it)+"/")))
                {
                    createFolder(startPath+*it+"/", calledFulc);
                }
            }
            else
            {
                QStringList folders=getFolderList(startPath, calledFulc);
                if (!folders.contains((*it)+"/"))
                {
                    createFolder(startPath+*it+"/", calledFulc);
                }
            }
            startPath=startPath+*it+"/";
        }
    }
}

void Terminal::checkAndCreateFile(const QString filePath,const QString calledFunc)
{
    int backSlashPos=filePath.lastIndexOf('/', filePath.count()-2);
    QString folderPath=filePath.left(backSlashPos);
    QString fileName=filePath.mid(backSlashPos+1);
    QStringList files=getFileList(folderPath, calledFunc);
    if (!files.contains(fileName))
    {
        createFile(filePath, calledFunc);
    }

}

void Terminal::createFile(const QString filePath, const QString calledFunc)
{
    QString createIconCommand="sudo touch '" + filePath + "'";
    runConsoleCommand(createIconCommand, calledFunc);
}

void Terminal::createFolder(const QString &folderPath,const QString callerFunc)
{
    QString createFolderCommand="sudo mkdir '" + folderPath + "'";
    runConsoleCommand(createFolderCommand, callerFunc);
}

QStringList Terminal::getFileList(const QString &folderPath, QString calledFulc)
{
    QString getFileListCommand="sudo ls '" + folderPath + "' -F -1 | grep /$ -v";
    QString files = runConsoleCommand(getFileListCommand, calledFulc);
    return files.split('\n');
}

QStringList Terminal::getFolderList(const QString &folderPath, QString calledFulc)
{
    QString getFoldersListCommand="sudo ls '" + folderPath + "' -F -1 | grep /$ -";
    QString folders = runConsoleCommand(getFoldersListCommand, calledFulc);
    return folders.split('\n');
}

void Terminal::writeTextToFile(const QString &text, const QString filePath, const QString calledFunc)
{
    QString writeTextToFileCommand="sudo sh -c 'echo \"" + text + "\" >> " +filePath +"'";
    runConsoleCommand(writeTextToFileCommand, calledFunc);
}

QString Terminal::getFileText(const QString filePath, const QString calledFulc)
{
    QString getFileTextCommand="sudo cat '" + filePath+ "'";
    return runConsoleCommand(getFileTextCommand, calledFulc);
}

QString Terminal::getFileTextNoSU(const QString filePath, const QString calledFulc)
{
    QString getFileTextCommand="cat '" + filePath+ "'";
    return runConsoleCommand(getFileTextCommand, calledFulc);
}

void Terminal::deleteFile(const QString filePath, const QString calledFulc)
{
    QString deleteFileCommand="sudo rm '" + filePath +"'";
    runConsoleCommand(deleteFileCommand, calledFulc);
}

void Terminal::clearFile(const QString &filePath, const QString calledFunc)
{
    QString clearFileCommand="sudo truncate -s 0 '" +filePath+"'";
    runConsoleCommand(clearFileCommand, calledFunc);
}

void Terminal::clearFolder(const QString &folderPath, const QString calledFunc)
{
    QString deleteAllFilesCommand="sudo rm -rf " + folderPath + "*";
    runConsoleCommand(deleteAllFilesCommand, calledFunc);
}

void Terminal::copyFile(const QString source, const QString &destination, const QString calledFunc)
{
    QString copyFileCommand="sudo cp '" +source + "' '" + destination + "'";
    runConsoleCommand(copyFileCommand, calledFunc);
}



