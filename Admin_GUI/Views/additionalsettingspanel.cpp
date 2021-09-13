#include "additionalsettingspanel.h"

AdditionalSettingsPanel::AdditionalSettingsPanel(Terminal *terminal, QWidget *parent)
    : QWidget(parent)
{
    initUI(terminal);
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

    QPalette userPallete = m_userFilesEditPanel->palette();
    userPallete.setColor(QPalette::Window, QColor(25, 25, 52));
    m_userFilesEditPanel->setPalette(userPallete);
    m_userFilesEditPanel->setAutoFillBackground(true);


    QPalette rolePallete = m_roleEditPanel->palette();
    rolePallete.setColor(QPalette::Window, QColor(25, 25, 25));
    m_roleEditPanel->setObjectName("roleEdit");
    m_roleEditPanel->setPalette(rolePallete);


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
