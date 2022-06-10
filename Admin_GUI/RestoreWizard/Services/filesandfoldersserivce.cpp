#include "filesandfoldersserivce.h"

FilesAndFoldersSerivce::FilesAndFoldersSerivce(const QString &rlsTiFolders)
    : m_rlsTiFolders(rlsTiFolders)
    , m_terminal(Terminal::GetTerminal())
{

}

FilesAndFoldersSerivce::~FilesAndFoldersSerivce()
{

}

void FilesAndFoldersSerivce::SetBackupFolder(const QString &backupFolder)
{
    m_backupFolder = backupFolder;
}

void FilesAndFoldersSerivce::GetFilesFromBackup(const QDomElement &elem)
{
    if (QStringLiteral("FILES") == elem.tagName())
    {
        QStringList fileNamesInBackup = ParseDomElement(elem);
        fileNamesInBackup.removeDuplicates();
        m_filesNames = ReturnExsistsFullPathFiles(fileNamesInBackup);
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Получили тэг с неверным именем. Должен быть FILES а имеем ") + elem.tagName()).toUtf8().constData());
    }
}

void FilesAndFoldersSerivce::GetFoldersFromBackup(const QDomElement &elem)
{
    if (QStringLiteral("FOLDERS") == elem.tagName())
    {
        QStringList folderNamesInBackup = ParseDomElement(elem);
        folderNamesInBackup.removeDuplicates();
        m_foldersNames = ReturnExsistsFullPathsFolders(folderNamesInBackup);
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Получили тэг с неверным именем. Должен быть FOLDERS а имеем ") + elem.tagName()).toUtf8().constData());
    }
}

void FilesAndFoldersSerivce::CopyFiles()
{
    for (const QString &fileName : qAsConst(m_filesNames))
    {
        const QString sourceFile = m_backupFolder + fileName;
        const QString destinationFile = m_rlsTiFolders + fileName;
        if (m_terminal->IsFileExists(sourceFile, Q_FUNC_INFO, true))
        {
            m_terminal->CopyFileSudo(sourceFile, destinationFile, Q_FUNC_INFO);
            m_terminal->SetPermissionToExecuteSudo(destinationFile, Q_FUNC_INFO);
        }
    }
}

void FilesAndFoldersSerivce::CopyFolders()
{
    for (const QString &folderName : qAsConst(m_foldersNames))
    {
        const QString sourceFolder = m_backupFolder + folderName;
        if (m_terminal->IsDirExists(folderName, Q_FUNC_INFO, true))
        {
            m_terminal->CopyFolderSudo(sourceFolder, m_rlsTiFolders, Q_FUNC_INFO);
        }
    }
}

QStringList FilesAndFoldersSerivce::ParseDomElement(const QDomElement &elem) const
{
    QStringList items;
    const QDomNodeList nodesList(elem.childNodes());
    for (int i = 0; i < nodesList.count(); ++i)
    {
        const QDomElement currentElem = nodesList.at(i).toElement();
        const QString currentText = currentElem.text();
        items.append(currentText);
    }
    return items;
}

QStringList FilesAndFoldersSerivce::ReturnExsistsFullPathFiles(const QStringList &fileNames)
{
    QStringList exsistsFiles;
    for (const QString &fileName : fileNames)
    {
        const QString fullPathToFile = m_backupFolder + fileName;
        if (m_terminal->IsFileExists(fullPathToFile, Q_FUNC_INFO, true))
        {
            exsistsFiles.append(fileName);
        }
    }
    return exsistsFiles;
}

QStringList FilesAndFoldersSerivce::ReturnExsistsFullPathsFolders(const QStringList &folderNames)
{
    QStringList exsistsFolders;
    for (const QString &folderName : folderNames)
    {
        const QString fullPathToFile = m_backupFolder + folderName;
        if (m_terminal->IsDirExists(fullPathToFile, Q_FUNC_INFO, true))
        {
            exsistsFolders.append(folderName);
        }
    }
    return exsistsFolders;
}
