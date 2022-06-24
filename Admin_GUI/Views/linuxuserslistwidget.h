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

    explicit LinuxUsersListWidget(UserModel *userModel, QWidget *parent);
    ~LinuxUsersListWidget();

private:
    void SetModel(UserModel *userModel);
    void CreateProxyModel();
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void SetModelToListView();
    void CreateConnections();

Q_SIGNALS:

    void ToUserClick(const User &user);
    void ToSearch(const QString &text, const QString &attribute);

private Q_SLOTS:
    void OnLineEditChange(const QString &text);
    void OnComboBoxChange(const QString &attribute);
    void OnAddNewUser();
    void GetUserData(const QModelIndex &index);

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

    QHBoxLayout *m_topLayout;
    QLabel *m_linuxUsersLabel;
    QPushButton *m_addNewUserOpenPanel;

    QHBoxLayout *m_searchLayout;
    QLineEdit *m_searchLineEdit;
    QComboBox *m_searchTypeComboBox;

    QListView *m_allUsersListView;
    UserDelegate *m_userDelegate;
    QStandardItemModel *m_model;

};

#endif // ADMIN_GUI_VIEWS_LINUXUSERLISTWIDGET_H
