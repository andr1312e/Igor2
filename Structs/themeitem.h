#ifndef STRUCTS_THEMEITEM_H
#define STRUCTS_THEMEITEM_H

#include <QString>
#include <QIcon>

#include "Styles/Themes/themecolors.h"

struct ThemeItem
{
    explicit ThemeItem()
        : themeName()
        , nextThemeToolTip()
        , themeIcon()
    {

    }
    explicit ThemeItem(ThemesNames _themeName, const QString &_nextThemeToolTip, const QString &_themeIcon)
        : themeName(_themeName)
        , nextThemeToolTip(_nextThemeToolTip)
        , themeIcon(_themeIcon)
    {

    }

    ThemeItem(const ThemeItem &another)
        : themeName(another.themeName)
        , nextThemeToolTip(another.nextThemeToolTip)
        , themeIcon(another.themeIcon)
    {

    }

    ~ThemeItem()
    {

    }

    ThemesNames themeName;
    QString nextThemeToolTip;
    QIcon themeIcon;
};


Q_DECLARE_TYPEINFO(ThemeItem, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(ThemeItem)

#endif // STRUCTS_THEMEITEM_H
