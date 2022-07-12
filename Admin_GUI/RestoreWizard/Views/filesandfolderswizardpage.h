#ifndef ADMIN_GUI_RESTOREWIZARD_VIEWS_FILESANDFOLDERSWIZARDPAGE_H
#define ADMIN_GUI_RESTOREWIZARD_VIEWS_FILESANDFOLDERSWIZARDPAGE_H

#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

#include "Logging/logger.h"

#include "Admin_GUI/RestoreWizard/Services/filesandfoldersserivce.h"

#include "Admin_GUI/RestoreWizard/Views/mywizardpage.h"

enum class AddFolderError
{
    NoError =0,
    TryToAddHomeFolder =1,
    AddSystemFolder = 3 ,
};

class FilesAndFoldersWizardPage : public MyWizardPage
{
public:
    FilesAndFoldersWizardPage(const QString currentUserName, FilesAndFoldersSerivce *service, QWidget *parent);
    ~FilesAndFoldersWizardPage();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();
private Q_SLOTS:
    void OnAddNewFile();
    void OnAddNewFolder();
public:
    virtual int nextId() const Q_DECL_OVERRIDE;
    virtual void initializePage() Q_DECL_OVERRIDE;
protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private:
    quint32 GetFilesCount(const QString &path, bool needToCountDirs=false) const noexcept;
    AddFolderError IsCopiedDirPathCorrect(const QString &path) const noexcept;
    void ParceAddFolderError(AddFolderError error);
private:
    FilesAndFoldersSerivce * const m_service;
    const QString m_currentUserName;
private:
    QLabel *m_topLabel;

    QLabel *m_filesLabel;
    QListWidget *m_filesListWidget;
    QPushButton *m_addFileButton;

    QLabel *m_foldersLabel;
    QTableWidget *m_foldersTableWidget;
    QPushButton *m_addFolderButton;
};

#endif // ADMIN_GUI_RESTOREWIZARD_VIEWS_FILESANDFOLDERSWIZARDPAGE_H
