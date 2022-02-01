#ifndef STYLES_WINDOWDRAGGER_H
#define STYLES_WINDOWDRAGGER_H

#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include <QStyleOption>

class WindowTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit WindowTitleBar(QWidget *parent);
    ~WindowTitleBar();

Q_SIGNALS:
    void ToDoubleClicked();

protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

protected:
    QPoint m_mousePosition;
    QPoint m_windowPosition;
    bool m_isMousePressed;
};

#endif // STYLES_WINDOWDRAGGER_H
