#ifndef ADMIN_GUI_VIEWS_WIZARD_CONCLUSIONWIZARDPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_CONCLUSIONWIZARDPAGE_H

#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QWizardPage>

#include "Structs/userstruct.h"

#include "Services/Settings/wizardservice.h"
#include "Services/Settings/usersprogramiconmakingservice.h"

#include "Admin_GUI/Views/Wizards/mywizardpage.h"
#include "Admin_GUI/Views/Wizards/actions.h"

class ConclusionWizardPage: public MyWizardPage
{
    Q_OBJECT
public:
    ConclusionWizardPage(WizardService *service, UsersProgramIconMakingService *iconMaker, QWidget *parent);
    ~ConclusionWizardPage();

private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void ConnectObjects();

public:
    virtual int nextId() const Q_DECL_OVERRIDE;
    virtual void initializePage() Q_DECL_OVERRIDE;

private:
    void SetUserActionValueLabel(const QString &actionWithUserRepository);
    void SetRolesActionValueLabel(int &roleIndex, const QString &actionWithRoleReposiory);

private:
    WizardService* const m_wizardService;
    UsersProgramIconMakingService* const m_iconMakerSerivce;

private:
    QLabel *m_titleLabel;
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

    QLabel *m_additionalActionsLabel;
    QCheckBox *m_addIconToDesktopToCurrentUser;
    QCheckBox *m_addIconToDesktopToAllUsers;
    QCheckBox *m_addIconToStartMenu;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_CONCLUSIONWIZARDPAGE_H
