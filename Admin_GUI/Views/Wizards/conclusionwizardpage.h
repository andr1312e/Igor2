#ifndef CONCLUSIONWIZARDPAGE_H
#define CONCLUSIONWIZARDPAGE_H

#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>

#include "Structs/userstruct.h"

#include "Services/Settings/wizardservice.h"

#include "Admin_GUI/Views/Wizards/actions.h"

class ConclusionWizardPage: public QWizardPage
{
   Q_OBJECT
public:
   ConclusionWizardPage(WizardService *service, QWidget *parent);
   ~ConclusionWizardPage();
   int nextId() const override;
   void initializePage() override;
private:
   WizardService *m_wizardService;

   QVBoxLayout *m_mainLayout;
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

private:
   void initUI();
   void insertWidgetsIntoLayout();
   void createConnections();
private:
   void setUserLabel(QString &actionWithUserRepository);
   void setRolesLabel(int &roleIndex, const QString &actionWithRoleReposiory);
};

#endif // CONCLUSIONWIZARDPAGE_H
