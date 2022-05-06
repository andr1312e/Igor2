#ifndef ADMIN_GUI_VIEWS_WIZARD_CONCLUSIONWIZARDPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_CONCLUSIONWIZARDPAGE_H

#include <QLabel>
#include <QCheckBox>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWizardPage>
#include <QProgressBar>

#include "Structs/userstruct.h"

#include "Admin_GUI/Wizard/Services/wizardservice.h"
#include "Admin_GUI/Wizard/Services/usersprogramiconmakingservice.h"

#include "Admin_GUI/Wizard/Views/mywizardpage.h"
#include "Admin_GUI/Wizard/Views/actions.h"

class ConclusionWizardPage: public MyWizardPage
{
    Q_OBJECT
public:
    explicit ConclusionWizardPage(WizardService *service, IconMaker *iconMaker, QWidget *parent);
    ~ConclusionWizardPage();

private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void ConnectObjects();

private Q_SLOTS:
    void OnCurrentActionDoing(const QString &actionDescription);
public:
    virtual int nextId() const Q_DECL_OVERRIDE;
    virtual void initializePage() Q_DECL_OVERRIDE;

private:
    void SetUserActionValueLabel(int actionIndexWithUserRepository);
    void SetRolesActionValueLabel(int roleIndex, int actionIndexWithRoleReposiory);
    void SetFutureInstalledPackagesToList();

private:
    WizardService* const m_wizardService;
    IconMaker* const m_iconMakerSerivce;

private:
    QLabel *m_topLabel;
    QLabel *m_descriptionLabel;
    QHBoxLayout *m_backupLayout;
    QLabel *m_backupLabel;
    QLabel *m_backupValue;
    QHBoxLayout *m_oldDataLayout;
    QLabel *m_oldDataLabel;
    QLabel *m_oldDataValue;

    QLabel *m_actionsLabel;

    QGridLayout *m_dataLayout;

    QLabel *m_usersLabel;
    QLabel *m_usersActionValue;
    QLabel *m_usersCommentLabel;
    QLabel *m_usersCommentValue;

    QLabel *m_firstRoleLabel;
    QLabel *m_firstRoleActionValue;
    QLabel *m_firstRoleCommentLabel;
    QLabel *m_firstRoleCommentValue;

    QLabel *m_secondRoleLabel;
    QLabel *m_secondRoleActionValue;
    QLabel *m_secondRoleCommentLabel;
    QLabel *m_secondRoleCommentValue;

    QLabel *m_thirdRoleLabel;
    QLabel *m_thirdRoleActionValue;
    QLabel *m_thirdRoleCommentLabel;
    QLabel *m_thirdRoleCommentValue;

    QLabel *m_fourthRoleLabel;
    QLabel *m_fourthRoleActionValue;
    QLabel *m_fourthRoleCommentLabel;
    QLabel *m_fourthRoleCommentValue;

    QLabel *m_packagesToInstallLabel;
    QListWidget *m_packagesToInstallList;

    QLabel *m_additionalActionsLabel;
    QCheckBox *m_needInstallPackages;
    QCheckBox *m_addIconToDesktopToCurrentUser;
    QCheckBox *m_addIconToDesktopToAllUsers;
    QCheckBox *m_addIconToStartMenu;

    QHBoxLayout *m_installingPackesLayout;
    QLabel *m_currentDoingActionLabel;
    QLabel *m_currentDoingActionName;
    QProgressBar *m_totalProgressBar;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_CONCLUSIONWIZARDPAGE_H
