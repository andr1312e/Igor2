#ifndef ADMIN_GUI_VIEWS_ADMIN_GUI_H
#define ADMIN_GUI_VIEWS_ADMIN_GUI_H

#include <QLabel>

#include "Services/databaseservice.h"
#include "Services/linuxuserservice.h"

#include "Admin_GUI/Model/usermodel.h"

#include "Admin_GUI/Views/topbar.h"
#include "Admin_GUI/Views/linuxuserslistwidget.h"
#include "Admin_GUI/Views/SettingsPanel/settingspanel.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/additionalsettingspanel.h"


class Admin_GUI : public QWidget
{
   Q_OBJECT

public:

   Admin_GUI(DatabaseService *databaseService, LinuxUserService *userService, QWidget *parent = Q_NULLPTR);
   ~Admin_GUI();

Q_SIGNALS:

   void ToChangeTheme(bool isDarkTheme);

private:

   void CreateUI();
   void SetWidgetSizes();
   void InsertWidgetsIntoLayout();
   void ConnectObjects();
   void SetMaximumWidgetSize();

private:

   DatabaseService *m_databaseService;
   LinuxUserService *m_linuxUserService;
   UserModel *m_userModel;

   QVBoxLayout *m_mainLayout;
   TopBar *m_topBar;

   QHBoxLayout *m_programLayout;
   LinuxUsersListWidget *m_linuxUsersListWidget;
   SettingsPanel *m_settingsPanel;
   AdditionalSettingsPanel *m_additionalSettingsPanel;



   QScreen *m_currentScreen;
   int m_maxWidth;
   int m_maxHeight;

private:
   void initTopBar();

private Q_SLOTS:
   void OnHideAdditionalSettings(bool state);
   void roleToViewChanged(const QString &role);
   void onLinuxUserClick(User &user);
};

#endif // ADMIN_GUI_VIEWS_ADMIN_GUI_H
