#ifndef STYLES_INTERACTIVEBUTTONS_CLOSEWINDOWBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_CLOSEWINDOWBUTTON_H

#include "interactivebuttonbase.h"

#include <QObject>

class CloseWindowButton : public InteractiveButton
{
public:
    explicit CloseWindowButton(QWidget *parent);
    ~CloseWindowButton();

private:
    void InitGeometry();

public:
    void setTopRightRadius(int radius);

protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual QPainterPath GetBackGroundPainterPath() Q_DECL_OVERRIDE;
    virtual QPainterPath getWaterPainterPath(Water water) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    int m_radius;
    const QPen m_pen;
    const QSize m_buttonSize;
};

#endif // STYLES_INTERACTIVEBUTTONS_CLOSEWINDOWBUTTON_H
