#ifndef ADMIN_GUI_VIEWS_ADMIN_GUI_H
#define ADMIN_GUI_VIEWS_ADMIN_GUI_H

#include <QLabel>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>

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

    Admin_GUI(const QString &currentUserId, ISqlDatabaseService *databaseService, LinuxUserService *userService, QWidget *parent);
    ~Admin_GUI();

private:
    void CreateUI(ISqlDatabaseService *databaseService, LinuxUserService *userService);
    void InsertWidgetsIntoLayout();
    void SetWidgetSizes();
    void ConnectObjects();
    void SetMaximumWidgetSize();

Q_SIGNALS:
    void ToChangeTheme(bool isDarkTheme);
    void ToSetDelegateView(bool state);
    void ToCurrentUserRoleChanged();
public Q_SLOTS:
    void OnHideAdditionalSettings(bool state);
private Q_SLOTS:
    void OnUserClick(const User &user);
    void OnDeleteUser(const QString &userId);
    void OnSaveUser(const QString&userId, const QString &FCS, const QString &rank, const int&oldRole, const int &newRole);
private:
    void InitTopBar();

private:
    int m_maxWidth;
    int m_maxHeight;
    const QString &m_currentUserId;
private:
    ISqlDatabaseService* const m_databaseService;
    LinuxUserService* const m_linuxUserService;

    QHBoxLayout *m_mainLayout;

    QVBoxLayout *m_leftSideLayout;
    TopLeftBar *m_leftTopBar;
    LinuxUsersListWidget *m_usersListWidget;

    QVBoxLayout *m_centerSideLayout;
    UserEditPanel *m_userEditPanel;
    DesktopPanel *m_userDesktopPanel;

    RoleEditPanel *m_roleEditPanel;
};

#endif // ADMIN_GUI_VIEWS_ADMIN_GUI_H
