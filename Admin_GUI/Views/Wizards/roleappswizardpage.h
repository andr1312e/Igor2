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

#include "Admin_GUI/Views/Wizards/mywizardpage.h"
#include "Admin_GUI/Views/Wizards/actions.h"
#include "Admin_GUI/Views/Wizards/roleappswizardsubwidget.h"

class RoleAppsWizardPage : public MyWizardPage
{
public:
   RoleAppsWizardPage(const QString &role, const int currentRoleIndex, WizardService* const service, QWidget *parent);
   ~RoleAppsWizardPage();

private:
   void CreateUI();
   void InsertWidgetsIntoLayout();
   void InitUI(const QString &role);
   void ConnectObjects();

public:
   virtual int nextId() const Q_DECL_OVERRIDE;
   virtual void initializePage() Q_DECL_OVERRIDE;
   QString GetUserChoise() const;

private:
   const int m_currentRoleIndex;
   WizardService* const m_wizardService;

private:
   QLabel *m_titleLabel;
   RoleAppsWizardSubWidget *m_backupWidget;
   RoleAppsWizardSubWidget *m_oldWidget;
   QComboBox *m_actionComboBox;

};

#endif // ADMIN_GUI_VIEWS_WIZARD_ROLEPAGE_H
