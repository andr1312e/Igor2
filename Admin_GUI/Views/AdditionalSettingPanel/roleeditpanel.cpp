#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QPushButton>

#include "roleeditpanel.h"

RoleEditPanel::RoleEditPanel(ISqlDatabaseService *sqlDatabaseService, RoleDesktopService * roleDesktopService,  QWidget *parent)
    : QWidget(parent)
{
    CreateUI(sqlDatabaseService, roleDesktopService);
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

RoleEditPanel::~RoleEditPanel()
{
    delete m_topLayout;
    delete m_mainLayout;

    delete m_titleLabel;
    delete m_descriptionLabel;
    delete m_currentRoleLabel;

    delete m_roleDesktopPanel;
    delete m_roleRunningApplicationPanel;
}

void RoleEditPanel::OnRoleToViewChanged(const int &roleId)
{
    if(roleId>=0 && roleId<Roles.count())
    {
        QString m_role=Roles.at(roleId);
        quint8 roleIdVal=(quint8)roleId;
        m_currentRoleLabel->setText(QStringLiteral("Выбранная роль: ")+ m_role);
        m_roleDesktopPanel->SetRoleId(roleIdVal);
        m_roleRunningApplicationPanel->SetRoleId(roleIdVal);
    }
}


void RoleEditPanel::CreateUI(ISqlDatabaseService *sqlDatabaseService, RoleDesktopService *roleDesktopService)
{
    m_mainLayout=new QVBoxLayout();
    m_topLayout=new QHBoxLayout();

    m_titleLabel=new QLabel();
    m_currentRoleLabel=new QLabel();
    m_descriptionLabel=new QLabel();

    m_roleDesktopPanel=new DesktopPanel(ICONS_PANEL_TYPE::ROLE_ICONS, Q_NULLPTR, roleDesktopService,  this);
    m_roleRunningApplicationPanel=new StartupPanel(sqlDatabaseService, this);
}

void RoleEditPanel::InsertWidgetsIntoLayout()
{
    m_topLayout->addWidget(m_titleLabel);
    m_topLayout->addStretch(4);
    m_topLayout->addWidget(m_currentRoleLabel);
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addWidget(m_descriptionLabel);
    m_mainLayout->addWidget(m_roleDesktopPanel);
    m_mainLayout->addWidget(m_roleRunningApplicationPanel);
    setLayout(m_mainLayout);
}

void RoleEditPanel::FillUI()
{
    m_titleLabel->setText("Панель изменения ролей");
    m_currentRoleLabel->setText("Выберите роль для просмотра настроек");
    m_descriptionLabel->setText("Изменения применятся ко всем пользователям с этой ролью");
    m_descriptionLabel->setStyleSheet("font-weight: bold;");
    m_descriptionLabel->setAlignment(Qt::AlignHCenter);
}

void RoleEditPanel::ConnectObjects()
{
//    connect(m_roleRunningApplicationPanel, &StartupPanel::ToRoleStartupChanges, this, &RoleEditPanel::ToRoleDesktopChanges);
}
