#ifndef STYLES_INTERACTIVEBUTTONS_MAXIMIZEWINDOWBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_MAXIMIZEWINDOWBUTTON_H

#include "interactivebuttonbase.h"

class MaximizeWindowButton : public InteractiveButtonBase
{
public:
    MaximizeWindowButton(QWidget* parent);
    ~MaximizeWindowButton();

protected:
    virtual void paintEvent(QPaintEvent*event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    void InitGeometry();

private:
    QPen *m_pen;
};

#endif // STYLES_INTERACTIVEBUTTONS_MAXIMIZEWINDOWBUTTON_H
