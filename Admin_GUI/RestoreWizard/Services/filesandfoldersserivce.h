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
    void SetBackupFolderPath(const QString &backupFolder) noexcept;
    void GetFilesFromBackup(const QDomElement &elem);
    void GetFoldersFromBackup(const QDomElement &elem);
    void ClearAll() noexcept;
public:
    void CopyFiles();
    void CopyFolders();
public:
    const QString &GetBackupFolderPath() const noexcept;
    const QStringList &GetAllFilesList() const noexcept;
    const QStringList &GetAllFoldersList() const noexcept;
public:
    bool AddFileToList(const QString &newFilePath) noexcept;
    bool AddFolderToList(const QString &newFolder) noexcept;
private:
    QStringList ParseDomElement(const QDomElement &elem) const noexcept;
    QStringList ReturnExsistsFullPathFiles(const QStringList &fileNames);
    QStringList ReturnExsistsFullPathsFolders(const QStringList &folderNames);
    QString GetEntityName(const QString &fullPath);
private:
    const QString m_rlsTiFolder;
    Terminal *const m_terminal;
private:
    QString m_backupFolder;
    QStringList m_filesNames;
    QStringList m_foldersNames;
};

#endif // FILESANDFOLDERSSERIVCE_H
