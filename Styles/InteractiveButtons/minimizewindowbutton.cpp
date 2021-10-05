#include "minimizewindowbutton.h"

MinimizeWindowButton::MinimizeWindowButton(QWidget* parent)
    : InteractiveButtonBase(parent)
    , m_left(new QPoint())
    , m_right(new QPoint())
    , m_mid(new QPoint())
    , m_buttonSize(new QSize(40, 40))
{
    initGeometry();
    initDrawing();
}

MinimizeWindowButton::~MinimizeWindowButton()
{
    delete m_pen;
    delete m_painter;
    delete m_left;
    delete m_mid;
    delete m_right;
    delete m_buttonSize;
}

void MinimizeWindowButton::paintEvent(QPaintEvent* event)
{
    if (!show_foreground) return ;

    int w = _w, h = _h;
    m_left->setX(_l+w/3);
    m_left->setY(_t+h/2);

    m_right->setX(_l+w*2/3);
    m_right->setY(_t+h/2);

    m_mid->setX(_l+w/2+offset_pos.x());
    m_mid->setY(_t+h/2+offset_pos.y());

    QPainterPath path;
    path.moveTo(*m_left);
    path.cubicTo(*m_left, *m_mid, *m_right);
    if (m_left->y() != m_mid->y())
        m_painter->setRenderHint(QPainter::Antialiasing,true);
    m_painter->begin(this);
    m_painter->drawPath(path);
    m_painter->end();
}

QSize MinimizeWindowButton::sizeHint() const
{
    return  *m_buttonSize;
}

void MinimizeWindowButton::initGeometry()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setUnifyGeomerey(true);
}

void MinimizeWindowButton::initDrawing()
{
    m_pen=new QPen(Qt::white);
    m_painter=new QPainter(this);
    m_painter->setPen(*m_pen);
}
