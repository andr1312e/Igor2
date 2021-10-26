#ifndef ADMIN_GUI_VIEWS_WIZARD_ROLEPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_ROLEPAGE_H

#include <QObject>
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QComboBox>

#include "Services/Settings/wizardservice.h"

#include "Admin_GUI/Views/Wizards/actions.h"
#include "Admin_GUI/Views/Wizards/roleappswizardsubwidget.h"

class RoleAppsWizardPage : public QWizardPage
{
public:
   RoleAppsWizardPage(const QString &role, const int currentRoleIndex, WizardService *service, QWidget *parent);
   ~RoleAppsWizardPage();
   int nextId() const override;
   void initializePage() override;
   QString GetUserChoise();

private:

   const int m_currentRoleIndex;
   WizardService *m_wizardService;

   QVBoxLayout *m_mainLayout;
   QLabel *m_titleLabel;
   RoleAppsWizardSubWidget *m_backupWidget;
   RoleAppsWizardSubWidget *m_oldWidget;

   QComboBox *m_actionComboBox;

private:
   void CreateUI(const QString &role);
   void InsertWidgetsIntoLayout();
   void CreateConnections();
};

#endif // ADMIN_GUI_VIEWS_WIZARD_ROLEPAGE_H
