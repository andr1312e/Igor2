#ifndef ROLEPAGE_H
#define ROLEPAGE_H

#include <QObject>
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QComboBox>

#include "Services/Settings/wizardservice.h"

#include "Admin_GUI/Views/Wizards/actions.h"
#include "Admin_GUI/Views/Wizards/roleappswizardwidget.h"

class RoleAppsWizardPage : public QWizardPage
{
public:
   RoleAppsWizardPage(const QString &role, int currentRoleIndex, WizardService *service, QWidget *parent);
   ~RoleAppsWizardPage();
   int nextId() const override;
   void initializePage() override;
   QString getUserChoise();

private:

   int m_currentRoleIndex;
   WizardService *m_wizardService;

   QVBoxLayout *m_mainLayout;
   QLabel *m_titleLabel;
   RoleAppsWizardWidget *m_backupWidget;
   RoleAppsWizardWidget *m_oldWidget;

   QComboBox *m_actionComboBox;

private:
   void initUI(const QString &role);
   void insertWidgetsIntoLayout();
   void createConnections();
private:

};

#endif // ROLEPAGE_H
