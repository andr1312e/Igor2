#ifndef ADMIN_GUI_VIEWS_DESKTOPPANEL_H
#define ADMIN_GUI_VIEWS_DESKTOPPANEL_H

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>

#include "Structs/programstruct.h"
#include "Services/isqlservice.h"
#include "Services/fileexplorer.h"

#include "Admin_GUI/Delegates/filedelegate.h"
#include "Admin_GUI/Widgets/qtmaterialdialog.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/filedialogwidget.h"

enum ICONS_PANEL_TYPE {
   USER_DESKTOP,
   ROLE_DESKTOP
};

class DesktopPanel: public QWidget
{
   Q_OBJECT
public:
   DesktopPanel(ICONS_PANEL_TYPE type, Terminal *terminal, ISqlDatabaseService *sqlDatabaseService, QWidget *parent);
   ~DesktopPanel();

private:
   void CreatePresenter(Terminal *terminal, ISqlDatabaseService *sqlDatabaseService);
   void CreateUI();
   void SetBackgroundColor();
   void SetPresenterModelToView();
   void InsertWidgetsIntoLayout();
   void ConnectObjects();

public Q_SLOTS:
   void OnSetDefaultRoleApps(const quint8 &role);
   void OnRoleDesktopChanges(const quint8 &roleId);

private Q_SLOTS:
   void OnAddProgram(const QString &exec, const QString &iconPath, const QString &iconName);
   void OnDeleteProgram();
   void OnProgramSelect(const QModelIndex &index);

public:
   void SetUser(const User &user);
   void SetRoleId(const int &roleId);//userName || role
   void DeleteUserAllRoleIcons();

private:

   int m_roleId;

   ICONS_PANEL_TYPE m_type;

   DesktopPanelPresenter *m_desktopPanelPresenter;

   QStringList *m_usersList;

   QStandardItemModel *m_model;

   FileDelegate *m_fileDelegate;

   QString m_userName;
   QString m_selectedItemName;

private:

   QVBoxLayout *m_mainLayout;

   QLabel *m_programsToRun;
   QListView *m_allProgramsListView;

   QHBoxLayout *m_bottomLayout;
   QPushButton *m_addProgramButton;
   QPushButton *m_deleteProgramButton;

   QtMaterialDialog *m_dialog;
   QVBoxLayout *m_dialogLayout;
   DesktopUploadDialogWidget *m_dialogWidget;

private:
   void UpdateAllUsersWithCurrentRole();
};

#endif // ADMIN_GUI_VIEWS_DESKTOPPANEL_H
