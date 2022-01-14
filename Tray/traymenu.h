#ifndef FACILEMENU_H
#define FACILEMENU_H

#include <QObject>
#include <QDialog>
#include <functional>
#include <QRegularExpression>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>
#include <QMenu>
#include <QLabel>
#include <QPalette>
#include <QAction>

#include "traymenuitem.h"

typedef std::function<void(int index, bool state)> const FuncCheckType;

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
    TrayMenu* BeginRow();
    TrayMenu* EndRow();
    void AddTextToMenu(const QString &text, int split = 0);
private:
    TrayMenuItem * AddAction(const QIcon &icon, const QString& text, FuncType clicked = Q_NULLPTR);
    QBoxLayout* GetCurrentLayout() const;
    TrayMenu* InstertLabelIntoMenu(QLabel *labelWidget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
public:
    void AddSpacing(int size);

    void AddSeparatorLineHorizontal();
    TrayMenu* split();
    TrayMenuItem* lastAddedItem();
    bool HasFocus() const;

    std::vector<TrayMenuItem*>::iterator indexOf(TrayMenuItem* item);
    TrayMenuItem* at(int index) const;

    void exec(QPoint pos = QPoint(-1, -1));
    void exec(QRect expt, bool vertical = false, QPoint pos = QPoint(-1, -1));
    void execute();
    void toHide(std::vector<TrayMenuItem*>::iterator focusIndex);
    void toClose();

    TrayMenu* setAppearAnimation(bool en);
    TrayMenu* setDisappearAnimation(bool en);

private:
    void SetTrayMenuColors(const QColor& normalColor, const QColor&  hoverColor, const QColor&  pressColor, const QColor&  textColor);
protected:
    TrayMenuItem* CreateMenuItem(QIcon icon, QString text);
    void setActionButton(InteractiveButtonBase* btn, bool isChip = false);
    bool isCursorInArea(QPoint pos, TrayMenu* child = Q_NULLPTR);
    TrayMenuItem* AddVerticalSeparator();
private:
    void StartButtonAnimationOnShowedMainRectange();
protected:
    void startAnimationOnHidden(int focusIndex);

    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

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
    std::vector<TrayMenuItem*> m_horizontalSeparatorsList;//списки разделителей для анимации
    std::vector<QLabel*> m_menuLabels; // Добавленные вручную виджеты
    QHBoxLayout* m_mainHorizontalLayout;
    QVBoxLayout* m_mainVerticalLayout;
    QList<QHBoxLayout*> m_rowHorizontalLayoutsList;
    QPixmap m_backgroundPixmap;

    TrayMenu* m_parentMenu = Q_NULLPTR; // меню родительского объекта

    const int m_durationToAnimateElements = 700;
    bool hidden_by_another = false; // Заменяется ли оно другим подменю для отображения. Если нет, скрыть все меню
    const int item_padding = 8; // пустое пространство вокруг каждого элемента
    const int tip_area_spacing = 8; // Расстояние между телом элемента и tr
    bool m_addingHorizontMode = false; // добавляет горизонтальное меню
    bool m_manyItemToRowMode = false; // Выравнивать ли по центру, если только значок или текст
    bool linger_on_submenu_clicked = false; // После нажатия на подменю, скрыто ли родительское меню уровень за уровнем (обратите внимание, что если в подменю есть параметры, их нужно изменить вручную)
    bool m_startShowingAnimationMode = false;
    std::vector<TrayMenuItem*>::iterator m_currentElementIndex; // текущий индекс
    bool using_keyboard = false; // используете ли вы клавиатуру для выбора меню
    QRect m_trayMenuRect;
    int m_trayMenuHeight = 0; // Высота окна, обновляется каждый раз, когда оно открывается
    QPoint _enter_later_pos = QPoint(-1, -1); // Не запускайте событие enterLater дважды подряд
    bool closed_by_clicked = false; // Он скрыт, потому что на него нажали, или закрыт по другим причинам?

    // Изменяемые свойства конфигурации
    int addin_tip_area = 48; // Область справа, используемая для отображения текста подсказки
    const int m_blurAlphaValue=30; // Уровень отображения фонового изображения, 0 отключен, 1~100 — прозрачность размытия
    const int m_borderRadius = 20; // закругленные углы
    bool split_in_row = false; // Добавлять ли разделительную линию по умолчанию на той же строке
    bool m_buttonAnimationIsEnabled = true;
    bool m_enableDisappearAnimation = true;
};

#endif // FACILEMENU_H
