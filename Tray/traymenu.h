#ifndef FACILEMENU_H
#define FACILEMENU_H

#include <functional>

#include <QObject>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QMenu>
#include <QLabel>
#include <QPalette>
#include <QAction>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>

#include "traymenuitem.h"

class TrayMenu : public QMenu
{
    Q_OBJECT
public:
    explicit TrayMenu(QWidget *parent);
    ~TrayMenu();

public Q_SLOTS:
    void OnUpdateViewColors();
private Q_SLOTS:
    void OnItemMouseEntered(TrayMenuItem* item);

public:
    void AddAction(QAction* action);
    TrayMenu* BeginInsertInRow();
    TrayMenu* EndInsertInRow();
    void AddTextToMenu(const QString &text);
    void AddSpacing(int size);
    void AddSeparatorLineHorizontal();
protected:
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
    QBoxLayout* GetCurrentLayout() const;
    TrayMenu* InstertLabelIntoMenu(QLabel *labelWidget);
    void SetTrayMenuColors(const QColor& normalColor, const QColor&  hoverColor, const QColor&  pressColor, const QColor&  textColor);
    TrayMenuItem* CreateMenuItem(const QIcon &icon,const QString &text);
    void SetStyleToButton(InteractiveButtonBase* button);
private:
    void CalculateWidgetRectOnScreen();
    void SetRoundedRectToWidgetBackground();
    void MakeBlurImageOnBackGroundForPaintEvent(QScreen *screen, const QRect &widgetRect, const int &radius);
    void StartButtonAnimationOnShowedMainRectange();
    void StartAnimationOnHidden(int focusIndex);

private:
    QColor m_normalBackGroud; // Обычный фон (используется как глобальный для упрощения настройки)
    QColor m_hoverBackgroud;  // при наведении фон
    QColor m_pressBackgroud;  // при нажатии фон
    QColor m_textColor;       // Цвет шрифта/обесцвечивания и полосочек
private:
    const QEasingCurve m_buttonsAnimatingCurve; // анимации кнопок, которая появляется
    const QEasingCurve m_rowAnimationCurve; // анимации полосочек, которые появляются
private:
    std::vector<TrayMenuItem*> m_trayMenuItems;
    std::vector<TrayMenuItem*>::iterator m_currentElementIndex=m_trayMenuItems.end(); // текущий индекс

    std::vector<TrayMenuItem*> m_horizontalSeparatorsList;//списки разделителей для анимации
    std::vector<QLabel*> m_menuLabels; // Добавленные вручную виджеты
    QHBoxLayout* m_mainHorizontalLayout;
    QVBoxLayout* m_mainVerticalLayout;
    std::list<QHBoxLayout*> m_rowHorizontalLayoutsList;
    QPixmap m_backgroundPixmap;

private:
    const QRect m_availableGeometry;
    const int m_durationToAnimateElementsMiliseconds = 700;
    const int m_itemPadding = 8; // пустое пространство вокруг каждого элемента
    const int m_blurAlphaValue=30; // Уровень отображения фонового изображения, 0 отключен, 1~100 — прозрачность размытия
    const int m_borderRadius = 20; // закругленные углы
    QRect m_trayMenuPosition;

    // Изменяемые свойства конфигурации
    bool m_addingHorizontMode = false; // добавляем горизонте или кладем вертикально
    bool m_startShowingAnimationMode = false;
    bool m_enableDisappearAnimation = true;
};

#endif // FACILEMENU_H
