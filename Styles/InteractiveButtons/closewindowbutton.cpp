#include "closewindowbutton.h"

CloseWindowButton::CloseWindowButton(QWidget *parent)
    : InteractiveButton (parent)
    , m_radius(0)
    , m_pen(Qt::white)
    , m_buttonSize(40, 40)
{
    InitGeometry();
    setUnifyGeomerey(true);

}

CloseWindowButton::~CloseWindowButton()
{

}

void CloseWindowButton::paintEvent(QPaintEvent *event)
{
    InteractiveButton::paintEvent(event);

    if (!isShowForeground)
    {
        return ;
    }
    QPainter painter(this);
    painter.setPen(m_pen);
    const int w = _w, h = _h;
    int l = _l+w/3, t = _t+h/3, r = w*2/3, b = h*2/3;
    int mx = _l+w/2+offset_pos.x(), my = _t+h/2+offset_pos.y();

    painter.setRenderHint(QPainter::Antialiasing,true);

    if (offset_pos.x()==0 && offset_pos.y()==0)
    {
        painter.drawLine(QPoint(l,t), QPoint(r,b));
        painter.drawLine(QPoint(r,t), QPoint(l,b));
    }
    else
    {
        QPainterPath path;
        path.moveTo(QPoint(l,t));
        path.cubicTo(QPoint(l,t), QPoint(mx,my), QPoint(r,b));
        path.moveTo(QPoint(r,t));
        path.cubicTo(QPoint(r,t), QPoint(mx,my), QPoint(l,b));

        painter.drawPath(path);
    }
}

void CloseWindowButton::setTopRightRadius(int radius)
{
    m_radius = radius;
}

QPainterPath CloseWindowButton::GetBackGroundPainterPath()
{
    if (!m_radius)
    {
        return InteractiveButton::GetBackGroundPainterPath();
    }
    QPainterPath path = InteractiveButton::GetBackGroundPainterPath();
    QPainterPath roundPath;
    roundPath.addEllipse(width() - m_radius - m_radius, 0, m_radius*2, m_radius*2);
    QPainterPath cornerPath;
    cornerPath.addRect(width() - m_radius, 0, m_radius, m_radius);
    cornerPath -= roundPath;
    path -= cornerPath;
    return path;
}

QPainterPath CloseWindowButton::getWaterPainterPath(Water water)
{
    return InteractiveButton::getWaterPainterPath(water) & CloseWindowButton::GetBackGroundPainterPath();
}

QSize CloseWindowButton::sizeHint() const
{
    return m_buttonSize;
}

void CloseWindowButton::InitGeometry()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setUnifyGeomerey(true);
}
