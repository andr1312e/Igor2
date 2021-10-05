#include "Styles/InteractiveButtons/maximizewindowbutton.h"

MaximizeWindowButton::MaximizeWindowButton(QWidget *parent)
    : InteractiveButtonBase (parent)
    , m_pen(new QPen(Qt::white))
    , m_painter(new QPainter(this))
{
    initGeometry();
    initDrawing();
}

MaximizeWindowButton::~MaximizeWindowButton()
{
    delete m_pen;
    delete m_painter;
}

void MaximizeWindowButton::paintEvent(QPaintEvent *event)
{
    InteractiveButtonBase::paintEvent(event);

    if (!show_foreground) return ; // 不显示前景

    int w = _w, h = _h;
    int dx = offset_pos.x(), dy = offset_pos.y();
    QRect r;

    r = QRect(_l+w/3+dx, _t+h/3+dy, w/3, h/3);


    m_painter->begin(this);
    m_painter->drawRect(r);
    m_painter->end();
}

QSize MaximizeWindowButton::sizeHint() const
{
    return  QSize(40, 40);
}

void MaximizeWindowButton::initGeometry()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setUnifyGeomerey(true);
}

void MaximizeWindowButton::initDrawing()
{
    m_painter->setPen(*m_pen);
}
