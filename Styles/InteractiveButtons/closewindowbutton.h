#ifndef STYLES_INTERACTIVEBUTTONS_CLOSEWINDOWBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_CLOSEWINDOWBUTTON_H

#include "interactivebuttonbase.h"

#include <QObject>

class CloseWindowButton : public InteractiveButtonBase
{
public:
    CloseWindowButton(QWidget* parent);

    ~CloseWindowButton();

    void setTopRightRadius(int r);

protected:
    void paintEvent(QPaintEvent*event);

    QPainterPath getBgPainterPath();
    QPainterPath getWaterPainterPath(Water water);

    QSize sizeHint() const override;

private:
    int tr_radius;
    QPen *m_pen;
    QPainter *m_painter;
    QSize *m_buttonSize;

private:
    void initGeometry();
    void initDrawing();
};

#endif // STYLES_INTERACTIVEBUTTONS_CLOSEWINDOWBUTTON_H
