#include "admingui.h"

Admin_GUI::Admin_GUI(const QString &currentUserId, ISqlDatabaseService *databaseService, LinuxUserService *userService, QWidget *parent)
    :  QWidget(parent)
    ,  m_currentUserId(currentUserId)
    ,  m_databaseService(databaseService)
    ,  m_linuxUserService(userService)
{
    CreateUI(databaseService, userService);
    InsertWidgetsIntoLayout();
    InitTopBar();
    SetWidgetSizes();
    ConnectObjects();
    SetMaximumWidgetSize();
}

Admin_GUI::~Admin_GUI()
{
    delete m_centerSideLayout;
    delete m_leftSideLayout;
    delete m_mainLayout;

    delete m_leftTopBar;
    delete m_usersListWidget;
    delete m_userDesktopPanel;
    delete m_userEditPanel;
    delete m_roleEditPanel;
}

void Admin_GUI::CreateUI(ISqlDatabaseService *databaseService, LinuxUserService *userService)
{
    m_mainLayout = new QHBoxLayout();

    m_leftSideLayout = new QVBoxLayout();
    m_leftTopBar = new TopLeftBar(this);
    m_usersListWidget = new LinuxUsersListWidget(databaseService, userService, this);

    m_centerSideLayout=new QVBoxLayout();
    m_userDesktopPanel=new DesktopPanel(ICONS_PANEL_TYPE::USER_DESKTOP, userService->GetTerminal(), databaseService, this);
    m_userEditPanel = new UserEditPanel(userService->GetCurrentUserName(), userService->GetTerminal(), this);

    m_roleEditPanel = new RoleEditPanel(userService->GetTerminal(), databaseService, this);
}

void Admin_GUI::SetWidgetSizes()
{
//    m_usersListWidget->setMaximumWidth(336);
//    m_userSettingsPanel->setMaximumWidth(450);
}

void Admin_GUI::InsertWidgetsIntoLayout()
{
    m_leftSideLayout->addWidget(m_leftTopBar);
    m_leftSideLayout->addWidget(m_usersListWidget);

    m_centerSideLayout->addWidget(m_userDesktopPanel);
    m_centerSideLayout->addWidget(m_userEditPanel);

    m_mainLayout->addLayout(m_leftSideLayout);
    m_mainLayout->addLayout(m_centerSideLayout);
    m_mainLayout->addWidget(m_roleEditPanel);

    setLayout(m_mainLayout);
}

void Admin_GUI::ConnectObjects()
{
    connect(m_usersListWidget, &LinuxUsersListWidget::ToUserClick, this, &Admin_GUI::OnUserClick);
    connect(m_userEditPanel, &UserEditPanel::ToRoleToViewChanged, m_roleEditPanel, &RoleEditPanel::OnRoleToViewChanged);
    connect(m_userEditPanel, &UserEditPanel::ToDeleteUser, this, &Admin_GUI::OnDeleteUser);
    connect(m_userEditPanel, &UserEditPanel::ToSaveUser, this, &Admin_GUI::OnSaveUser);
    connect(m_roleEditPanel, &RoleEditPanel::ToRoleDesktopChanges, m_userDesktopPanel, &DesktopPanel::OnRoleDesktopChanges);
    connect(this, &Admin_GUI::ToSetDelegateView, m_usersListWidget, &LinuxUsersListWidget::ToSetDelegateView);
}

void Admin_GUI::SetMaximumWidgetSize()
{
//    QScreen *m_currentScreen = QGuiApplication::screenAt(mapToGlobal({width() / 2, 0}));
//    m_maxWidth = m_currentScreen->availableSize().width();
//    m_maxHeight = m_currentScreen->availableSize().height() - 30;
//    this->setMaximumWidth(m_maxWidth);
//    this->setMaximumHeight(m_maxHeight);
}

void Admin_GUI::InitTopBar()
{
    QString currentUserLinuxUserName = m_linuxUserService->GetCurrentUserName();
    int userRole = m_databaseService->GetUserRole(currentUserLinuxUserName);
    if(userRole!=-1)
    {
        QString userFCS = m_databaseService->GetUserFCS(currentUserLinuxUserName);
        QString userRank = m_databaseService->GetUserRank(currentUserLinuxUserName);
        m_leftTopBar->SetData(userRank, userFCS, userRole);
    }
}

void Admin_GUI::OnHideAdditionalSettings(bool state)
{
    if (state) {
        m_roleEditPanel->hide();
//        m_usersListWidget->setMaximumWidth(width() / 2);
//        m_userEditPanel->setMaximumWidth(width() / 2);
//        this->setMinimumSize(m_usersListWidget->minimumWidth() + m_userEditPanel->minimumWidth(), 600);
    } else {
//        if (m_maxWidth > 1713) {
//            m_usersListWidget->setFixedWidth(350);
//            m_userEditPanel->setFixedWidth(450);
//            this->setMinimumSize(1700, 600);
//        } else {
//            m_usersListWidget->setFixedWidth(250);
//            m_userEditPanel->setFixedWidth(350);
//            this->setMinimumSize(500, 600);
//        }
//        this->setMaximumSize(m_maxWidth, m_maxHeight);
        m_roleEditPanel->show();

    }
}

void Admin_GUI::OnUserClick(const User &user)
{
    m_userEditPanel->SetUser(user);
    m_userDesktopPanel->SetUser(user);
    if(user.role!=0)
    {
        m_roleEditPanel->OnRoleToViewChanged(user.role);
    }
}

void Admin_GUI::OnDeleteUser(const QString &userId)
{
    m_usersListWidget->DeleteUser(userId);
    m_userDesktopPanel->DeleteUserAllRoleIcons();
}

void Admin_GUI::OnSaveUser(const QString &userId, const QString &FCS, const QString &rank, const int &oldRole, const int &newRole)
{
    m_usersListWidget->AddUserToModel(userId, FCS, rank, newRole);
    m_roleEditPanel->OnRoleToViewChanged(newRole);
    if(m_currentUserId==userId)
    {
        if(oldRole!=newRole)
        {
            Q_EMIT ToCurrentUserRoleChanged();
        }
        m_leftTopBar->SetData(rank, FCS, newRole);
    }
    //удалить иконки с декскоп панели старые
    //добавить новые иконки с декскоп панели

}
