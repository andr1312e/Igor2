#include "closewindowbutton.h"

CloseWindowButton::CloseWindowButton(QWidget *parent)
    : InteractiveButtonBase (parent)
    , tr_radius(0)
    , m_pen(new QPen(Qt::white))
    , m_buttonSize(new QSize(40, 40))
{
    InitGeometry();
    setUnifyGeomerey(true);
}

CloseWindowButton::~CloseWindowButton()
{
    delete m_pen;
    delete m_buttonSize;
}

void CloseWindowButton::paintEvent(QPaintEvent *event)
{

    InteractiveButtonBase::paintEvent(event);
    QPainter *m_painter=new QPainter(this);
    m_painter->setPen(*m_pen);
    if (!show_foreground) return ;

    int w = _w, h = _h;
    int l = _l+w/3, t = _t+h/3, r = w*2/3, b = h*2/3;
    int mx = _l+w/2+offset_pos.x(), my = _t+h/2+offset_pos.y();

    m_painter->setRenderHint(QPainter::Antialiasing,true);

    if (offset_pos.x()==0 && offset_pos.y()==0)
    {
        m_painter->drawLine(QPoint(l,t), QPoint(r,b));
        m_painter->drawLine(QPoint(r,t), QPoint(l,b));
    }
    else
    {
        QPainterPath path;
        path.moveTo(QPoint(l,t));
        path.cubicTo(QPoint(l,t), QPoint(mx,my), QPoint(r,b));
        path.moveTo(QPoint(r,t));
        path.cubicTo(QPoint(r,t), QPoint(mx,my), QPoint(l,b));

        m_painter->drawPath(path);
    }
    m_painter->end();
    delete m_painter;
}

void CloseWindowButton::setTopRightRadius(int r)
{
    tr_radius = r;
}

QPainterPath CloseWindowButton::getBgPainterPath()
{
    if (!tr_radius)
        return InteractiveButtonBase::getBgPainterPath();

    QPainterPath path = InteractiveButtonBase::getBgPainterPath();
    QPainterPath round_path;
    round_path.addEllipse(width() - tr_radius - tr_radius, 0, tr_radius*2, tr_radius*2);
    QPainterPath corner_path;
    corner_path.addRect(width() - tr_radius, 0, tr_radius, tr_radius);
    corner_path -= round_path;
    path -= corner_path;
    return path;
}

QPainterPath CloseWindowButton::getWaterPainterPath(Water water)
{
    return InteractiveButtonBase::getWaterPainterPath(water) & CloseWindowButton::getBgPainterPath();
}

QSize CloseWindowButton::sizeHint() const
{
    return *m_buttonSize;
}

void CloseWindowButton::InitGeometry()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setUnifyGeomerey(true);
}
