#ifndef ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H
#define ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H

#include <QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

#include "Services/linuxuserservice.h"

#include "Services/Settings/wizardservice.h"
#include "Services/Settings/programfilesstate.h"
#include "Services/Settings/usersettingsrepository.h"
#include "Services/Settings/roleappsandstartupsettingsrepository.h"


#include "Admin_GUI/Views/Wizards/intropage.h"
#include "Admin_GUI/Views/Wizards/wizardpages.h"
#include "Admin_GUI/Views/Wizards/userwizardpage.h"
#include "Admin_GUI/Views/Wizards/roleappswizardpage.h"
#include "Admin_GUI/Views/Wizards/conclusionwizardpage.h"

class StartupWizard : public QWizard
{
   Q_OBJECT
public:
   StartupWizard(const QString &rlsTiFolder, LoadingState &loadedDbAdnRolesState, LinuxUserService *linuxUserService, ISqlDatabaseService *iSqlDataBaseService, QWidget *parent);
   ~StartupWizard();
private:
   void CreateServices(const QString &rlsTiFolder, LoadingState &loadedDbAdnRolesState, LinuxUserService *linuxUserService, ISqlDatabaseService *iSqlDataBaseService);
   void CreateUI(LoadingState &loadedDbAdnRolesState);
   void InitSizes();
   void InitStyles();
   void InitBehaviour();
   void ConnectObjects();

Q_SIGNALS:
   void ToChangeTheme(bool state);
   void ToSetDbAndIconsPaths(const QStringList &paths);

private Q_SLOTS:
   void OnHelpButtonClick();
   void OnThemeButtonClick(bool checked);

public:
   void accept() Q_DECL_OVERRIDE;
   void reject() Q_DECL_OVERRIDE;

private:
   WizardService *m_wizardService;

   QPushButton *m_themePushButton;
   IntroPage *m_introPage;
   UserWizardPage *m_userWizardPage;
   QVector<RoleAppsWizardPage *> m_rolesPages;
   ConclusionWizardPage *m_conclusionPage;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H
