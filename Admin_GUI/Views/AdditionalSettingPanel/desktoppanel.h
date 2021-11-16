#ifndef ADMIN_GUI_VIEWS_DESKTOPPANEL_H
#define ADMIN_GUI_VIEWS_DESKTOPPANEL_H

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>

#include "Structs/programstruct.h"

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

   DesktopPanel(Terminal *terminal, ICONS_PANEL_TYPE type, QWidget *parent);
   ~DesktopPanel();

   void SetParam(const QString &param, QStringList *users);//userName || role

public Q_SLOTS:

   void OnSetDefaultRoleApps(const QString &role);

private Q_SLOTS:

   void OnAddProgram(const QString &exec, const QString &iconPath, const QString &iconName);
   void OnDeleteProgram();
   void OnProgramSelect(const QModelIndex &index);

private:

   void CreateServices(Terminal *terminal);
   void CreateUI();
   void SetBackGroundColor();
   void InitModel();
   void InsertWidgetsIntoLayout();
   void ConnectObjects();

private:

   ICONS_PANEL_TYPE m_type;

   FileExplorer *m_rootFileService;

   QStringList *m_usersList;

   QStandardItemModel *m_model;

   FileDelegate *m_fileDelegate;

   QString m_pararm;

   QString m_selectedItemName;

   QVBoxLayout *m_mainLayout;

   QLabel *m_programsToRun;

   QListView *m_allProgramsListView;

   QHBoxLayout *m_bottomLayout;

   QPushButton *m_addProgramButton;

   QPushButton *m_deleteProgramButton;


   QtMaterialDialog *m_dialog;

   QVBoxLayout *m_dialogLayout;

   FileDialogWidget *m_dialogWidget;

private:

   void updateAllUsersWithCurrentRole();


};

#endif // ADMIN_GUI_VIEWS_DESKTOPPANEL_H
