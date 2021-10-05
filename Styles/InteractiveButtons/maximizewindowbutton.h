#ifndef STYLES_INTERACTIVEBUTTONS_MAXIMIZEWINDOWBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_MAXIMIZEWINDOWBUTTON_H

#include "interactivebuttonbase.h"

class MaximizeWindowButton : public InteractiveButtonBase
{
public:
    MaximizeWindowButton(QWidget* parent);

    ~MaximizeWindowButton();

protected:
    void paintEvent(QPaintEvent*event) override;

    QSize sizeHint() const override;

private:

    void initGeometry();

    void initDrawing();

    QPen *m_pen;

    QPainter *m_painter;
};

#endif // STYLES_INTERACTIVEBUTTONS_MAXIMIZEWINDOWBUTTON_H
