#ifndef STYLES_WINDOWDRAGGER_H
#define STYLES_WINDOWDRAGGER_H

#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>


class WindowTitleBar : public QWidget {
  Q_OBJECT

 public:
  explicit WindowTitleBar(QWidget *parent = Q_NULLPTR);
  virtual ~WindowTitleBar() {}

 signals:
  void doubleClicked();

 protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);

 protected:
  QPoint mousePos;
  QPoint wndPos;
  bool mousePressed;
};

#endif // STYLES_WINDOWDRAGGER_H
