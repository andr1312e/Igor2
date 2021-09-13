#ifndef ADMIN_GUI_VIEWS_USEREDITPANEL_H
#define ADMIN_GUI_VIEWS_USEREDITPANEL_H

#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QPushButton>

#include "Services/databaseservice.h"
#include "Services/kioskservice.h"

#include "Admin_GUI/Widgets/qtmaterialtextfield.h"
#include "Admin_GUI/Widgets/qtmaterialtoggle.h"

#include "Admin_GUI/Views/qtoast.h"

class UserEditPanel : public QWidget
{
    Q_OBJECT

public:
    UserEditPanel(const QString &userName, DatabaseService *repository, QWidget *parent);
    ~UserEditPanel();
    void setUser(User &user);

private:

    void initServices(DatabaseService *repository);
    void initUI();
    void applyStyle();
    void insertWidgetsIntoLayout();
    void createConnections();

signals:

    void setDefaultRoleApps(const QString &role);
    void roleToViewChanged(const QString &role);
    void fillUserAdditionalInfo(const QString &FCS, const QString &rank, const QString &role);
    void clearUserAdditionalInfo();

private:

    DatabaseService *m_accountRepository;
    KioskService *m_kioskLockUnLockService;

    QString m_userName;
    QString m_currentUserName;
    QString m_userId;

    QVBoxLayout *m_mainLayout;
    QLabel *m_editFieldsLabel;
    QtMaterialTextField *m_FCSLineEdit;
    QLabel *m_rankEditLabel;
    QComboBox *m_rankComboBox;
    QLabel *m_roleEditLabel;
    QComboBox *m_roleComboBox;

    QHBoxLayout *m_actionLayout;
    QPushButton *m_addUserButton;
    QPushButton *m_removeUserButton;

    QHBoxLayout *m_kioskLayout;
    QLabel *m_kioskModeIsEnabledLabel;
    QLabel *m_kioskModeIsDisabledLabel;
    QtMaterialToggle *m_kiosModeState;

    QMessageBox *m_messagBox;

private:

    void setKioskMode(bool kioskModeState);
    void checkUserKiosk(QString &userName);
    void insertUserData(User &user);
    void showToast(QString &userName);

private slots:

    void saveUser();
    void deleteUser();

};

#endif // ADMIN_GUI_VIEWS_USEREDITPANEL_H
