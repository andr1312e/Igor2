#ifndef ADMIN_GUI_VIEWS_USEREDITPANEL_H
#define ADMIN_GUI_VIEWS_USEREDITPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QResizeEvent>

#include <Admin_GUI/Model/usermodel.h>

#include "Services/kioskservice.h"

#include "Admin_GUI/Widgets/qtmaterialtoggle.h"
#include "Admin_GUI/Views/qtoast.h"

class UserEditPanel : public QWidget
{
    Q_OBJECT

public:
    UserEditPanel(QStringView currentUserName, QWidget *parent);
    ~UserEditPanel();

private:
    void InitServicesAndModel();
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();
    void SetObjectNames();
    void ConnectObjects();

Q_SIGNALS:
    void ToDeleteUser(const QString &userId, const QString &userName);
    void ToRoleToViewChanged(const int &roleId);
    void ToSaveUser(const QString&userId, const QString&m_userName, const QString &FCS, const QString &rank, const int &oldRoleIndex, const int &newRoleIndex);

private Q_SLOTS:
    void OnSaveUser();
    void OnDeleteUser();
    void OnSetKioskMode(bool kioskModeState);

public:
    void SetUser(const User &user);
    void SetFontSize(const int fontSize);
    void SetButtonSize(const int size);

private:
    void GetUserKioskState(const QString &userName);
    void InsertUserData(const User &user);
    void ShowSaveUserToast(const QString &userName);

private:
    const QStringView m_currentUserName;

    QFont m_editPanelFont;

    KioskService *m_kioskService;

    QString m_userName;
    QString m_userId;
    int m_oldRoleIndex;

    QVBoxLayout *m_mainLayout;
    QLabel *m_editFieldsLabel;
    QLabel *m_FCSFieldsLabel;
    QLineEdit *m_FCSLineEdit;
    QLabel *m_rankEditLabel;
    QComboBox *m_rankComboBox;
    QLabel *m_roleEditLabel;
    QComboBox *m_roleComboBox;

    QHBoxLayout *m_actionLayout;
    QPushButton *m_saveUserDataButton;
    QPushButton *m_removeUserButton;

    QHBoxLayout *m_kioskLayout;
    QLabel *m_kioskModeIsEnabledLabel;
    QLabel *m_kioskModeIsDisabledLabel;
    QtMaterialToggle *m_kiosModeState;

    QMessageBox *m_messagBox;
};

#endif // ADMIN_GUI_VIEWS_USEREDITPANEL_H
