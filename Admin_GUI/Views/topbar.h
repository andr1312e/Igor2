#ifndef ADMIN_GUI_VIEWS_TOPBAR_H
#define ADMIN_GUI_VIEWS_TOPBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <QPushButton>
#include <QResizeEvent>

class TopBar : public QWidget
{
   Q_OBJECT

public:

   explicit TopBar(QWidget *parent);

   ~TopBar();

   void SetData(QString &rank, QString &FCS, QString &role);

Q_SIGNALS:

   void ToHideAdditionalSettings(bool state);
   void ToChangeTheme(bool state);

private:

   void CreateUI();

   void ApplyStyle();

   void InsertWidgetsIntoLayouts();

   void CreateConnections();

private:

   bool m_isWhiteTheme;

   bool m_settingsSetHidden;

   QFont *m_titleFont;

   const int m_iconSize;

   int m_oldFontSize;

   QDateTime *m_dateTime;

   QTimer *m_timer;

   QHBoxLayout *m_userInfoLayout;

   QLabel *m_currentUserNameLabel;

   QLabel *m_currentTimeLabel;

   QLabel *m_currentUseRoleLabel;

   QPushButton *m_settingsPushButtton;

   QPushButton *m_themePushButton;

   QSize *m_iconsSize;

private Q_SLOTS:

   void OnSettingsButtonClick(bool state);
   void OnThemeButtonClick(bool state);
   void RefreshDateTime();

private:

   bool IsNeedToUpdateFontSize(int width);
   void UpdateFontSize();

protected:

   void resizeEvent(QResizeEvent *event);
};

#endif // ADMIN_GUI_VIEWS_TOPBAR_H
