#ifndef ADMIN_GUI_VIEWS_ROLEEDITPANEL_H
#define ADMIN_GUI_VIEWS_ROLEEDITPANEL_H

#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QListView>
#include <QVBoxLayout>
#include <QPushButton>

#include "Admin_GUI/Views/AdditionalSettingPanel/desktoppanel.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/startuppanel.h"

class RoleEditPanel : public QWidget
{
    Q_OBJECT

public:

    RoleEditPanel(Terminal *terminal, QWidget *parent);

    ~RoleEditPanel();

    void setRoleToViewWithUsers(const QString &role, QStringList *users);

private:

    void initUI(Terminal *terminal);

    void insertWidgetsIntoLayout();

    void setBackGroundColor();

    void createConnections();

signals:

    void roleStartupFileChanged(const QString &role);

private:

    QString m_role;

    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_topLayout;

    QLabel *m_titleLabel;

    QLabel *m_descriptionLabel;

    QLabel *m_currentRole;

    DesktopPanel *m_roleDesktopPanel;

    StartupPanel *m_roleRunningApplicationPanel;
};

#endif // ADMIN_GUI_VIEWS_ROLEINFOPANEL_H
