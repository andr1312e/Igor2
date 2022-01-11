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

class ISqlDatabaseService;

class StartupPanel : public QWidget
{
   Q_OBJECT

public:
   StartupPanel(ISqlDatabaseService *sqlDatabaseService, QWidget *parent);
   ~StartupPanel();

private:
   void CreateServices(ISqlDatabaseService *sqlDatabaseService);
   void SetBackGroundColor();
   void CreateUI();
   void CreateModel();
   void InsertWidgetsIntoLayout();
   void ConnectObjects();

Q_SIGNALS:
   void ToRoleStartupChanges(const quint8 &roleId);

private Q_SLOTS:
   void OnDeleteProgram();
   void OnAddProgram(const QString &startupPath);
   void OnProgramSelect(const QModelIndex &index);

public:
   void SetRoleId(const quint8 &roleId);

private:
   void AppendStartupToModel(const QString &startupPath);
   void GetAllStartups();

private:

   quint8 m_roleId;

   StartupRepositoryPresenter *m_startupRepositoryPresenter;

   QStringListModel *m_appsList;

   int m_selectedItemIndex;

private:

   QVBoxLayout *m_mainLayout;

   QLabel *m_titleLabel;

   QListView *m_allProgramsListView;

   QHBoxLayout *m_bottomLayout;

   QPushButton *m_addProgramButton;

   QPushButton *m_deleteProgramButton;

   QtMaterialDialog *m_dialog;

   QVBoxLayout *m_dialogLayout;

   StartupDialogWidget *m_dialogWidget;
};

#endif // ADMIN_GUI_VIEWS_STARTUPPANEL_H
