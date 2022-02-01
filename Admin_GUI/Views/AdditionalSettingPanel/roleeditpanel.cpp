#include "roleeditpanel.h"

RoleEditPanel::RoleEditPanel(ISqlDatabaseService *sqlDatabaseService, RoleDesktopService * roleDesktopService,  QWidget *parent)
    : QWidget(parent)
{
    CreateUI(sqlDatabaseService, roleDesktopService);
    InsertWidgetsIntoLayout();
    FillUI();
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
    if(Log4Qt::Logger::rootLogger()->HasAppenders())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Хотим установить роль :") + QString::number(roleId));
    }
    if(roleId>=0 && roleId<Roles.count())
    {
        const QString currentRoleName=Roles.at(roleId);
        m_currentRoleLabel->setText(QStringLiteral("Выбранная роль: ")+ currentRoleName);
        m_roleDesktopPanel->SetRoleId(roleId);
        m_roleRunningApplicationPanel->SetRoleId(roleId);
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
    m_titleLabel->setText(QStringLiteral("Панель изменения ролей"));
    m_currentRoleLabel->setText(QStringLiteral("Выберите роль для просмотра настроек"));
    m_descriptionLabel->setText(QStringLiteral("Изменения применятся ко всем пользователям с этой ролью"));
    m_descriptionLabel->setStyleSheet(QStringLiteral("font-weight: bold;"));
    m_descriptionLabel->setAlignment(Qt::AlignHCenter);
}
