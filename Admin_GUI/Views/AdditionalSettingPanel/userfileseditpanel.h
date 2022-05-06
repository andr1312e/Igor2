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

Q_SIGNALS:

   void ToSetDefaultRoleApps(const QString &role);
   void ToRoleToViewChanged(const QString &role);

public:

   void SetUserNameAndRole(const QString &userName, const QString &role);

private:

   void CreateUI(Terminal *terminal);

   void InsertWidgetsIntoLayout();

   void ConnectObjects();

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
