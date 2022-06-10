#ifndef STYLES_INTERACTIVEBUTTONS_MAXIMIZEWINDOWBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_MAXIMIZEWINDOWBUTTON_H

#include "interactivebuttonbase.h"

class MaximizeWindowButton : public InteractiveButton
{
public:
    explicit MaximizeWindowButton(QWidget *parent);
    ~MaximizeWindowButton();

protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    void InitGeometry();

private:
    const QPen m_pen;
    const QSize m_buttonSize;
};

#endif // STYLES_INTERACTIVEBUTTONS_MAXIMIZEWINDOWBUTTON_H
