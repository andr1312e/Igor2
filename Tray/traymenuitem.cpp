#include "traymenuitem.h"

#include <QPainterPath>

TrayMenuItem::TrayMenuItem(QWidget *parent)
    : InteractiveButtonBase(parent)
{

}

TrayMenuItem::TrayMenuItem(const QString& text, QWidget *parent)
    : InteractiveButtonBase(text, parent)
{

}

TrayMenuItem::TrayMenuItem(const QIcon& pixmap, QWidget *parent)
    : InteractiveButtonBase(pixmap, parent)
{

}

TrayMenuItem::TrayMenuItem(const QIcon& pixmap, const QString& text, QWidget *parent)
    : InteractiveButtonBase(pixmap, text, parent)
{

}

void TrayMenuItem::SetIconPath(const QString &imagePath)
{
    m_pixmapPath=imagePath;
    setIcon(QIcon(imagePath));
}

TrayMenuItem *TrayMenuItem::setEnabled(bool e)
{
    InteractiveButtonBase::setEnabled(e);
    return this;
}

void TrayMenuItem::SetCheckable(bool c)
{
    m_isCheckable = c;
    m_isCheckedState=false;
}

bool TrayMenuItem::IsCheckable() const
{
    return m_isCheckable;
}

void TrayMenuItem::SetChecked(bool c)
{
    m_isCheckedState = c;
}

bool TrayMenuItem::IsChecked()
{
    return getState();
}

void TrayMenuItem::SetTooltipText(const QString &tooltipText)
{
    QWidget::setToolTip(tooltipText);
}

TrayMenuItem *TrayMenuItem::disable(bool exp)
{
    if (exp)
        setDisabled(true);
    return this;
}

TrayMenuItem *TrayMenuItem::enable(bool exp)
{
    if (exp)
        setEnabled(true);
    return this;
}

TrayMenuItem *TrayMenuItem::hide(bool exp)
{
    if (exp)
        InteractiveButtonBase::hide();
    return this;
}

/**
 * 默认就是show状态
 * 为了和show区分开
 */
TrayMenuItem *TrayMenuItem::visible(bool exp)
{
    if (exp)
        InteractiveButtonBase::setVisible(true);
    return this;
}

void TrayMenuItem::Check(bool exp)
{
    SetChecked(exp);
}

/**
 * 切换状态
 * 如果选中了，则取消选中；反之亦然
 * （本来打算不只是选中状态，然而还没想到其他有什么能切换的）
 */
TrayMenuItem *TrayMenuItem::toggle(bool exp)
{
    if (!exp)
        return this;
    if (IsCheckable())
    {
        SetChecked(!IsChecked());
    }
    // 以后什么功能想到再加
    return this;
}

/**
 * 点击自动切换状态
 * 小心点，因为信号槽顺序的关系，若放在triggered后面，可能会出现相反的check
 * 建议只用于和顺序无关的自动切换
 * （还不知道怎么修改信号槽的调用顺序）
 */
TrayMenuItem *TrayMenuItem::autoToggle()
{
    connect(this, &InteractiveButtonBase::clicked, this, [=]{
        toggle();
    });
    return this;
}

TrayMenuItem *TrayMenuItem::text(bool exp, QString str)
{
    if (exp)
    {
        // 去掉快捷键符号
        // 注意：这里设置文字不会改变原来的快捷键！
        setText(str.replace(QRegExp("&([\\w\\d])\\b"), "\\1"));
        // 调整大小
        setFixedForeSize();
    }
    return this;
}

/**
 * 设置字符串，成立时 tru，不成立时 fal
 * 注意：这里是直接设置完整的文字，不会去掉快捷键&符号
 */
TrayMenuItem *TrayMenuItem::text(bool exp, QString tru, QString fal)
{
    if (exp)
        setText(tru);
    else
        setText(fal);
    return this;
}

TrayMenuItem *TrayMenuItem::fgColor(QColor color)
{
    SetTextColor(color);
    return this;
}

TrayMenuItem *TrayMenuItem::fgColor(bool exp, QColor color)
{
    if (exp)
        return fgColor(color);
    return this;
}

TrayMenuItem *TrayMenuItem::bgColor(QColor color)
{
    setBgColor(color);
    return this;
}

TrayMenuItem *TrayMenuItem::bgColor(bool exp, QColor color)
{
    if (exp)
        bgColor(color);
    return this;
}

TrayMenuItem *TrayMenuItem::borderR(int radius, QColor co)
{
    setRadius(radius);
    if (co != Qt::transparent)
        setBorderColor(co);
    else
        setBorderColor(press_bg);
    return this;
}

/**
 * 绑定某一布尔类型的变量（只能全局变量）
 * 点击即切换值
 * 注意：因为是异步的，局部变量会导致崩溃！
 */
TrayMenuItem *TrayMenuItem::bind(bool &val)
{
    connect(this, &InteractiveButtonBase::clicked, this, [&]{
        val = !val;
    });
    return this;
}

/**
 * 短期长按效果
 * 该操作不会影响其它任何交互效果
 * 即不会隐藏菜单，也不会解除单击信号
 */
TrayMenuItem *TrayMenuItem::longPress(FuncType func)
{
    connect(this, &InteractiveButtonBase::ToMousePressLater, this, [=](QMouseEvent*){
        func();
    });
    return this;
}

/**
 * 适用于连续设置
 * 当 iff 成立时继续
 * 否则取消后面所有设置
 */
TrayMenuItem *TrayMenuItem::ifer(bool exp)
{
    if (exp)
        return this;

    // 返回一个无用item，在自己delete时也delete掉
    return createTempItem();
}

/**
 * 完全等于 ifer
 * 如果已经在 ifer 里面，则先退出
 */
TrayMenuItem *TrayMenuItem::elifer(bool exp)
{
    if (parent_menu_item_in_if) // ifer 不成立后的，退出并转至新的 ifer
        return parent_menu_item_in_if->ifer(exp);
    return ifer(exp); // 直接使用，完全等同于 ifer
}

TrayMenuItem *TrayMenuItem::elser()
{
    if (parent_menu_item_in_if)
        return parent_menu_item_in_if;
    return createTempItem();
}

/**
 * 适用于连续设置action时，满足条件则退出
 * 相当于一个控制语句
 * 当ex成立时，取消后面所有设置
 */
TrayMenuItem *TrayMenuItem::exiter(bool exp)
{
    if (!exp)
        return this;

    // 返回一个无用item，在自己delete时也delete掉
    return createTempItem(false);
}

/**
 * 适用于连续设置
 * 满足某一条件则执行 func(this)
 */
TrayMenuItem *TrayMenuItem::ifer(bool exp, FuncItemType func, FuncItemType elseFunc)
{
    if (exp)
    {
        if (func)
            func(this);
    }
    else
    {
        if (elseFunc)
            elseFunc(this);
    }
    return this;
}

/**
 * 适用于连续设置
 * 类似 switch 语句，输入判断的值
 * 当后续的 caser 满足 value 时，允许执行 caser 的 func 或后面紧跟着的的设置
 */
TrayMenuItem *TrayMenuItem::switcher(int value)
{
    switch_value = value;
    switch_matched = false;
    return this;
}

/**
 * 当 value 等同于 switcher 判断的 value 时，执行 func
 * 并返回原始 item
 * 注意与重载的 caser(int) 进行区分
 */
TrayMenuItem *TrayMenuItem::caser(int value, FuncType func)
{
    if (value == switch_value)
    {
        switch_matched = true;
        if (func)
            func();
    }
    return this;
}

/**
 * 当 value 等同于 switcher 的 value 时，返回原始 item
 * 即执行 caser 后面的设置，直至 breaker
 * 注意与重载的 caser(int FuncType) 进行区分
 */
TrayMenuItem *TrayMenuItem::caser(int value)
{
    // 可能已经接着一个没有 breaker 的 caser
    // 则回到上一级（这样会导致无法嵌套）
    if (this->parent_menu_item_in_if)
    {
        // 接着一个 !=的caser 后面
        if (value == parent_menu_item_in_if->switch_value)
        {
            parent_menu_item_in_if->switch_matched = true;
            return parent_menu_item_in_if; // 真正需要使用的实例
        }
        return this; // 继续使用自己（一个临时实例）
    }
    else // 自己是第一个 caser 或者 ==的caser 后面
    {
        if (value == switch_value)
        {
            switch_matched = true;
            return this;
        }
        return createTempItem();
    }
}

/**
 * caser 的 value 不等于 switcher 的 value 时
 * 此语句用来退出
 */
TrayMenuItem *TrayMenuItem::breaker()
{
    if (parent_menu_item_in_if)
        return parent_menu_item_in_if;
    return this; // 应该不会吧……
}

/**
 * 如果switcher的caser没有满足
 */
TrayMenuItem *TrayMenuItem::defaulter()
{
    if (switch_matched) // 已经有 caser 匹配了
        return createTempItem(); // 返回无效临时实例
    return this; // 能用，返回自己
}

TrayMenuItem *TrayMenuItem::createTempItem(bool thisIsParent)
{
    auto useless = new TrayMenuItem(QIcon(), "", this);
    useless->parent_menu_item_in_if = thisIsParent ? this : nullptr;
    useless->hide();
    useless->setEnabled(false);
    useless->setMinimumSize(0, 0);
    useless->setFixedSize(0, 0);
    useless->move(-999, -999);
    return useless;
}
