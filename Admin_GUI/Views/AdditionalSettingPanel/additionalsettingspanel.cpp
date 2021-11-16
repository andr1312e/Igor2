#include "additionalsettingspanel.h"

AdditionalSettingsPanel::AdditionalSettingsPanel(Terminal *terminal, QWidget *parent)
   : QWidget(parent)
{
   CreateUI(terminal);
   SetBackGroundColor();
   InsertWidgetsIntoLayout();
   ConnectObjects();
}

AdditionalSettingsPanel::~AdditionalSettingsPanel()
{
   delete m_mainLayout;
   delete m_userFilesEditPanel;
   delete m_roleEditPanel;
}

void AdditionalSettingsPanel::setRoleEditPanelData(const QString &role, QStringList *users)
{
   m_roleEditPanel->setRoleToViewWithUsers(role, users);
}

void AdditionalSettingsPanel::setUserFilesEditPanelData(const QString &name, const QString &role)
{
   m_userFilesEditPanel->SetUserNameAndRole(name, role);
}

void AdditionalSettingsPanel::CreateUI(Terminal *terminal)
{
   m_mainLayout = new QHBoxLayout();

   m_userFilesEditPanel = new UserFilesEditPanel(terminal, this);

   m_roleEditPanel = new RoleEditPanel(terminal, this);
}

void AdditionalSettingsPanel::SetBackGroundColor()
{
   m_userFilesEditPanel->setBackgroundRole(QPalette::AlternateBase);
   m_userFilesEditPanel->setAutoFillBackground(true);
   m_roleEditPanel->setBackgroundRole(QPalette::AlternateBase);
   m_roleEditPanel->setAutoFillBackground(true);
}

void AdditionalSettingsPanel::InsertWidgetsIntoLayout()
{
   m_mainLayout->addWidget(m_userFilesEditPanel);
   m_mainLayout->addWidget(m_roleEditPanel);

   setLayout(m_mainLayout);
}

void AdditionalSettingsPanel::ConnectObjects()
{
   connect(this, &AdditionalSettingsPanel::ToSetDefaultRoleApps, m_userFilesEditPanel, &UserFilesEditPanel::ToSetDefaultRoleApps);
}
