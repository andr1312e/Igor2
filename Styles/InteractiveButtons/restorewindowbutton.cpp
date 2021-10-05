#include "restorewindowbutton.h"

RestoreWindowButton::RestoreWindowButton(QWidget* parent)
    : InteractiveButtonBase(parent)
    , m_pen(new QPen(Qt::white))
    , m_painter(new QPainter(this))
    , m_buttonSize(new QSize(40, 40))
    , m_br(new QRect())
    , m_topLeft(new QPoint())
    , m_topRight(new QPoint())
    , m_bottomLeft(new QPoint())
    , m_bottomRight(new QPoint())
    , m_points(new std::array<QPoint, 5>())
{
    m_painter->setPen(*m_pen);
    initGeometry();
    setUnifyGeomerey(true);
}

RestoreWindowButton::~RestoreWindowButton()
{
    delete m_pen;
    delete m_painter;
    delete m_buttonSize;
    delete m_br;
    delete m_topLeft;
    delete m_topRight;
    delete m_bottomLeft;
    delete m_bottomRight;
    delete m_points;
}

void RestoreWindowButton::paintEvent(QPaintEvent* event)
{

    if (!show_foreground) return ;

    int w = _w, h = _h;
    int dx = offset_pos.x(), dy = offset_pos.y();
    m_br->setRect(_l+w/3+dx, _t+h/3+dy, w/3, h/3);
    m_painter->begin(this);
    m_painter->drawRect(*m_br);

    dx /= 2; dy /= 2;
    int l = _l+w*4/9+dx, t = _t+h*2/9+dy, r = _l+w*7/9+dx, b = _t+h*5/9+dy;
    m_topLeft->setX(l);
    m_topLeft->setY(t);

    m_topRight->setX(r);
    m_topRight->setY(t);

    m_bottomLeft->setX(l);
    m_bottomLeft->setY(b);

    m_bottomRight->setX(r);
    m_bottomRight->setY(b);

    if (m_br->topLeft() == *m_topLeft)
    {
        m_points->operator[](0)=*m_topLeft;
        m_points->operator[](1)=*m_topRight;
        m_points->operator[](2)=*m_bottomRight;
        m_points->operator[](3)=*m_bottomLeft;
        m_points->operator[](4)=*m_topLeft;
    }
    else if (m_br->contains(*m_topLeft))
    {
        m_points->operator[](0)=QPoint(m_br->right()+1, t);
        m_points->operator[](1)=*m_topRight;
        m_points->operator[](2)=*m_bottomRight;
        m_points->operator[](3)=*m_bottomLeft;
        m_points->operator[](4)=QPoint(l, m_br->bottom()+1);
    }
    else if (m_br->contains(*m_topRight))
    {
        m_points->operator[](0)=QPoint(r, m_br->bottom()+1);
        m_points->operator[](1)=*m_bottomRight;
        m_points->operator[](2)=*m_bottomLeft;
        m_points->operator[](3)=*m_topLeft;
        m_points->operator[](4)=QPoint(m_br->left(), t);
    }
    else if (m_br->contains(*m_bottomLeft))
    {
        m_points->operator[](0)=QPoint(l, m_br->top());
        m_points->operator[](1)=*m_topLeft;
        m_points->operator[](2)=*m_topRight;
        m_points->operator[](3)=*m_bottomRight;
        m_points->operator[](4)=QPoint(m_br->right()+1, b);
    }
    else if (m_br->contains(*m_bottomRight))
    {
        m_points->operator[](0)=QPoint(m_br->left(), b);
        m_points->operator[](1)=*m_bottomLeft;
        m_points->operator[](2)=*m_topLeft;
        m_points->operator[](3)=*m_topRight;
        m_points->operator[](4)=QPoint(r, m_br->top());
    }
    else // 没有重合
    {
        m_points->operator[](0)=*m_topLeft;
        m_points->operator[](1)=*m_topRight;
        m_points->operator[](2)=*m_bottomRight;
        m_points->operator[](3)=*m_bottomLeft;
        m_points->operator[](4)=*m_topLeft;
    }

    QPainterPath path;
    path.moveTo(m_points->at(0));
    for (int i = 1; i < 5; ++i)
        path.lineTo(m_points->at(i));
    m_painter->begin(this);
    m_painter->drawPath(path);
    m_painter->end();
}

QSize RestoreWindowButton::sizeHint() const
{
    return *m_buttonSize;
}

void RestoreWindowButton::initGeometry()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setUnifyGeomerey(true);
}
