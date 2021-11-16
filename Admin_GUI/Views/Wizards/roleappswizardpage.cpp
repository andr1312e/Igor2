#include "roleappswizardpage.h"

RoleAppsWizardPage::RoleAppsWizardPage(const QString &role, const int currentRoleIndex, WizardService *service, QWidget *parent)
   : QWizardPage(parent)
   , m_currentRoleIndex(currentRoleIndex)
   , m_wizardService(service)
{
   CreateUI(role);
   InsertWidgetsIntoLayout();
   ConnectObjects();
}

RoleAppsWizardPage::~RoleAppsWizardPage()
{
   delete m_mainLayout;

   delete m_titleLabel;
   delete m_backupWidget;
   delete m_oldWidget;
   delete m_actionComboBox;

}

int RoleAppsWizardPage::nextId() const
{
   return m_currentRoleIndex + 3;
}

void RoleAppsWizardPage::initializePage()
{
   QStringList currentStartups;
   QList<DesktopEntity> currentDesktops;
   m_actionComboBox->clear();

   if (m_wizardService->HasBackup()) {
      m_wizardService->GetDataFromDesktopRepository(m_currentRoleIndex, false, currentDesktops, currentStartups);
      m_backupWidget->setVisible(true);
      m_backupWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);

      if (m_wizardService->HasOldData()) {

         m_wizardService->GetDataFromDesktopRepository(m_currentRoleIndex, true, currentDesktops, currentStartups);
         m_oldWidget->setVisible(true);
         m_oldWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
         m_actionComboBox->addItems(m_rolesWizardPageComboBoxBackupAndOldDataActions);

      } else {
         m_oldWidget->setVisible(false);
         m_actionComboBox->addItems(m_rolesWizardPageComboBoxBackupActions);
      }

      m_actionComboBox->setCurrentIndex(1);
   } else {
      m_oldWidget->setVisible(true);
      m_backupWidget->setVisible(false);

      if (m_wizardService->HasOldData()) {
         m_wizardService->GetDataFromDesktopRepository(m_currentRoleIndex, false, currentDesktops, currentStartups);
         m_oldWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
         m_actionComboBox->addItems(m_rolesWizardPageComboBoxOldDataActions);
         m_actionComboBox->setCurrentIndex(1);
      } else {
         m_oldWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
         m_actionComboBox->addItems(m_rolesWizardPageComboBoxNoDataActions);
      }
   }
}

QString RoleAppsWizardPage::GetUserChoise()
{
   return m_actionComboBox->currentText();
}

void RoleAppsWizardPage::CreateUI(const QString &role)
{
   m_mainLayout = new QVBoxLayout();
   m_titleLabel = new QLabel("Ярлыки и программы на рабочем столе для роли: " + role);
   m_oldWidget = new RoleAppsWizardSubWidget(" уже имеющихся", nullptr);
   m_backupWidget = new RoleAppsWizardSubWidget(" файла восстановления", nullptr);
   m_actionComboBox = new QComboBox();
}

void RoleAppsWizardPage::InsertWidgetsIntoLayout()
{
   m_mainLayout->addWidget(m_titleLabel);
   m_mainLayout->addWidget(m_oldWidget);
   m_mainLayout->addWidget(m_backupWidget);
   m_mainLayout->addWidget(m_actionComboBox);
   setLayout(m_mainLayout);
}

void RoleAppsWizardPage::ConnectObjects()
{
   connect(m_actionComboBox, &QComboBox::currentTextChanged, [&](const QString & actionWithRoleRepository) {
      m_wizardService->SetActionWithRoleRepository(m_currentRoleIndex, actionWithRoleRepository);
   });
}
