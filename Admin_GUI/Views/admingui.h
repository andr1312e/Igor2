#ifndef ADMIN_GUI_VIEWS_ADMIN_GUI_H
#define ADMIN_GUI_VIEWS_ADMIN_GUI_H

#include <QLabel>
#include <QDebug>
#include <QScreen>

#include "Logging/logger.h"

#include "Services/Sql/isqlservice.h"
#include "Services/linuxuserservice.h"

#include "Admin_GUI/Model/usermodel.h"

#include "Admin_GUI/Views/topleftbar.h"
#include "Admin_GUI/Views/linuxuserslistwidget.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/desktoppanel.h"
#include "Admin_GUI/Views/SettingsPanel/usereditpanel.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/roleeditpanel.h"

class Admin_GUI : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_GUI(ISqlDatabaseService *databaseService, LinuxUserService *userService, QStringView currentAdminId, QStringView currentUserName, QWidget *parent);
    ~Admin_GUI();

private:
    void CreateModel();
    void CreateServices();
    void CreateUI(QStringView currentUserName);
    void InitTopBar(QStringView currentUserName);
    void InsertWidgetsIntoLayout();
    void ConnectObjects();

Q_SIGNALS:
    void ToChangeTheme(bool isDarkTheme);
    void ToCurrentUserRoleChanged();
private Q_SLOTS:
    void OnUserClick(const User &user);
    void OnDeleteUser(const QString &userId, const QString &userName);
    void OnSaveUser(const QString &userId, const QString &userName, const QString &FCS, int oldRole, int newRole);
private:
    int m_maxWidth;
    int m_maxHeight;
    const QStringView m_currentAdminId;

    ISqlDatabaseService *const m_sqlDatabaseService;
    LinuxUserService *const m_linuxUserService;
    UserDesktopService *m_userDesktopService;
    RoleDesktopService *m_roleDesktopService;
    UserModel *m_userModel;

private:
    QHBoxLayout *m_mainLayout;

    QVBoxLayout *m_leftSideLayout;
    TopLeftBar *m_leftTopBar;
    LinuxUsersListWidget *m_usersListWidget;

    QVBoxLayout *m_centerSideLayout;
    DesktopPanel *m_userDesktopPanel;
    UserEditPanel *m_userEditPanel;

    RoleEditPanel *m_roleEditPanel;
};

#endif // ADMIN_GUI_VIEWS_ADMIN_GUI_H
