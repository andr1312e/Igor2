#ifndef STYLES_INTERACTIVEBUTTONS_MINIMIZEWINDOWBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_MINIMIZEWINDOWBUTTON_H

#include "interactivebuttonbase.h"
#include <QPainterPath>
class MinimizeWindowButton : public InteractiveButtonBase
{
    Q_OBJECT
public:

    MinimizeWindowButton(QWidget* parent);

    ~MinimizeWindowButton();

    void paintEvent(QPaintEvent* event) override;

    QSize sizeHint() const override;

private:

    void initGeometry();

    void initDrawing();

    QPen *m_pen;

    QPainter *m_painter;

    QPoint *m_left;

    QPoint *m_right;

    QPoint *m_mid;

    QSize *m_buttonSize;

};

#endif // STYLES_INTERACTIVEBUTTONS_MINIMIZEWINDOWBUTTON_H
