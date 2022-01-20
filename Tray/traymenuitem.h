#ifndef FACILEMENUITEM_H
#define FACILEMENUITEM_H

#include "Styles/InteractiveButtons/interactivebuttonbase.h"

class TrayMenu;
class TrayMenuItem;

typedef std::function<void()> const FuncType;
typedef std::function<void(int)> const FuncIntType;
typedef std::function<QString(QString)> const FuncStringStringType;
typedef std::function<void(TrayMenuItem*)> const FuncItemType;
typedef std::function<void(TrayMenuItem*, int)> const FuncItemIntType;

class TrayMenuItem : public InteractiveButtonBase
{
public:
    TrayMenuItem(QWidget* parent = Q_NULLPTR);
    TrayMenuItem(QString t, QWidget* parent = Q_NULLPTR);
    TrayMenuItem(QIcon i, QWidget* parent = Q_NULLPTR);
    TrayMenuItem(QIcon i, QString t, QWidget* parent = Q_NULLPTR);

    TrayMenuItem* setEnabled(bool e);
    TrayMenuItem* setCheckable(bool c);
    bool isCheckable() const;
    TrayMenuItem* setChecked(bool c);
    bool isChecked();

    void SetTooltipText(const QString &tooltipText);
    TrayMenuItem* disable(bool exp = true); // 满足情况下触发，不满足不变，下同
    TrayMenuItem* enable(bool exp = true);
    TrayMenuItem* hide(bool exp = true);
    TrayMenuItem* visible(bool exp = true);
    TrayMenuItem* check(bool exp = true);
    TrayMenuItem* uncheck(bool exp = true);
    TrayMenuItem* toggle(bool exp = true);
    TrayMenuItem* autoToggle();
    TrayMenuItem* text(bool exp, QString str);
    TrayMenuItem* text(bool exp, QString tru, QString fal);
    TrayMenuItem* fgColor(QColor color);
    TrayMenuItem* fgColor(bool exp, QColor color);
    TrayMenuItem* bgColor(QColor color);
    TrayMenuItem* bgColor(bool exp, QColor color);
    TrayMenuItem* prefix(bool exp, QString pfix);
    TrayMenuItem* suffix(bool exp, QString sfix, bool inLeftParenthesis = true);
    TrayMenuItem* prefix(QString pfix);
    TrayMenuItem* suffix(QString sfix, bool inLeftParenthesis = true);
    TrayMenuItem* icon(bool exp, QIcon icon);
    TrayMenuItem* borderR(int radius = 3, QColor co = Qt::transparent);
    TrayMenuItem* bind(bool &val);
    TrayMenuItem* longPress(FuncType func);
    TrayMenuItem* textAfterClick(QString newText);
    TrayMenuItem* textAfterClick(FuncStringStringType func);

    TrayMenuItem* ifer(bool exp);
    TrayMenuItem* elifer(bool exp);
    TrayMenuItem* elser();
    TrayMenuItem* exiter(bool exp = true);
    TrayMenuItem* ifer(bool exp, FuncItemType func, FuncItemType elseFunc = Q_NULLPTR);
    TrayMenuItem* switcher(int value);
    TrayMenuItem* caser(int value, FuncType func);
    TrayMenuItem* caser(int value);
    TrayMenuItem* breaker();
    TrayMenuItem* defaulter();

protected:
    virtual void drawIconBeforeText(QPainter &painter, QRect icon_rect) Q_DECL_OVERRIDE;

    TrayMenuItem* createTempItem(bool thisIsParent = true);

private:
    bool checkable = false;
    QString shortcut_tip = ""; // 快捷键提示
    TrayMenuItem* parent_menu_item_in_if = Q_NULLPTR; // elser/caser专用
    int switch_value = 0; // switcher的值，用来和caser比较（不需要breaker……）
    bool switch_matched = true;
    QVariant data;
};

#endif // FACILEMENUITEM_H
