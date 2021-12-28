#include "Styles/InteractiveButtons/maximizewindowbutton.h"

MaximizeWindowButton::MaximizeWindowButton(QWidget *parent)
    : InteractiveButtonBase (parent)
    , m_pen(new QPen(Qt::white))
{
    InitGeometry();
}

MaximizeWindowButton::~MaximizeWindowButton()
{
    delete m_pen;
    delete m_painter;
}

void MaximizeWindowButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);
    m_painter=new QPainter(this);
    m_painter->setPen(*m_pen);
    if (!show_foreground) return ; // 不显示前景

    int w = _w, h = _h;
    int dx = offset_pos.x(), dy = offset_pos.y();
    QRect r;

    r = QRect(_l+w/3+dx, _t+h/3+dy, w/3, h/3);


//    m_painter->begin(this);
    m_painter->drawRect(r);
    m_painter->end();
}

QSize MaximizeWindowButton::sizeHint() const
{
    return  QSize(40, 40);
}

void MaximizeWindowButton::InitGeometry()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setUnifyGeomerey(true);
}
