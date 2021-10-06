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
    UserEditPanel(const QString &userName, UserModel *model, Terminal *terminal, QWidget *parent);
    ~UserEditPanel();
    void setUser(User &user);

private:

    void initServicesAndModel(UserModel *model, Terminal *terminal);
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

    QFont *m_editPanelFont;
    QFont *m_widgetsFont;

    UserModel *m_model;
    KioskService *m_kioskService;

    QString m_userName;
    QString m_currentUserName;
    QString m_userId;

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

    QLineEdit *m_fontEdit;
    QLineEdit *m_buttonWidthEdit;

private:

    void setKioskMode(bool kioskModeState);
    void checkUserKiosk(QString &userName);
    void insertUserData(User &user);
    void showToast(QString &userName);

    void resizeButton(int size);

private slots:

    void saveUser();
    void deleteUser();
    void setFontSize(const QString &size);
    void ButtonSize(const QString &size);

protected:

    void resizeEvent(QResizeEvent *event) override;

};

#endif // ADMIN_GUI_VIEWS_USEREDITPANEL_H
