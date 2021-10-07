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

signals:

    void onUserClick(User &user);
    void search(const QString &text, const QString &attribute);

private:

    void createProxyModel(UserModel *userModel);
    void initUI();
    void insertWidgetsIntoLayout();
    void setModelToView();
    void createConnections();

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

    void updateFontSize();

private slots:

    void onLineEditChange(const QString &text);
    void onComboBoxChange(const QString &attribute);
    void getUserData(const QModelIndex &index);

protected:

    void resizeEvent(QResizeEvent *event) override;

};

#endif // ADMIN_GUI_VIEWS_LINUXUSERLISTWIDGET_H
