#ifndef FACILEMENU_H
#define FACILEMENU_H

#include <functional>

#include <QObject>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QThread>
#include <QMenu>
#include <QLabel>
#include <QPalette>
#include <QAction>
#include <QGraphicsDropShadowEffect>
#include <QPainterPath>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

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
    void AddButtonToMenu(QAction* action);
    void BeginInsertInRow();
    void EndInsertInRow();
    void AddTextToMenu(const QString &text);
    void AddSpacing(int size);
    void AddSeparatorLineHorizontal();

protected:
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;//сначала вызвается closeEvent потом hideEvent
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
    QBoxLayout* GetCurrentLayout() const;
    void InstertLabelIntoMenu(QLabel *labelWidget);
    void SetTrayMenuColors(const QColor& normalColor, const QColor&  hoverColor, const QColor&  pressColor, const QColor&  textColor);
    TrayMenuItem* CreateMenuItem(QAction *action);
    void SetStyleToButton(InteractiveButtonBase* button);
private:

    void CalculateWidgetRectOnScreen();
    void SetRoundedFormToWidget();
    void MakeTransparentImage();
    void ChangeBackgroundImageAplha(int &alpha);
    QPoint CalculateStartAndEndAnimationPostionFromUserMousePos() const;
    void ShowAnimationPlay();
    void HiddenAnimationPlay();

private:
    void HideHorizontalSeparates();
private:
    QParallelAnimationGroup* AnimateDisclosureOfAllButtons(const QPoint &startPosition);
    void AnimateDisclosureHorizontalSeparates();
    void AnimateFoldingHorizontalSeparates();
    QParallelAnimationGroup* AnimateFoldingOfAllButtons(const QPoint &startPosition);


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
    std::vector<QHBoxLayout*> m_rowHorizontalLayoutsList;
    QImage m_backgroundImage;

private:
    const QRect m_availableGeometry;
    const int m_showAnimateDurationInMiliseconds=700;
    const int m_hideAnimateDurationInMiliseconds=400;
    const int m_itemPadding = 8; // пустое пространство вокруг каждого элемента
    const int m_alphaValue=70; // Уровень прозрачности
    const int m_borderRadius = 20; // закругленные углы
    const int m_blurRadius=64; //радиус размытия
    // Изменяемые свойства конфигурации
    bool m_addingHorizontMode = false; // добавляем горизонте или кладем вертикально
    bool m_isAnimationMode = false;
};

#endif // FACILEMENU_H
