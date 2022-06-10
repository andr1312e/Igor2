#ifndef ADMIN_GUI_VIEWS_WIZARD_INTROPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_INTROPAGE_H
#include <QLabel>
#include <QWizardPage>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QDateTime>
#include <QStandardPaths>

#include <sys/types.h>
#include <sys/stat.h>

#include "Admin_GUI/RestoreWizard/Services/wizardservice.h"
#include "Admin_GUI/RestoreWizard/Structs/programfilesstate.h"
#include "Admin_GUI/RestoreWizard/Services/dependenciesserivce.h"

#include "Admin_GUI/RestoreWizard/Views/wizardnavigtionbar.h"
#include "Admin_GUI/RestoreWizard/Views/wizardpages.h"
#include "Admin_GUI/RestoreWizard/Views/mywizardpage.h"

#include "Styles/Themes/themecolors.h"

class IntroPage : public MyWizardPage
{
    Q_OBJECT
public:
    explicit IntroPage(LoadingStates state, WizardService *service, ThemesNames themeName, QWidget *parent);
    ~IntroPage();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI(LoadingStates state);
    void ConnectObjects();

private Q_SLOTS:
    void OnCheckBackupFile();
    void OnClearBackup();
    void OnSendBackupParceError(const QString &error);

public:
    virtual int nextId() const Q_DECL_OVERRIDE;
    void SetWizardStyles(ThemesNames themeName);


private:
    WizardService *const m_wizardService;

    QLabel *m_topLabel;
    QVBoxLayout *m_backupFileLayout;
    QHBoxLayout *m_backupFileLoadLayout;
    QPushButton *m_backupLoadButton;
    QPushButton *m_backupClearButton;
    QLabel *m_backupLabel;
    QLineEdit *m_backupLineEdit;
    QLabel *m_faqLabel;
    QLabel *m_errorLabel;

    QString m_backupFilePath;

};

#endif // ADMIN_GUI_VIEWS_WIZARD_INTROPAGE_H
