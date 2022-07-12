#include "filesandfoldersserivce.h"

FilesAndFoldersSerivce::FilesAndFoldersSerivce(const QString &rlsTiFolders)
    : m_rlsTiFolder(rlsTiFolders)
    , m_terminal(Terminal::GetTerminal())
{

}

FilesAndFoldersSerivce::~FilesAndFoldersSerivce()
{

}

void FilesAndFoldersSerivce::SetBackupFolderPath(const QString &backupFolder) noexcept
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

void FilesAndFoldersSerivce::ClearAll() noexcept
{
    m_filesNames.clear();
    m_foldersNames.clear();
    m_backupFolder.clear();
}

void FilesAndFoldersSerivce::CopyFiles()
{
    for (const QString &fileFullPath : qAsConst(m_filesNames))
    {
        const QString fileName=GetEntityName(fileFullPath);
        const QString destinationFile = m_rlsTiFolder + fileName;
        if (m_terminal->IsFileExists(fileFullPath, Q_FUNC_INFO, true))
        {
            if (m_terminal->IsFileExists(destinationFile, Q_FUNC_INFO, true))
            {
                m_terminal->DeleteFileSudo(destinationFile, Q_FUNC_INFO);
            }
            m_terminal->CopyFileSudo(fileFullPath, m_rlsTiFolder, Q_FUNC_INFO);
            m_terminal->SetPermissionToExecuteSudo(destinationFile, Q_FUNC_INFO);
        }
    }
}

void FilesAndFoldersSerivce::CopyFolders()
{
    for (const QString &folderPath : qAsConst(m_foldersNames))
    {
        const QString folderName=GetEntityName(folderPath);
        const QString resultFolder = m_rlsTiFolder + folderName;
        if (m_terminal->IsDirExists(folderPath, Q_FUNC_INFO, true))
        {
            if (m_terminal->IsDirExists(resultFolder, Q_FUNC_INFO, true))
            {
                m_terminal->DeleteFolderSudo(resultFolder, Q_FUNC_INFO);
            }
            m_terminal->CopyFolderSudo(folderPath, m_rlsTiFolder, Q_FUNC_INFO);
        }
    }
}

const QString &FilesAndFoldersSerivce::GetBackupFolderPath() const noexcept
{
    return m_backupFolder;
}

const QStringList &FilesAndFoldersSerivce::GetAllFilesList() const noexcept
{
    return m_filesNames;
}

const QStringList &FilesAndFoldersSerivce::GetAllFoldersList() const noexcept
{
    return m_foldersNames;
}

bool FilesAndFoldersSerivce::AddFileToList(const QString &newFilePath) noexcept
{
    if(m_filesNames.contains(newFilePath))
    {
        return false;
    }
    else
    {
        m_filesNames.append(newFilePath);
        return true;
    }
}

bool FilesAndFoldersSerivce::AddFolderToList(const QString &newFolder) noexcept
{
    if(m_foldersNames.contains(newFolder))
    {
        return false;
    }
    else
    {
        m_foldersNames.append(newFolder);
        return true;
    }
}

QStringList FilesAndFoldersSerivce::ParseDomElement(const QDomElement &elem) const noexcept
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
    QStringList exsistsFullPathsFiles;
    for (const QString &fileName : fileNames)
    {
        const QString fullPathToFile = m_backupFolder + fileName;
        if (m_terminal->IsFileExists(fullPathToFile, Q_FUNC_INFO, true))
        {
            exsistsFullPathsFiles.append(fullPathToFile);
        }
    }
    return exsistsFullPathsFiles;
}

QString FilesAndFoldersSerivce::GetEntityName(const QString &fullPath)
{
    return fullPath.mid(fullPath.lastIndexOf('/')+1);
}

QStringList FilesAndFoldersSerivce::ReturnExsistsFullPathsFolders(const QStringList &folderNames)
{
    QStringList exsistsFullFolderPaths;
    for (const QString &folderName : folderNames)
    {
        const QString fullPathToFile = m_backupFolder + folderName;
        if (m_terminal->IsDirExists(fullPathToFile, Q_FUNC_INFO, true))
        {
            exsistsFullFolderPaths.append(fullPathToFile);
        }
    }
    return exsistsFullFolderPaths;
}

