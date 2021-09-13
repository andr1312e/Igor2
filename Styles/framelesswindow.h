#ifndef STYLES_FRAMELESSWINDOW_H
#define STYLES_FRAMELESSWINDOW_H

#include <QLabel>
#include <QToolButton>
#include <QWidget>
#include <QBoxLayout>

#include "Styles/windowdragger.h"

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
  QToolButton *m_closeButton;
  QToolButton *m_maximizeButton;
  QToolButton *m_minimizeButton;
  QToolButton *m_restoreButton;
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
