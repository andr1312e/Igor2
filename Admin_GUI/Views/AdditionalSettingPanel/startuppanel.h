#ifndef ADMIN_GUI_VIEWS_STARTUPPANEL_H
#define ADMIN_GUI_VIEWS_STARTUPPANEL_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QStringListModel>
#include <QWidget>
#include <QListView>
#include <QPushButton>

#include "Services/startuprepositoryservice.h"

#include "Admin_GUI/Views/qtoast.h"

#include "Admin_GUI/Widgets/qtmaterialdialog.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/startupdialogwidget.h"

enum STARTUP_PANEL_TYPE {
   USER_APPS,
   ROLE_APPS,
};

class StartupPanel : public QWidget
{
   Q_OBJECT

public:

   StartupPanel(Terminal *terminal, const STARTUP_PANEL_TYPE type, QWidget *parent);

   ~StartupPanel();

   void setParam(const QString &param, QStringList *users);

public Q_SLOTS:

   void OnSetDefaultRoleApps(const QString &role);

Q_SIGNALS:

   void ToRoleStartupFileChanging();

private:

   void CreateServices(Terminal *terminal);
   void SetBackGroundColor();
   void CreateUI();
   void CreateModel();
   void InsertWidgetsIntoLayout();
   void ConnectObjects();

private:

   const STARTUP_PANEL_TYPE m_type;

   QStringList *m_usersList;

   StartupRepositoryService *m_startupRepositoryService;

   QString m_startupFilePath;

   QStringListModel *m_appsList;

   int m_selectedItemIndex;

   QVBoxLayout *m_mainLayout;

   QLabel *m_titleLabel;

   QListView *m_allProgramsListView;

   QHBoxLayout *m_bottomLayout;

   QPushButton *m_addProgramButton;

   QPushButton *m_deleteProgramButton;

   QtMaterialDialog *m_dialog;

   QVBoxLayout *m_dialogLayout;

   StartupDialogWidget *m_dialogWidget;

private Q_SLOTS:

   void OnDeleteProgram();

   void OnAddProgram(const QString &exec);

   void OnProgramSelect(const QModelIndex &index);

private:

   void UpdateAllUsersWithCurrentRole(QStringList &appsList);

   void UpdateModel();

   void WriteAppListToAllUsersWithRole(QStringList &appsList);

};

#endif // ADMIN_GUI_VIEWS_STARTUPPANEL_H
