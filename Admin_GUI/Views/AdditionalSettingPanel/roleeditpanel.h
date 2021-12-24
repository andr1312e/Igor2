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

   RoleEditPanel(Terminal *terminal, ISqlDatabaseService *sqlDatabaseService, QWidget *parent);

   ~RoleEditPanel();

private:

   void CreateUI(Terminal *terminal, ISqlDatabaseService *sqlDatabaseService);

   void InsertWidgetsIntoLayout();

   void FillUI();

   void ConnectObjects();

Q_SIGNALS:

   void ToRoleDesktopChanges(const quint8 &roleId);

public Q_SLOTS:

   void OnRoleToViewChanged(const int &roleId);

private:

   QVBoxLayout *m_mainLayout;

   QHBoxLayout *m_topLayout;

   QLabel *m_titleLabel;

   QLabel *m_descriptionLabel;

   QLabel *m_currentRoleLabel;

   DesktopPanel *m_roleDesktopPanel;

   StartupPanel *m_roleRunningApplicationPanel;
};

#endif // ADMIN_GUI_VIEWS_ROLEINFOPANEL_H
