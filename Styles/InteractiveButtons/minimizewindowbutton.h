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

private:
    const QPen * const m_pen;
    QPoint * const m_left;
    QPoint * const m_right;
    QPoint * const m_mid;
    const QSize * const m_buttonSize;

};

#endif // STYLES_INTERACTIVEBUTTONS_MINIMIZEWINDOWBUTTON_H
