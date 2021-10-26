#ifndef ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H
#define ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H

#include <QWizard>
#include <QLabel>
#include <QLineEdit>

#include "Services/Settings/wizardservice.h"
#include "Services/Settings/programfilesstate.h"
#include "Services/Settings/usersettingsrepository.h"
#include "Services/Settings/roleappsandstartupsettingsrepository.h"

#include "Services/Settings/firststartsettingsservice.h"


#include "Admin_GUI/Views/Wizards/intropage.h"
#include "Admin_GUI/Views/Wizards/wizardpages.h"
#include "Admin_GUI/Views/Wizards/userwizardpage.h"
#include "Admin_GUI/Views/Wizards/roleappswizardpage.h"
#include "Admin_GUI/Views/Wizards/conclusionwizardpage.h"

class StartupWizard : public QWizard
{
   Q_OBJECT
public:
   StartupWizard(ProgramState &loadedDbAdnRolesState, FirstStartSettingsService *appFirstLoadingService, QWidget *parent);
   ~StartupWizard();
   void accept() override;
   void reject() override;
Q_SIGNALS:
   void finish();
private Q_SLOTS:
   void showHelp();
private:
   ProgramState m_programState;
   FirstStartSettingsService *m_firstStartSettingService;
   WizardService *m_wizardService;

   IntroPage *m_introPage;
   UserWizardPage *m_userWizardPage;
   QVector<RoleAppsWizardPage *> m_rolesPages;
   ConclusionWizardPage *m_conclusionPage;

private:
   void CreateServices();
   void CreateUI();
   void InitSizes();
   void InitStyles();
   void InitBehaviour();
   void CreateConnections();

};

#endif // ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H
