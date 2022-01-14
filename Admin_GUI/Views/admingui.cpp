#include "admingui.h"

Admin_GUI::Admin_GUI(ISqlDatabaseService *databaseService, LinuxUserService *userService, QStringView currentAdminId, QStringView currentUserName, QWidget *parent)
    :  QWidget(parent)
    ,  m_currentAdminId(currentAdminId)
    ,  m_databaseService(databaseService)
    ,  m_linuxUserService(userService)
{
    CreateModel(databaseService, userService);
    CreateServices(databaseService);
    CreateUI(databaseService, currentUserName);
    InsertWidgetsIntoLayout();
    InitTopBar(currentUserName);
    ConnectObjects();
    SetMaximumWidgetSize();
}

Admin_GUI::~Admin_GUI()
{
    delete m_userDesktopService;
    delete m_roleDesktopService;

    delete m_centerSideLayout;
    delete m_leftSideLayout;
    delete m_mainLayout;

    delete m_leftTopBar;
    delete m_usersListWidget;
    delete m_userDesktopPanel;
    delete m_userEditPanel;
    delete m_roleEditPanel;
}

void Admin_GUI::CreateModel(ISqlDatabaseService *databaseService, LinuxUserService *userService)
{
    m_userModel=new UserModel(databaseService, userService);
}

void Admin_GUI::CreateServices(ISqlDatabaseService *databaseService)
{
    m_userDesktopService=new UserDesktopService(databaseService);
    m_roleDesktopService=new RoleDesktopService(databaseService);
}

void Admin_GUI::CreateUI(ISqlDatabaseService *databaseService, QStringView currentUserName)
{
    m_mainLayout = new QHBoxLayout();

    m_leftSideLayout = new QVBoxLayout();
    m_leftTopBar = new TopLeftBar(this);
    m_usersListWidget = new LinuxUsersListWidget(m_userModel, this);

    m_centerSideLayout=new QVBoxLayout();
    m_userDesktopPanel=new DesktopPanel(ICONS_PANEL_TYPE::USER_ICONS, m_userDesktopService, Q_NULLPTR, this);
    m_userEditPanel = new UserEditPanel(currentUserName, this);

    m_roleEditPanel = new RoleEditPanel(databaseService, m_roleDesktopService, this);
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

void Admin_GUI::InitTopBar(QStringView currentUserName)
{
    const int userRole = m_databaseService->GetUserRole(currentUserName);
    if(userRole!=-1)
    {
        const QString userFCS = m_databaseService->GetUserFCS(currentUserName);
        const QString userRank = m_databaseService->GetUserRank(currentUserName);
        m_leftTopBar->SetData(userRank, userFCS, userRole);
    }
}

void Admin_GUI::OnHideAdditionalSettings(bool state)
{
    if (state) {
        m_roleEditPanel->hide();
    } else {
        m_roleEditPanel->show();
    }
}

void Admin_GUI::OnUserClick(const User &user)
{
    m_userEditPanel->SetUser(user);
    m_userDesktopPanel->SetUser(user);
    if(user.role>0 && user.role<Roles.count())
    {
        m_roleEditPanel->OnRoleToViewChanged(user.role);
    }
}

void Admin_GUI::OnDeleteUser(const QString &userId, const QString &userName)
{
    const int roleId=m_userModel->GetRoleIdByUserId(userId);
    m_userModel->DeleteUser(userId);
    if(roleId>=0)
    {
         m_userDesktopService->DeleteAllIconsToUser(roleId, userName);
    }
    m_userDesktopService->GetAllUserDesktops(userName);
}

void Admin_GUI::OnSaveUser(const QString &userId, const QString &userName, const QString &FCS, const QString &rank, const int &oldRole, const int &newRole)
{
    m_userModel->AddUserToModel(userId, userName, FCS, rank, newRole);
    m_roleEditPanel->OnRoleToViewChanged(newRole);
    m_roleDesktopService->SetDefaultIconsToUserOnUserRoleUpdate(oldRole, newRole, userName);
    if(userId==m_currentAdminId)
    {
        if(oldRole!=newRole)
        {
            Q_EMIT ToCurrentUserRoleChanged();
        }
        m_leftTopBar->SetData(rank, FCS, newRole);
    }
    m_userDesktopService->GetAllUserDesktops(userName);
}
