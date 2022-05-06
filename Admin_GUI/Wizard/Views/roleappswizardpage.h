#ifndef ADMIN_GUI_VIEWS_WIZARD_ROLEPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_ROLEPAGE_H

#include <QObject>
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QComboBox>

#include "Admin_GUI/Wizard/Services/wizardservice.h"

#include "Admin_GUI/Wizard/Views/mywizardpage.h"
#include "Admin_GUI/Wizard/Views/actions.h"
#include "Admin_GUI/Wizard/Views/roleappswizardsubwidget.h"

class RoleAppsWizardPage : public MyWizardPage
{
public:
   explicit RoleAppsWizardPage(const QString &role, const int currentRoleIndex, WizardService* const service, QWidget *parent);
   ~RoleAppsWizardPage();

private:
   void CreateUI();
   void InsertWidgetsIntoLayout();
   void InitUI(const QString &role);
   void ConnectObjects();
private Q_SLOTS:
   void OnCurrentIndexChanged(const QString &action);
public:
   virtual int nextId() const Q_DECL_OVERRIDE;
   virtual void initializePage() Q_DECL_OVERRIDE;
   QString GetUserChoise() const;

private:
   const int m_currentRoleIndex;
   WizardService* const m_wizardService;

private:
   QLabel *m_topLabel;
   RoleAppsWizardSubWidget *m_backupWidget;
   RoleAppsWizardSubWidget *m_oldWidget;
   QComboBox *m_actionComboBox;

};

#endif // ADMIN_GUI_VIEWS_WIZARD_ROLEPAGE_H
