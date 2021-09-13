#ifndef ADMIN_GUI_VIEWS_TOPBAR_H
#define ADMIN_GUI_VIEWS_TOPBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <QPushButton>

class TopBar : public QWidget
{
    Q_OBJECT

public:

    explicit TopBar(QWidget *parent);

    ~TopBar();

    void setData(QString &rank, QString &FCS, QString &role);

signals:

    void hideAdditionalSettings(bool state);

private:

    void initUI();

    void applyStyle();

    void insertWidgetsIntoLayouts();

    void createConnections();

private:

    QDateTime *m_dateTime;

    QTimer *m_timer;

    QHBoxLayout *m_userInfoLayout;

    QLabel *m_currentUserNameLabel;

    QLabel *m_currentTimeLabel;

    QLabel *m_currentUseRoleLabel;

    QPushButton *m_settingsPushButtton;

private slots:

    void onButtonClick(bool state);
    void refreshDateTime();

private:

    void changeTextSize(bool state);

    const int m_smallFontSize=10;
    const int m_bigFontSize=19;
};

#endif // ADMIN_GUI_VIEWS_TOPBAR_H
