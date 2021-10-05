#ifndef STYLES_FRAMELESSWINDOW_H
#define STYLES_FRAMELESSWINDOW_H

#include <QLabel>
#include <QToolButton>
#include <QWidget>
#include <QBoxLayout>
#include "Styles/InteractiveButtons/minimizewindowbutton.h"
#include "Styles/InteractiveButtons/maximizewindowbutton.h"
#include "Styles/InteractiveButtons/restorewindowbutton.h"
#include "Styles/InteractiveButtons/closewindowbutton.h"
#include "Styles/Frameless/windowdragger.h"

class FramelessWindow : public QWidget {

  Q_OBJECT

public:

  explicit FramelessWindow(QWidget *parent);
  virtual ~FramelessWindow();

  void setAdminGUI(QWidget *w);

signals:
    void minimizeWindow();


 private:
  bool leftBorderHit(const QPoint &pos);
  bool rightBorderHit(const QPoint &pos);
  bool topBorderHit(const QPoint &pos);
  bool bottomBorderHit(const QPoint &pos);
  void styleWindow(bool bActive, bool bNoState);

public slots:

  void setWindowTitle(const QString &text);
  void setWindowIcon(const QIcon &icon);

private slots:

  void on_applicationStateChanged(Qt::ApplicationState state);
  void restoreButtonClicked();
  void maximizeButtonClicked();
  void windowDraggerDoubleClicked();

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

  QRect m_StartGeometry;
  const quint8 CONST_DRAG_BORDER_SIZE = 15;
  bool m_bMousePressed;
  bool m_bDragTop;
  bool m_bDragLeft;
  bool m_bDragRight;
  bool m_bDragBottom;

private:

  void initUI();
  void setShadowUnderTitleText();
  void setWindowShadow();
  void insertWidgetsIntoLayout();
  void applyStyles();
  void setObjectNames();
  void createConnections();
};

#endif  // STYLES_FRAMELESSWINDOW_H
