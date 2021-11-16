#include "roleeditpanel.h"

RoleEditPanel::RoleEditPanel(Terminal *terminal, QWidget *parent)
    : QWidget(parent)
{
    CreateUI(terminal);
    InsertWidgetsIntoLayout();
    ConnectObjects();
}

RoleEditPanel::~RoleEditPanel()
{
    delete m_topLayout;
    delete m_mainLayout;

    delete m_titleLabel;
    delete m_descriptionLabel;
    delete m_currentRole;
    delete m_roleDesktopPanel;
    delete m_roleRunningApplicationPanel;
}

void RoleEditPanel::setRoleToViewWithUsers(const QString &role, QStringList *users)
{
    m_role=role;
    m_currentRole->setText("Выбранная роль: "+ m_role);
    m_roleDesktopPanel->SetParam(role, users);
    m_roleRunningApplicationPanel->setParam(role, users);
}


void RoleEditPanel::CreateUI(Terminal *terminal)
{
    m_mainLayout=new QVBoxLayout();
    m_topLayout=new QHBoxLayout();

    m_titleLabel=new QLabel("Панель изменения ролей");
    m_currentRole=new QLabel("Выберите роль для просмотра настроек");
    m_descriptionLabel=new QLabel("Изменения применятся ко всем пользователям с этой ролью");
    m_descriptionLabel->setStyleSheet("font-weight: bold;");
    m_descriptionLabel->setAlignment(Qt::AlignHCenter);

    m_roleDesktopPanel=new DesktopPanel(terminal, ICONS_PANEL_TYPE::ROLE_DESKTOP,  this);
    m_roleRunningApplicationPanel=new StartupPanel(terminal, STARTUP_PANEL_TYPE::ROLE_APPS, this);
}

void RoleEditPanel::InsertWidgetsIntoLayout()
{
    m_topLayout->addWidget(m_titleLabel);
    m_topLayout->addStretch(4);
    m_topLayout->addWidget(m_currentRole);
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addWidget(m_descriptionLabel);
    m_mainLayout->addWidget(m_roleDesktopPanel);
    m_mainLayout->addWidget(m_roleRunningApplicationPanel);
    setLayout(m_mainLayout);
}

void RoleEditPanel::SetBackGroundColor()
{
//    m_roleDesktopPanel->setBackgroundRole(QPalette::Base);
//    m_roleDesktopPanel->setAutoFillBackground(true);

//    m_roleRunningApplicationPanel->setBackgroundRole(QPalette::Base);
//    m_roleRunningApplicationPanel->setAutoFillBackground(true);
}

void RoleEditPanel::ConnectObjects()
{
    connect(m_roleRunningApplicationPanel, &StartupPanel::ToRoleStartupFileChanging, [=](){emit ToRoleStartupFileChanging(m_role);});
}
