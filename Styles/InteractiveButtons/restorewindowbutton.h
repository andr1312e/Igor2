#ifndef STYLES_INTERACTIVEBUTTONS_RESTOREWINDOWBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_RESTOREWINDOWBUTTON_H

#include "interactivebuttonbase.h"

class RestoreWindowButton : public InteractiveButtonBase
{
public:
    RestoreWindowButton(QWidget* parent);

    ~RestoreWindowButton();

    void paintEvent(QPaintEvent* event) override;

    QSize sizeHint() const override;

private:

    void InitGeometry();

private:
    QPen *m_pen;
    QPainter *m_painter;
    QSize *m_buttonSize;
    QRect *m_br;
    QPoint *m_topLeft, *m_topRight, *m_bottomLeft , *m_bottomRight;
    std::array<QPoint, 5> *m_points;
};

#endif // STYLES_INTERACTIVEBUTTONS_RESTOREWINDOWBUTTON_H
