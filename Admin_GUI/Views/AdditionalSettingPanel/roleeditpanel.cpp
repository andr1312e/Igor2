#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QPushButton>

#include "roleeditpanel.h"

RoleEditPanel::RoleEditPanel(ISqlDatabaseService *sqlDatabaseService, RoleDesktopService *roleDesktopService,  QWidget *parent)
    : QWidget(parent)
{
    CreateUI(sqlDatabaseService, roleDesktopService);
    InsertWidgetsIntoLayout();
}

RoleEditPanel::~RoleEditPanel()
{
    delete m_topLayout;
    delete m_mainLayout;

    delete m_roleDesktopPanel;
    delete m_roleRunningApplicationPanel;
}

void RoleEditPanel::OnRoleToViewChanged(int roleId)
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Хотим установить роль :") + QString::number(roleId));
    if (User::RoleIsValid(roleId))
    {
        const QString currentRoleName = Roles.at(roleId);
        m_roleDesktopPanel->SetRoleId(roleId);
        m_roleRunningApplicationPanel->SetRoleId(roleId);
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" ид роли больше чем их общее колличество ")).toUtf8().constData());
    }
}


void RoleEditPanel::CreateUI(ISqlDatabaseService *sqlDatabaseService, RoleDesktopService *roleDesktopService)
{
    m_mainLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();
    m_roleDesktopPanel = new DesktopPanel(IconsPanelType::RoleIcons, Q_NULLPTR, roleDesktopService,  this);
    m_roleRunningApplicationPanel = new StartupPanel(sqlDatabaseService, this);
}

void RoleEditPanel::InsertWidgetsIntoLayout()
{
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addWidget(m_roleDesktopPanel);
    m_mainLayout->addWidget(m_roleRunningApplicationPanel);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_mainLayout);
}
