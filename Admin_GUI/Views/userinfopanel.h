#ifndef ADMIN_GUI_VIEWS_USERINFOPANEL_H
#define ADMIN_GUI_VIEWS_USERINFOPANEL_H

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#include "Structs/userstruct.h"
#include "Services/integrityservice.h"

class UserInfoPanel : public QWidget
{
    Q_OBJECT
public:

    UserInfoPanel(Terminal *terminal, QWidget *parent);
    ~UserInfoPanel();
    void setUser(User &user);

private:

    void initServices(Terminal *terminal);
    void initUI();
    void insertWidgetsIntoLayout();

private:

    IntegrityService *m_integrityService;

    QString m_repositoryPath;

    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_userIdLayout;
    QLabel *m_userIdLabel;
    QLabel *m_userIdValue;

    QHBoxLayout *m_userNameLayout;
    QLabel *m_userNameLabel;
    QLabel *m_userNameValue;

    QHBoxLayout *m_fileIntegrityLevelLayout;
    QLabel *m_fileIntegrityLevelLabel;
    QLabel *m_fileIntegrityLevelValue;

    QHBoxLayout *m_userIntegrityLevelLayout;
    QLabel *m_userIntegrityLevelLabel;
    QLabel *m_userIntegrityLevelValue;

    QLabel *m_canUserEditFile;

    QLabel *m_isInDBExistLabel;

    QHBoxLayout *m_FCSLayout;
    QLabel *m_FCSLabel;
    QLabel *m_FCSValue;

    QHBoxLayout *m_rankLayout;
    QLabel *m_rankLabel;
    QLabel *m_rankValue;

    QHBoxLayout *m_roleLayout;
    QLabel *m_roleLabel;
    QLabel *m_roleValue;

private:

    void setBasicInfo(QString &userId, QString userName);

public slots:

    void fillUserAdditionalInfo(const QString &FCs, const QString &rank, const QString &role);

    void clearAdditionalInfo();

};

#endif // ADMIN_GUI_VIEWS_USERINFOPANEL_H
