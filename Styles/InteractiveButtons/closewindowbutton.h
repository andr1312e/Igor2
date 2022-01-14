#ifndef STYLES_INTERACTIVEBUTTONS_CLOSEWINDOWBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_CLOSEWINDOWBUTTON_H

#include "interactivebuttonbase.h"

#include <QObject>

class CloseWindowButton : public InteractiveButtonBase
{
public:
    CloseWindowButton(QWidget* parent);
    ~CloseWindowButton();

    void setTopRightRadius(int radius);

protected:
    virtual void paintEvent(QPaintEvent*event) Q_DECL_OVERRIDE;
    virtual QPainterPath GetBackGroundPainterPath() Q_DECL_OVERRIDE;
    virtual QPainterPath getWaterPainterPath(Water water) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    int m_radius;
    QPen *m_pen;
    QSize *m_buttonSize;

private:
    void InitGeometry();
};

#endif // STYLES_INTERACTIVEBUTTONS_CLOSEWINDOWBUTTON_H
