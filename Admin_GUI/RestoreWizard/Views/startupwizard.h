#ifndef ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H
#define ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H

#include <QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSharedPointer>

#include "Services/linuxuserservice.h"

#include "Admin_GUI/RestoreWizard/Services/wizardservice.h"
#include "Admin_GUI/RestoreWizard/Structs/programfilesstate.h"
#include "Admin_GUI/RestoreWizard/Repositories/usersettingsrepository.h"
#include "Admin_GUI/RestoreWizard/Services/iconmaker.h"
#include "Admin_GUI/RestoreWizard/Repositories/roleappsandstartupsettingsrepository.h"

#include "Admin_GUI/RestoreWizard/Views/wizardnavigtionbar.h"
#include "Admin_GUI/RestoreWizard/Views/intropage.h"
#include "Admin_GUI/RestoreWizard/Views/wizardpages.h"
#include "Admin_GUI/RestoreWizard/Views/userwizardpage.h"
#include "Admin_GUI/RestoreWizard/Views/roleappswizardpage.h"
#include "Admin_GUI/RestoreWizard/Views/conclusionwizardpage.h"

#include "Styles/Themes/themecolors.h"

class StartupWizard : public QWizard
{
    Q_OBJECT
public:
    explicit StartupWizard(const QString &applicationPath, const QString &applicationName, const QString &rlsTiFolder, LoadingStates states, LinuxUserService *linuxUserService, ISqlDatabaseService *iSqlDataBaseService,  ThemesNames themeName, QWidget *parent);
    ~StartupWizard();
private:
    void CreateServices(const QString &applicationPath, const QString &applicationName, const QString &rlsTiFolder, LoadingStates states, LinuxUserService *const linuxUserService, ISqlDatabaseService *const iSqlDataBaseService);
    void CreateUI(LoadingStates states, ThemesNames themeName);
    void InitSizes();
    void InitStyles();
    void InitBehaviour();
    void ConnectObjects();

Q_SIGNALS:
    void ToSetDbAndIconsPaths(const QStringList &paths);
public Q_SLOTS:
    void OnChangeTheme(ThemesNames themeName);
private Q_SLOTS:
    void OnHelpButtonClick();
    void OnCurrentPageChanged(int id);
    void OnPageNumMove(int pageIdToMove);

public:
    virtual void accept() Q_DECL_OVERRIDE;
    virtual void reject() Q_DECL_OVERRIDE;

private:
    IconMaker *m_iconMakingService;
    WizardService *m_wizardService;

private:
    WizardNavigtionBar *m_wizardNavigationBar;
    IntroPage *m_introPage;
    UserWizardPage *m_userWizardPage;
    QVector<RoleAppsWizardPage *> m_rolesPages;
    ConclusionWizardPage *m_conclusionPage;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H
