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
#include "Styles/Frameless/windowdragger.h"

class FramelessWindow : public QWidget
{

   Q_OBJECT

public:

   explicit FramelessWindow(QWidget *parent);
   virtual ~FramelessWindow();

   void SetMainWidget(QWidget *w);

Q_SIGNALS:
   void minimizeWindow();


private:
   bool LeftBorderHit(const QPoint &pos);
   bool RightBorderHit(const QPoint &pos);
   bool TopBorderHit(const QPoint &pos);
   bool BottomBorderHit(const QPoint &pos);
   void StyleWindow(bool bActive, bool bNoState);

public Q_SLOTS:

   void SetWindowTitle(const QString &text);
   void SetWindowIcon(const QIcon &icon);

private Q_SLOTS:

   void OnApplicationStateChanged(Qt::ApplicationState state);
   void OnRestoreButtonClicked();
   void OnMaximizeButtonClicked();
   void WindowDraggerDoubleClicked();


protected:

   virtual void changeEvent(QEvent *event);
   virtual void mouseDoubleClickEvent(QMouseEvent *event);
   virtual void checkBorderDragging(QMouseEvent *event);
   virtual void mousePressEvent(QMouseEvent *event);
   virtual void mouseReleaseEvent(QMouseEvent *event);
   virtual bool eventFilter(QObject *obj, QEvent *event);

private:

   WindowTitleBar *m_WindowTitleBar;
   QWidget *m_windowFrame;
   QVBoxLayout *m_mainLayout;
   QVBoxLayout *m_vertivalLayout;
   QHBoxLayout *m_topLayout;
   CloseWindowButton *m_closeButton;
   MaximizeWindowButton *m_maximizeButton;
   MinimizeWindowButton *m_minimizeButton;
   RestoreWindowButton *m_zoomButton;
   QLabel *m_icon;
   QLabel *m_titleText;

   QGraphicsDropShadowEffect *m_textShadow;

   QRect m_StartGeometry;
   const quint8 CONST_DRAG_BORDER_SIZE = 15;
   bool m_bMousePressed;
   bool m_bDragTop;
   bool m_bDragLeft;
   bool m_bDragRight;
   bool m_bDragBottom;

private:

   void CreateUI();
   void SetShadowUnderTitleText();
   void SetWindowShadow();
   void InsertWidgetsIntoLayout();
   void ApplyStyles();
   void SetObjectNames();
   void CreateConnections();
};

#endif  // STYLES_FRAMELESSWINDOW_H
