#ifndef STARTUPWIZARD_H
#define STARTUPWIZARD_H

#include <QWizard>
#include <QLabel>
#include <QLineEdit>

#include "Admin_GUI/Views/Wizards/wizardpages.h"
#include "Admin_GUI/Views/Wizards/intropage.h"
#include "Admin_GUI/Views/Wizards/fcspage.h"

#include "Services/Settings/appfirstloadingsettingsservice.h"
#include "Services/Settings/wizardservice.h"

#include "Services/Settings/programfilesstate.h"
#include "Services/Settings/usersettingsrepository.h"
#include "Services/Settings/roleappsandstartupsettingsrepository.h"

class StartupWizard : public QWizard
{
    Q_OBJECT
public:
    StartupWizard(ProgramFilesState &loadedDbAdnRolesState, AppFirstLoadlingSettingsService *appFirstLoadingService, QWidget *parent);
    ~StartupWizard();
    void accept() override;
    void reject() override;
Q_SIGNALS:
    void finish();
private Q_SLOTS:
    void showHelp();
private:
    ProgramFilesState m_programState;
    AppFirstLoadlingSettingsService *m_settingFileService;
    WizardService *m_wizardService;

    IntroPage *m_introPage;
    FCSPage *m_fcsPage;

private:
    void initServices();
    void initUI();
    void initSizes();
    void initStyles();
    void initBehaviour();
    void createConnections();

};

#endif // STARTUPWIZARD_H
