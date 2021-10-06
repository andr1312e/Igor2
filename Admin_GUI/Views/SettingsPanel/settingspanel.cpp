#include "settingspanel.h"
#include <QDebug>


SettingsPanel::SettingsPanel(const QString &userName, DatabaseService *repository, QWidget *parent)
    : QWidget(parent)
{
    initUI(userName, repository);
    insertWidgetsIntoLayout();
    createConnections();
}

SettingsPanel::~SettingsPanel()
{
    delete m_mainLayout;

    delete m_userInfoPanel;
    delete m_userEditPanel;
}

void SettingsPanel::initUI(const QString &userName, DatabaseService *repository)
{

    m_mainLayout=new QVBoxLayout();
    m_userInfoPanel=new UserInfoPanel(repository->getTerminal(), this);
    m_userEditPanel=new UserEditPanel(userName, repository, this);
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
