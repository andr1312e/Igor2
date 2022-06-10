#include "admingui.h"

Admin_GUI::Admin_GUI(ISqlDatabaseService *databaseService, LinuxUserService *userService, QStringView currentAdminId, QStringView currentUserName, QWidget *parent)
    :  QWidget(parent)
    ,  m_currentAdminId(currentAdminId)
    ,  m_sqlDatabaseService(databaseService)
    ,  m_linuxUserService(userService)
{
    CreateModel();
    CreateServices();
    CreateUI(currentUserName);
    InsertWidgetsIntoLayout();
    InitTopBar(currentUserName);
    ConnectObjects();
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

void Admin_GUI::CreateModel()
{
    m_userModel = new UserModel(m_sqlDatabaseService, m_linuxUserService);
}

void Admin_GUI::CreateServices()
{
    m_userDesktopService = new UserDesktopService(m_sqlDatabaseService);
    m_roleDesktopService = new RoleDesktopService(m_sqlDatabaseService);
}

void Admin_GUI::CreateUI(QStringView currentUserName)
{
    m_mainLayout = new QHBoxLayout();

    m_leftSideLayout = new QVBoxLayout();
    m_leftTopBar = new TopLeftBar(this);
    m_usersListWidget = new LinuxUsersListWidget(m_userModel, this);

    m_centerSideLayout = new QVBoxLayout();
    m_userDesktopPanel = new DesktopPanel(IconsPanelType::UserIcons, m_userDesktopService, Q_NULLPTR, this);
    m_userEditPanel = new UserEditPanel(currentUserName, this);

    m_roleEditPanel = new RoleEditPanel(m_sqlDatabaseService, m_roleDesktopService, this);
}

void Admin_GUI::InsertWidgetsIntoLayout()
{
    m_leftSideLayout->addWidget(m_leftTopBar);
    m_leftSideLayout->addWidget(m_usersListWidget);

    m_centerSideLayout->addWidget(m_userDesktopPanel, 2);
    m_centerSideLayout->addWidget(m_userEditPanel);

    m_mainLayout->addLayout(m_leftSideLayout);
    m_mainLayout->addLayout(m_centerSideLayout);
    m_mainLayout->addWidget(m_roleEditPanel);
    m_mainLayout->setContentsMargins(15, 10, 15, 10);
    setLayout(m_mainLayout);
}

void Admin_GUI::ConnectObjects()
{
    connect(m_usersListWidget, &LinuxUsersListWidget::ToUserClick, this, &Admin_GUI::OnUserClick);
    connect(m_userEditPanel, &UserEditPanel::ToRoleToViewChanged, m_roleEditPanel, &RoleEditPanel::OnRoleToViewChanged);
    connect(m_userEditPanel, &UserEditPanel::ToDeleteUser, this, &Admin_GUI::OnDeleteUser);
    connect(m_userEditPanel, &UserEditPanel::ToSaveUser, this, &Admin_GUI::OnSaveUser);
}

/**
 * Запись роли и фио в топ бар
 * Если роль не валидна то не заполняем, иначе берем фамилию и пишем
 */
void Admin_GUI::InitTopBar(QStringView currentUserName)
{
    const int roleId = m_sqlDatabaseService->GetUserRole(currentUserName);
    if (User::RoleIsValid(roleId))
    {
        const QString userFCS = m_sqlDatabaseService->GetUserFCS(currentUserName);
        m_leftTopBar->SetData(userFCS, roleId);
    }
}
/**
 * Слот на клик по юзеру
 * Записываем в сред ниж панель, берем ярлыки раб стола и в зависимости от валид роли, заполняем вкладку роли
 */
void Admin_GUI::OnUserClick(const User &user)
{
    m_userEditPanel->SetUser(user);
    m_userDesktopPanel->SetUser(user);
    if (user.RoleIsValid())
    {
        m_roleEditPanel->OnRoleToViewChanged(user.GetUserRole());
    }
}

void Admin_GUI::OnDeleteUser(const QString &userId, const QString &userName)
{
    const int roleId = m_userModel->GetRoleIdByUserId(userId);
    m_userModel->DeleteUser(userId);
    if (User::RoleIsValid(roleId))
    {
        m_userDesktopService->DeleteAllIconsToUser(roleId, userName);
    }
    m_userDesktopService->GetAllUserDesktops(userName);
}

void Admin_GUI::OnSaveUser(const QString &userId, const QString &userName, const QString &FCS, int oldRole, int newRole)
{
    m_userModel->AddUserToModel(userId, userName, FCS, newRole);
    m_roleEditPanel->OnRoleToViewChanged(newRole);
    m_roleDesktopService->SetDefaultIconsToUserOnUserRoleUpdate(oldRole, newRole, userName);
    if (userId == m_currentAdminId)
    {
        if (oldRole != newRole)
        {
            Q_EMIT ToCurrentUserRoleChanged();
        }
        m_leftTopBar->SetData(FCS, newRole);
    }
    m_userDesktopService->GetAllUserDesktops(userName);
}
