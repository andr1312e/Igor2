#include "Styles/Frameless/windowdragger.h"

WindowTitleBar::WindowTitleBar(QWidget *parent)
  : QWidget(parent)
{
  mousePressed = false;
}

void WindowTitleBar::mousePressEvent(QMouseEvent *event) {
  mousePressed = true;
  mousePos = event->globalPos();

  QWidget *parent = parentWidget();
  if (parent) parent = parent->parentWidget();

  if (parent) wndPos = parent->pos();
}

void WindowTitleBar::mouseMoveEvent(QMouseEvent *event) {
  QWidget *parent = parentWidget();
  if (parent) parent = parent->parentWidget();

  if (parent && mousePressed)
    parent->move(wndPos + (event->globalPos() - mousePos));
}

void WindowTitleBar::mouseReleaseEvent(QMouseEvent *event) {
  Q_UNUSED(event);
  mousePressed = false;
}

void WindowTitleBar::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QStyleOption styleOption;
  styleOption.init(this);
  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
}

void WindowTitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
  Q_UNUSED(event);
  emit doubleClicked();
}
