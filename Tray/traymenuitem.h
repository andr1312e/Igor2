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
    TrayMenuItem(const QString &text, QWidget* parent = Q_NULLPTR);
    TrayMenuItem(const QIcon &pixmapPath, QWidget* parent = Q_NULLPTR);
    TrayMenuItem(const QIcon &pixmapPath, const QString &text, QWidget* parent = Q_NULLPTR);

    void SetIconPath(const QString &imagePath);
    QString GetIconPath(){ return m_pixmapPath;}
    TrayMenuItem* setEnabled(bool e);
    void SetCheckable(bool c);
    bool IsCheckable() const;
    void SetChecked(bool c);
    bool IsChecked();

    void SetTooltipText(const QString &tooltipText);
    TrayMenuItem* disable(bool exp = true); // 满足情况下触发，不满足不变，下同
    TrayMenuItem* enable(bool exp = true);
    TrayMenuItem* hide(bool exp = true);
    TrayMenuItem* visible(bool exp = true);
    void Check(bool exp = true);
    TrayMenuItem* toggle(bool exp = true);
    TrayMenuItem* autoToggle();
    TrayMenuItem* text(bool exp, QString str);
    TrayMenuItem* text(bool exp, QString tru, QString fal);
    TrayMenuItem* fgColor(QColor color);
    TrayMenuItem* fgColor(bool exp, QColor color);
    TrayMenuItem* bgColor(QColor color);
    TrayMenuItem* bgColor(bool exp, QColor color);
    TrayMenuItem* borderR(int radius = 3, QColor co = Qt::transparent);
    TrayMenuItem* bind(bool &val);
    TrayMenuItem* longPress(FuncType func);

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

private:
    TrayMenuItem* createTempItem(bool thisIsParent = true);

private:
    bool m_isCheckable = false;
    QString shortcut_tip = ""; // 快捷键提示
    TrayMenuItem* parent_menu_item_in_if = Q_NULLPTR; // elser/caser专用
    int switch_value = 0; // switcher的值，用来和caser比较（不需要breaker……）
    bool switch_matched = true;
    QVariant data;
    QString m_pixmapPath;
};

#endif // FACILEMENUITEM_H
