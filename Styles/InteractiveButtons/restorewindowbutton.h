#ifndef STYLES_INTERACTIVEBUTTONS_RESTOREWINDOWBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_RESTOREWINDOWBUTTON_H

#include "interactivebuttonbase.h"

class RestoreWindowButton : public InteractiveButtonBase
{
public:
    explicit RestoreWindowButton(QWidget* parent);

    ~RestoreWindowButton();

    virtual void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:

    void InitGeometry();

private:
    const QPen m_pen;
    QPainter *m_painter;
    const QSize m_buttonSize;
    QRect *m_br;
    QPoint *m_topLeft, *m_topRight, *m_bottomLeft , *m_bottomRight;
    std::array<QPoint, 5> m_points;
};

#endif // STYLES_INTERACTIVEBUTTONS_RESTOREWINDOWBUTTON_H
