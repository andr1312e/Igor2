#include "traymenuitem.h"

#include <QPainterPath>

TrayMenuItem::TrayMenuItem(QWidget *parent)
    : InteractiveButton(parent)
{

}

TrayMenuItem::TrayMenuItem(const QString &text, QWidget *parent)
    : InteractiveButton(text, parent)
{

}

TrayMenuItem::TrayMenuItem(const QIcon &pixmapPath, QWidget *parent)
    : InteractiveButton(pixmapPath, parent)
{

}

TrayMenuItem::TrayMenuItem(const QIcon &pixmapPath, const QString &text, QWidget *parent)
    : InteractiveButton(pixmapPath, text, parent)
{

}

TrayMenuItem::~TrayMenuItem()
{

}

void TrayMenuItem::ChangeButtonIconEnabled()
{
    QString path = InteractiveButton::GetIconPath();
    QString stringEnd;
    if (path.endsWith("notchecked"))
    {
        stringEnd = "checked";
    }
    else
    {
        stringEnd = "notchecked";
    }
    const int firstIndex = path.indexOf('_');
    path.chop(path.count() - firstIndex);
    path.append(stringEnd);
    InteractiveButton::SetIcon(path);
}

void TrayMenuItem::ChangeButtonIconColor(ThemesNames themeName)
{
    QString path = InteractiveButton::GetIconPath();
    const int firstIndex = path.indexOf('_') + 1;
    const int secondIndex = path.indexOf('_', firstIndex);
    path.remove(firstIndex, secondIndex - firstIndex);
    switch (themeName)
    {
    case  ThemesNames::BlackTheme:
    {
        path.insert(firstIndex, "black");
        InteractiveButton::SetIcon(path);
        break;
    }
    case ThemesNames::AstraRedTheme:
    {
        path.insert(firstIndex, "colored");
        InteractiveButton::SetIcon(path);
        break;
    }
    case ThemesNames::AstraBlueTheme:
    {
        //теже что и выше поэтому не меняем
        return;
    }
    default:
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Нереализованное поведение")).toUtf8().constData());
    }
    }
}

bool TrayMenuItem::IsChecked() const noexcept
{
    return InteractiveButton::GetCheckedState();
}

void TrayMenuItem::SetEndAnimationGeometry(const QRect &topLeftPosition) noexcept
{
    m_endAnimationGeometry = topLeftPosition;
}

const QRect &TrayMenuItem::GetEndAnimationRect() const noexcept
{
    return  m_endAnimationGeometry;
}
