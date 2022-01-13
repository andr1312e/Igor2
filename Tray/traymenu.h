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
#include <QAction>

#include "traymenuitem.h"
//#include "traymenubarinterface.h"

typedef std::function<void(int index, bool state)> const FuncCheckType;

class TrayMenu : public QMenu
{
    Q_OBJECT
public:
    explicit TrayMenu(QWidget *parent);
    ~TrayMenu();
public:
    TrayMenuItem * AddAction(const QIcon &icon, const QString& text, FuncType clicked = Q_NULLPTR);
    void AddAction(QAction* action);

    TrayMenu* BeginRow();
    TrayMenu* EndRow();
    QBoxLayout* GetCurrentLayout() const;
    void AddTextToMenu(const QString &text, int split = 0);
private:
    TrayMenu* InstertLabelIntoMenu(QLabel *widget, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
public:

    TrayMenu* AddLayout(QLayout *layout, int stretch = 0);
    TrayMenu* addLayoutItem(QLayoutItem *item);
    TrayMenu* addSpacerItem(QSpacerItem *spacerItem);
    TrayMenu* addSpacing(int size);
    TrayMenu* addStretch(int stretch = 0);
    TrayMenu* addStrut(int size);
    TrayMenu* setSpacing(int spacing);
    TrayMenu* setStretchFactor(QWidget *widget, int stretch);
    TrayMenu* setStretchFactor(QLayout *layout, int stretch);

    TrayMenuItem* addSeparator();
    TrayMenu* split();
    TrayMenuItem* lastAddedItem();
    bool hasFocus() const;

    int indexOf(TrayMenuItem* item);
    TrayMenuItem* at(int index) const;

    void exec(QPoint pos = QPoint(-1, -1));
    void exec(QRect expt, bool vertical = false, QPoint pos = QPoint(-1, -1));
    void execute();
    void toHide(int focusIndex = -1);
    void toClose();
    bool isClosedByClick() const;
    TrayMenu* finished(FuncType func);

    TrayMenu* addOptions(QList<QString>texts, QList<bool>states, FuncIntType clicked);
    TrayMenu* addOptions(QList<QString>texts, int select, FuncIntType clicked);
    TrayMenu* singleCheck(TrayMenuItem* item);
    TrayMenu* uncheckAll(TrayMenuItem* except = Q_NULLPTR, int begin = -1, int end = -1);
    QList<TrayMenuItem*> checkedItems();
    QList<int> checkedIndexes();
    QStringList checkedItemTexts();
    QList<QVariant> checkedItemDatas();
    TrayMenu* setSingleCheck(FuncCheckType clicked = Q_NULLPTR);
    TrayMenu* setMultiCheck(FuncCheckType clicked = Q_NULLPTR);

    TrayMenu* setTipArea(int x = 48);
    TrayMenu* setTipArea(QString longestTip);
    TrayMenu* setSplitInRow(bool split = true);
    TrayMenu* setBorderRadius(int r);

    TrayMenu* setAppearAnimation(bool en);
    TrayMenu* setDisappearAnimation(bool en);
    TrayMenu* setSubMenuShowOnCursor(bool en);

Q_SIGNALS:
    void ToTriggerAction(TrayMenuItem* action);
    void ToHide(); // 只是隐藏了自己

private Q_SLOTS:
    void OnItemMouseEntered(TrayMenuItem* item);

protected:
    TrayMenu(bool sub, QWidget* parent = Q_NULLPTR);
    TrayMenuItem* CreateMenuItem(QIcon icon, QString text);
    Qt::Key getShortcutByText(QString text) const;
    void setActionButton(InteractiveButtonBase* btn, bool isChip = false);
    void showSubMenu(TrayMenuItem* item);
    bool isCursorInArea(QPoint pos, TrayMenu* child = Q_NULLPTR);
    void setKeyBoardUsed(bool use = true);
    bool isSubMenu() const;
    TrayMenuItem* AddVerticalSeparator();
    void StartButtonAnimationOnShowedMainRectange();
    void startAnimationOnHidden(int focusIndex);

    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

public:
    static void SetTrayMenuColors(const QColor &normal, const QColor &hover, const QColor &press, const QColor &text);
    static QColor m_normalBackGroud; // Обычный фон (используется как глобальный для упрощения настройки)
    static QColor m_hoverBackgroud;  // плавающий фон
    static QColor m_pressBackgroud;  // 按下背景
    static QColor m_textColor;   // Цвет значка шрифта/обесцвечивания
    const static int m_blurAlphaValue=33; // Уровень отображения фонового изображения, 0 отключен, 1~100 — прозрачность размытия
    static QEasingCurve m_easingCurve; // кривая анимации, которая появляется
private:
    QList<TrayMenuItem*> m_trayMenuItems;
    QList<TrayMenuItem*> v_separators, m_horizontalSeparatorsList;//списки разделителей для анимации
    QList<QWidget*> m_addedLabels; // Добавленные вручную виджеты
    QHBoxLayout* m_mainHorizontalLayout;
    QVBoxLayout* m_mainVerticalLayout;
    QList<QHBoxLayout*> m_rowHorizontalLayoutsList;
    QList<QAction*> import_actions;
    QPixmap m_backgorudPixmap;

    TrayMenu* current_sub_menu = Q_NULLPTR; // Открытое в данный момент подменю (не обязательно отображаемое)
    TrayMenu* m_parentMenu = Q_NULLPTR; // меню родительского объекта
//    TrayMenuBarInterface* menu_bar = Q_NULLPTR; // интерфейс строки меню
    FuncType* finished_func = Q_NULLPTR; // выполнить перед уничтожением

    const int m_durationToAnimateElements = 700;
    bool hidden_by_another = false; // Заменяется ли оно другим подменю для отображения. Если нет, скрыть все меню
    const int item_padding = 8; // пустое пространство вокруг каждого элемента
    const int tip_area_spacing = 8; // Расстояние между телом элемента и tr
    bool m_addingHorizontMode = false; // добавляет горизонтальное меню
    bool m_oneItemToRowMode = false; // Выравнивать ли по центру, если только значок или текст
    bool linger_on_submenu_clicked = false; // После нажатия на подменю, скрыто ли родительское меню уровень за уровнем (обратите внимание, что если в подменю есть параметры, их нужно изменить вручную)
    bool m_startShowingAnimationMode = false;
    int m_currentElementIndex = -1; // текущий индекс
    bool using_keyboard = false; // используете ли вы клавиатуру для выбора меню
    QRect m_trayMenuRect;
    int m_trayMenuHeight = 0; // Высота окна, обновляется каждый раз, когда оно открывается
    QPoint _enter_later_pos = QPoint(-1, -1); // Не запускайте событие enterLater дважды подряд
    bool closed_by_clicked = false; // Он скрыт, потому что на него нажали, или закрыт по другим причинам?

    // Изменяемые свойства конфигурации
    int addin_tip_area = 48; // Область справа, используемая для отображения текста подсказки
    int border_radius = 5; // закругленные углы
    bool split_in_row = false; // Добавлять ли разделительную линию по умолчанию на той же строке
    bool enable_appear_animation = true;
    bool enable_disappear_animation = true;
    bool sub_menu_show_on_cursor = true; // Следит ли подменю за мышью или находится на краю главного меню

    // QWidget interface

};

#endif // FACILEMENU_H
