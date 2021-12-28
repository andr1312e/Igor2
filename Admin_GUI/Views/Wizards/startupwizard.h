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
#include "Services/Settings/usersprogramiconmakingservice.h"
#include "Services/Settings/roleappsandstartupsettingsrepository.h"

#include "Admin_GUI/Views/Wizards/wizardnavigtionbar.h"
#include "Admin_GUI/Views/Wizards/intropage.h"
#include "Admin_GUI/Views/Wizards/wizardpages.h"
#include "Admin_GUI/Views/Wizards/userwizardpage.h"
#include "Admin_GUI/Views/Wizards/roleappswizardpage.h"
#include "Admin_GUI/Views/Wizards/conclusionwizardpage.h"

class StartupWizard : public QWizard
{
   Q_OBJECT
public:
   StartupWizard(const QString &rlsTiFolder,const LoadingState &loadedDbAdnRolesState, LinuxUserService *linuxUserService, ISqlDatabaseService *iSqlDataBaseService, QWidget *parent);
   ~StartupWizard();
private:
   void CreateServices(const QString &rlsTiFolder, const LoadingState &loadedDbAdnRolesState, LinuxUserService * const linuxUserService, ISqlDatabaseService * const iSqlDataBaseService);
   void CreateUI(const LoadingState &loadedDbAdnRolesState);
   void InitSizes();
   void InitStyles();
   void InitBehaviour();
   void ConnectObjects();

Q_SIGNALS:
   void ToChangeTheme(bool state);
   void ToSetDbAndIconsPaths(const QStringList &paths);

private Q_SLOTS:
   void OnHelpButtonClick();
   void OnCurrentPageChanged(int id);
   void OnPageNumMove(int pageIdToMove);

public:
   virtual void accept() Q_DECL_OVERRIDE;
   virtual void reject() Q_DECL_OVERRIDE;

private:
   UsersProgramIconMakingService *m_iconMakingService;
   WizardService *m_wizardService;


private:
   WizardNavigtionBar *m_wizardNavigationBar;
   IntroPage *m_introPage;
   UserWizardPage *m_userWizardPage;
   QVector<RoleAppsWizardPage *> m_rolesPages;
   ConclusionWizardPage *m_conclusionPage;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H
