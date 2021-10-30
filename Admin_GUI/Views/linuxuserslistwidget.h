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

   LinuxUsersListWidget(UserModel *userModel, QWidget *parent);
   ~LinuxUsersListWidget();

Q_SIGNALS:

   void OnUserClick(User &user);
   void Search(const QString &text, const QString &attribute);

private:

   void CreateProxyModel(UserModel *userModel);
   void CreateUI();
   void InsertWidgetsIntoLayout();
   void SetModelToListView();
   void CreateConnections();

private:

   QFont *m_font;

   int m_oldFontSize;

   QVBoxLayout *m_mainLayout;

   QLabel *m_linuxUsersLabel;

   QHBoxLayout *m_searchLayout;

   QLineEdit *m_searchLineEdit;

   QComboBox *m_searchTypeComboBox;

   QListView *m_allUsersListView;

   UserDelegate *m_userDelegate;

   QStandardItemModel *m_model;

   SortModel *m_sortModel;

private:

   void UpdateFontSize();

private Q_SLOTS:

   void OnLineEditChange(const QString &text);
   void OnComboBoxChange(const QString &attribute);
   void GetUserData(const QModelIndex &index);

protected:

   void resizeEvent(QResizeEvent *event) override;

};

#endif // ADMIN_GUI_VIEWS_LINUXUSERLISTWIDGET_H
