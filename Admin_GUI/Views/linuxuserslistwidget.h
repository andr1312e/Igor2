#ifndef ADMIN_GUI_VIEWS_LINUXUSERLISTWIDGET_H
#define ADMIN_GUI_VIEWS_LINUXUSERLISTWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QListView>
#include <QPushButton>
#include <QComboBox>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include "Admin_GUI/Delegates/userdelegate.h"

#include "Admin_GUI/Model/sortmodel.h"
#include "Admin_GUI/Model/usermodel.h"

class LinuxUsersListWidget : public QWidget
{
   Q_OBJECT
public:

   LinuxUsersListWidget(ISqlDatabaseService *databaseService, LinuxUserService *userService, QWidget *parent);
   ~LinuxUsersListWidget();

private:
   void CreateModel(ISqlDatabaseService *databaseService, LinuxUserService *userService);
   void CreateProxyModel();
   void CreateUI();
   void InsertWidgetsIntoLayout();
   void SetModelToListView();
   void CreateConnections();

Q_SIGNALS:

   void ToUserClick(const User &user);
   void ToSearch(const QString &text, const QString &attribute);
   void ToSetDelegateView(bool state);

private Q_SLOTS:
   void OnLineEditChange(const QString &text);
   void OnComboBoxChange(const QString &attribute);
   void GetUserData(const QModelIndex &index);

public:
   void SetDelegateView(bool state);
   void DeleteUser(const QString &userId);
   void AddUserToModel(const QString &userId, const QString &FCS, const QString &rank, const int &role);

private:
   void UpdateFontSize();

protected:
   virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
   QFont m_font;
   int m_oldFontSize;

private:

   UserModel *m_userModel;
   SortModel *m_sortModel;

private:

   QVBoxLayout *m_mainLayout;
   QLabel *m_linuxUsersLabel;

   QHBoxLayout *m_searchLayout;
   QLineEdit *m_searchLineEdit;
   QComboBox *m_searchTypeComboBox;

   QListView *m_allUsersListView;
   UserDelegate *m_userDelegate;
   QStandardItemModel *m_model;

};

#endif // ADMIN_GUI_VIEWS_LINUXUSERLISTWIDGET_H
