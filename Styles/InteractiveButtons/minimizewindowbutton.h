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

private:
    virtual void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    void InitGeometry();
    void InitDrawing();

private:
    QPen *m_pen;
    QPoint *m_left;
    QPoint *m_right;
    QPoint *m_mid;
    QSize *m_buttonSize;

};

#endif // STYLES_INTERACTIVEBUTTONS_MINIMIZEWINDOWBUTTON_H
