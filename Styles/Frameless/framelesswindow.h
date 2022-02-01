#ifndef STYLES_FRAMELESSWINDOW_H
#define STYLES_FRAMELESSWINDOW_H

#include <QLabel>
#include <QToolButton>
#include <QWidget>
#include <QBoxLayout>
#include <QGraphicsDropShadowEffect>
#include "Styles/InteractiveButtons/minimizewindowbutton.h"
#include "Styles/InteractiveButtons/maximizewindowbutton.h"
#include "Styles/InteractiveButtons/restorewindowbutton.h"
#include "Styles/InteractiveButtons/closewindowbutton.h"
#include "Styles/Frameless/windowtitlebar.h"

class FramelessWindow : public QWidget
{
   Q_OBJECT
public:

   explicit FramelessWindow(QWidget *parent);
   ~FramelessWindow();

private:
   void CreateUI();
   void InitUI();
   void SetShadowUnderTitleText();
   void SetWindowShadow();
   void InsertWidgetsIntoLayout();
   void ApplyStyles();
   void SetObjectNames();
   void CreateConnections();

Q_SIGNALS:
   void ToChangeTheme(bool isDarkTheme);
   void ToHideAdditionalSettings(bool state);
   void ToSetDelegateView(bool state);

public Q_SLOTS:
   void OnSetWindowTitle(const QString &text);
   void OnSetWindowIcon(const QIcon &icon);

private Q_SLOTS:
   void OnApplicationStateChanged(const Qt::ApplicationState state);
   void OnChangeThemeButtonClicked(bool state);
   void OnChangeAdditionalSettingsButtonClicked(bool state);
   void OnChangeDelegatesViewButtonClicked(bool state);
   void OnRestoreButtonClicked();
   void OnMaximizeButtonClicked();
   void OnWindowDraggerDoubleClicked();

private:
   bool LeftBorderHit(const QPoint &pos);
   bool RightBorderHit(const QPoint &pos);
   bool TopBorderHit(const QPoint &pos);
   bool BottomBorderHit(const QPoint &pos);
   void StyleWindow(bool isActive, bool isMaximized);
   void CheckBorderDragging(QMouseEvent *event);
protected:
   virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
   virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
   virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
   virtual bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
public:
   void SetMainWidget(QWidget *widget);
private:
   const QSize m_buttonSize=QSize(20, 20);
   WindowTitleBar *m_WindowTitleBar;
   QWidget *m_windowFrame;
   QVBoxLayout *m_mainLayout;
   QVBoxLayout *m_verticalLayout;
   QHBoxLayout *m_topLayout;
   QPushButton *m_changeDelegatesView;
   QPushButton *m_changeThemePushButton;
   QPushButton *m_changeAdditionalSettingsView;

   CloseWindowButton *m_closeButton;
   MaximizeWindowButton *m_maximizeButton;
   MinimizeWindowButton *m_minimizeButton;
   RestoreWindowButton *m_zoomButton;
   QLabel *m_icon;
   QLabel *m_titleText;

   QGraphicsDropShadowEffect *m_textShadow;
   QGraphicsDropShadowEffect *m_windowShadow;//сам удаляется

   QRect m_startGeometry;
   const quint8 m_dragBorderSize = 15;
   bool m_isMousePressed;
   bool m_isDragTop;
   bool m_isDragLeft;
   bool m_isDragRight;
   bool m_isDragBottom;


};

#endif  // STYLES_FRAMELESSWINDOW_H
