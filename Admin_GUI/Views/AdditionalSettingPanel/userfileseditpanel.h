#ifndef ADMIN_GUI_VIEWS_USERFILESEDITPANEL_H
#define ADMIN_GUI_VIEWS_USERFILESEDITPANEL_H

#include <QWidget>


#include "Admin_GUI/Views/AdditionalSettingPanel/desktoppanel.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/startuppanel.h"

class UserFilesEditPanel : public QWidget
{

    Q_OBJECT

public:

    UserFilesEditPanel(Terminal *terminal, QWidget *parent);

    ~UserFilesEditPanel();

    void setUserNameAndRole(const QString &userName, const QString &role);

signals:

    void setDefaultRoleApps(const QString &role);
    void roleToViewChanged(const QString &role);

private:

    void initUI(Terminal *terminal);

    void insertWidgetsIntoLayout();

    void createConnections();

private:

    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_topUserNameLayout;

    QHBoxLayout *m_topRoleLayout;

    QLabel *m_userNameLabel;

    QLabel *m_userNameValue;

    QLabel *m_userRoleLabel;

    QLabel *m_userRoleValue;

    DesktopPanel *m_userDesktopPanel;

    StartupPanel *m_userRunnubleApplicationPanel;
};

#endif // ADMIN_GUI_VIEWS_USERFILESEDITPANEL_H
