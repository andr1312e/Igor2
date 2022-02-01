#ifndef ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H
#define ADMIN_GUI_VIEWS_WIZARD_STARTUPWIZARD_H

#include <QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSharedPointer>

#include "Services/linuxuserservice.h"

#include "Admin_GUI/Wizard/Services/wizardservice.h"
#include "Admin_GUI/Wizard/Structs/programfilesstate.h"
#include "Admin_GUI/Wizard/Repositories/usersettingsrepository.h"
#include "Admin_GUI/Wizard/Services/usersprogramiconmakingservice.h"
#include "Admin_GUI/Wizard/Repositories/roleappsandstartupsettingsrepository.h"

#include "Admin_GUI/Wizard/Views/wizardnavigtionbar.h"
#include "Admin_GUI/Wizard/Views/intropage.h"
#include "Admin_GUI/Wizard/Views/wizardpages.h"
#include "Admin_GUI/Wizard/Views/userwizardpage.h"
#include "Admin_GUI/Wizard/Views/roleappswizardpage.h"
#include "Admin_GUI/Wizard/Views/conclusionwizardpage.h"

class StartupWizard : public QWizard
{
   Q_OBJECT
public:
   StartupWizard(const QString &currentUserName, const QString &currentUserId, const QString &applicationName, const QString &rlsTiFolder,const LoadingState &loadedDbAdnRolesState, LinuxUserService *linuxUserService, ISqlDatabaseService *iSqlDataBaseService, QWidget *parent);
   ~StartupWizard();
private:
   void CreateServices(const QString &currentUserName, const QString &currentUserId,const QString &applicationName, const QString &rlsTiFolder, const LoadingState &loadedDbAdnRolesState, LinuxUserService * const linuxUserService, ISqlDatabaseService * const iSqlDataBaseService);
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
