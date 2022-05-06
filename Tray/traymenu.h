#ifndef TRAY_TRAYMENU_H
#define TRAY_TRAYMENU_H

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
    void OnChangeButtonIcon();

public://Методы добавления кнопок и линий
    void AddButtonToMenu(QAction* action);
    void BeginInsertInRow();
    void EndInsertInRow();
    void AddTextToMenu(const QString &text);
    void AddSpacing(int size);
    void AddSeparatorLineHorizontal();
private:
    QBoxLayout* GetCurrentLayout() const;
    void InstertLabelIntoMenu(QLabel *labelWidget);
    TrayMenuItem* CreateMenuItem(QAction *action);

protected: //Методы переопределенные
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;//сначала вызвается closeEvent потом hideEvent
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
private:
    void SetStyleToButton(InteractiveButtonBase* button);
private:
    QPoint CalculateTopLeftWidgetPosOnScreen() const;
    void SetRoundedFormToWidget();
    void MakeTransparentImage();
private://Анимация показа
    void ShowAnimationPlay();
    QParallelAnimationGroup* AnimateDisclosureOfAllButtons();
private://Анимация сокрытия
    void HiddenAnimationPlay();
    QParallelAnimationGroup* AnimateFoldingOfAllButtons();

private:
    QColor m_normalBackGroud; // Обычный фон (используется как глобальный для упрощения настройки)

private:
    const QEasingCurve m_buttonsAnimatingCurve=QEasingCurve::OutBack; // анимации кнопок, которая появляется
    const QEasingCurve m_rowAnimationCurve=QEasingCurve::OutQuad; // анимации полосочек, которые появляются
private:
    QVector<TrayMenuItem*> m_trayMenuItems;

    QVector<TrayMenuItem*> m_horizontalSeparatorsList;//списки разделителей для анимации
    QVector<QLabel*> m_menuLabels; // Добавленные вручную виджеты

    QVBoxLayout* m_mainVerticalLayout;
    QVector<QHBoxLayout*> m_rowHorizontalLayoutsList;
    QImage m_backgroundImage;

private:
    const int m_showAnimateDurationInMiliseconds = 500;
    const int m_hideAnimateDurationInMiliseconds=300;
    const QString m_checkedImage=QStringLiteral("_checked.png");
    const QString m_notCheckedImage=QStringLiteral("_notchecked.png");
    const int m_itemPadding = 8; // пустое пространство вокруг каждого элемента
    const int m_alphaValue=70; // Уровень отображения фонового изображения, 0 отключен, 1~100 — прозрачность размытия
    const int m_borderRadius = 20; // закругленные углы
    const int m_blurRadius=64; //радиус размытия

    // Изменяемые свойства конфигурации
    bool m_addingHorizontMode = false; // добавляем в горизонте или кладём вертикально
};

#endif // TRAY_TRAYMENU_H
