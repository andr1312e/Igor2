#ifndef FILESANDFOLDERSSERIVCE_H
#define FILESANDFOLDERSSERIVCE_H
#include <QDomElement>

#include "Services/Terminals/terminal.h"

class FilesAndFoldersSerivce
{
public:
    explicit FilesAndFoldersSerivce(const QString &rlsTiFolders);
    ~FilesAndFoldersSerivce();
public:
    void SetBackupFolder(const QString &backupFolder);
    void GetFilesFromBackup(const QDomElement &elem);
    void GetFoldersFromBackup(const QDomElement &elem);
public:
    void CopyFiles();
    void CopyFolders();
private:
    QStringList ParseDomElement(const QDomElement &elem) const;
    QStringList ReturnExsistsFullPathFiles(const QStringList &fileNames);
    QStringList ReturnExsistsFullPathsFolders(const QStringList &folderNames);
private:
    const QString m_rlsTiFolders;
    Terminal *const m_terminal;
private:
    QString m_backupFolder;
    QStringList m_filesNames;
    QStringList m_foldersNames;
};

#endif // FILESANDFOLDERSSERIVCE_H
