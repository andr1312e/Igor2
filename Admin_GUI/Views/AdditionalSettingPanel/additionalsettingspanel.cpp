#include "additionalsettingspanel.h"

AdditionalSettingsPanel::AdditionalSettingsPanel(Terminal *terminal, QWidget *parent)
    : QWidget(parent)
{
    initUI(terminal);
    setBackGroundColor();
    insertWidgetsIntoLayout();
    createConnections();
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
    m_userFilesEditPanel->setUserNameAndRole(name, role);
}

void AdditionalSettingsPanel::initUI(Terminal *terminal)
{
    m_mainLayout=new QHBoxLayout();

    m_userFilesEditPanel=new UserFilesEditPanel(terminal, this);

    m_roleEditPanel=new RoleEditPanel(terminal, this);
}

void AdditionalSettingsPanel::setBackGroundColor()
{
    m_userFilesEditPanel->setBackgroundRole(QPalette::Base);
    m_userFilesEditPanel->setAutoFillBackground(true);
    m_roleEditPanel->setBackgroundRole(QPalette::Base);
    m_roleEditPanel->setAutoFillBackground(true);
}

void AdditionalSettingsPanel::insertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_userFilesEditPanel);
    m_mainLayout->addWidget(m_roleEditPanel);

    setLayout(m_mainLayout);
}

void AdditionalSettingsPanel::createConnections()
{
    connect(this, &AdditionalSettingsPanel::setDefaultRoleApps, m_userFilesEditPanel, &UserFilesEditPanel::setDefaultRoleApps);
}
