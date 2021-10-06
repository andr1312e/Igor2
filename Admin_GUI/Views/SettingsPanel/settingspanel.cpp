#include "settingspanel.h"
#include <QDebug>


SettingsPanel::SettingsPanel(const QString &userName, UserModel *model, Terminal *terminal, QWidget *parent)
    : QWidget(parent)
{
    initUI(userName, model, terminal);
    insertWidgetsIntoLayout();
    createConnections();
}

SettingsPanel::~SettingsPanel()
{
    delete m_mainLayout;

    delete m_userInfoPanel;
    delete m_userEditPanel;
}

void SettingsPanel::initUI(const QString &userName, UserModel *model, Terminal *terminal)
{

    m_mainLayout=new QVBoxLayout();
    m_userInfoPanel=new UserInfoPanel(terminal, this);
    m_userEditPanel=new UserEditPanel(userName, model, terminal, this);
}

void SettingsPanel::applyStyle()
{

}

void SettingsPanel::insertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_userInfoPanel);
    m_mainLayout->addWidget(m_userEditPanel);
    setLayout(m_mainLayout);
}

void SettingsPanel::createConnections()
{
    connect(m_userEditPanel, &UserEditPanel::clearUserAdditionalInfo, m_userInfoPanel, &UserInfoPanel::clearAdditionalInfo);
    connect(m_userEditPanel, &UserEditPanel::fillUserAdditionalInfo, m_userInfoPanel, &UserInfoPanel::fillUserAdditionalInfo);
    connect(m_userEditPanel, &UserEditPanel::setDefaultRoleApps, this, &SettingsPanel::setDefaultRoleApps);
    connect(m_userEditPanel, &UserEditPanel::roleToViewChanged, this, &SettingsPanel::roleToViewChanged);
}

void SettingsPanel::setUser(User &user)
{
    m_userInfoPanel->setUser(user);
    m_userEditPanel->setUser(user);
}
