#ifndef ADMIN_GUI_VIEWS_USERINFOPANEL_H
#define ADMIN_GUI_VIEWS_USERINFOPANEL_H

#include <QGridLayout>
#include <QLabel>

#include "Structs/userstruct.h"

class UserInfoPanel : public QWidget
{
    Q_OBJECT
public:
    UserInfoPanel(QWidget *parent);
    ~UserInfoPanel();

private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void FillUI();

public Q_SLOTS:
    void OnFillUserAdditionalInfo(const QString &userId, const QString &FCS, const QString &rank, const QString &oldRole, const QString &newRole);
    void OnEraseData();

public:
    void SetUser(const User &user);
    void SetFontSize(int fontSize);

private:
    void SetUserName(const QString &userName);

private:
    QFont m_editPanelFont;

    QGridLayout *m_mainLayout;

    QLabel *m_userIdLabel;
    QLabel *m_userIdValue;
    QLabel *m_userNameLabel;
    QLabel *m_userNameValue;

    QLabel *m_isInDBExistLabel;

    QLabel *m_FCSLabel;
    QLabel *m_FCSValue;

    QLabel *m_rankLabel;
    QLabel *m_rankValue;
    QLabel *m_roleLabel;
    QLabel *m_roleValue;
};

#endif // ADMIN_GUI_VIEWS_USERINFOPANEL_H
