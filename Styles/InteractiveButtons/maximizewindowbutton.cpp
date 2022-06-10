#include "Styles/InteractiveButtons/maximizewindowbutton.h"

MaximizeWindowButton::MaximizeWindowButton(QWidget *parent)
    : InteractiveButton (parent)
    , m_pen(Qt::white)
    , m_buttonSize(40, 40)
{
    InitGeometry();
}

MaximizeWindowButton::~MaximizeWindowButton()
{

}

void MaximizeWindowButton::paintEvent(QPaintEvent *event)
{
    InteractiveButton::paintEvent(event);
    QPainter painter(this);
    painter.setPen(m_pen);
    if (!isShowForeground)
    {
        return ;
    }
    const int w = _w, h = _h;
    const int dx = offset_pos.x(), dy = offset_pos.y();
    const QRect r(_l+w/3+dx, _t+h/3+dy, w/3, h/3);

    painter.drawRect(r);
}

QSize MaximizeWindowButton::sizeHint() const
{
    return m_buttonSize;
}

void MaximizeWindowButton::InitGeometry()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setUnifyGeomerey(true);
}
