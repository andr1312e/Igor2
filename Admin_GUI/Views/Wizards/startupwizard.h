#ifndef STARTUPWIZARD_H
#define STARTUPWIZARD_H

#include <QWizard>
#include <QLabel>
#include <QLineEdit>

#include "Admin_GUI/Views/Wizards/wizardpages.h"
#include "Admin_GUI/Views/Wizards/intropage.h"
#include "Admin_GUI/Views/Wizards/fcspage.h"

#include "Services/Settings/usersettingsrepository.h"
#include "Services/Settings/roleappsandstartupsettingsrepository.h"

class StartupWizard : public QWizard
{
    Q_OBJECT
public:
    StartupWizard(int state, QString &currentUserName, QString &currentUserId, QStringList params, QWidget *parent);
    ~StartupWizard();
private Q_SLOTS:
    void showHelp();
private:

    QDomDocument *m_backupDocument;

    CurrentUserWizardRepository *m_userWizardRepository;
    RoleAndStartupWizardRepository *m_roleWizardRepostory;
    IntroPage *m_introPage;
    FCSPage *m_fcsPage;

private:
    void initServices(CurrentUserWizardRepository *userWizardRepository, RoleAndStartupWizardRepository *roleWizardRepostory);
    void initUI();
    void initSizes();
    void initStyles();
    void initBehaviour();
    void createConnections();
};

#endif // STARTUPWIZARD_H
