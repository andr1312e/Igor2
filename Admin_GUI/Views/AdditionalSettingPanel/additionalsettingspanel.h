#ifndef ADMIN_GUI_VIEWS_ADDITIONALSETINGSPANEL_H
#define ADMIN_GUI_VIEWS_ADDITIONALSETINGSPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QListView>
#include <QPushButton>
#include <QStandardItemModel>

#include "Services/linuxuserservice.h"

#include "Admin_GUI/Views/AdditionalSettingPanel/roleeditpanel.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/userfileseditpanel.h"

class AdditionalSettingsPanel : public QWidget
{
   Q_OBJECT

public:

   AdditionalSettingsPanel(Terminal *terminal, QWidget *parent);
   ~AdditionalSettingsPanel();

private:
   void CreateUI(Terminal *terminal);
   void SetBackGroundColor();
   void InsertWidgetsIntoLayout();
   void ConnectObjects();

Q_SIGNALS:
   void ToSetDefaultRoleApps(const QString &role);

public:
   void setRoleEditPanelData(const QString &role, QStringList *users);
   void setUserFilesEditPanelData(const QString &name, const QString &role);

private:

   QHBoxLayout *m_mainLayout;

   UserFilesEditPanel *m_userFilesEditPanel;

   RoleEditPanel *m_roleEditPanel;

};

#endif // ADMIN_GUI_VIEWS_ADDITIONALSETINGSPANEL_H
