#include "Styles/Frameless/windowdragger.h"

WindowTitleBar::WindowTitleBar(QWidget *parent)
    : QWidget(parent)
    , m_isMousePressed(false)
{
}

WindowTitleBar::~WindowTitleBar()
{

}

void WindowTitleBar::mousePressEvent(QMouseEvent *event) {
    m_isMousePressed = true;
    m_mousePosition = event->globalPos();

    QWidget *topWidget = parentWidget();
    if (topWidget)
    {
        topWidget = topWidget->parentWidget();
    }

    if (topWidget)
    {
        m_windowPosition = topWidget->pos();
    }
}

void WindowTitleBar::mouseMoveEvent(QMouseEvent *event) {
    QWidget *parent = parentWidget();
    if (parent) parent = parent->parentWidget();

    if (parent && m_isMousePressed)
    {
        parent->move(m_windowPosition + (event->globalPos() - m_mousePosition));
    }
}

void WindowTitleBar::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    m_isMousePressed = false;
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
    Q_EMIT ToDoubleClicked();
}
