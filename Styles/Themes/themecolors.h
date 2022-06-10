#ifndef STYLES_THEMES_THEMECOLORS_H
#define STYLES_THEMES_THEMECOLORS_H

#include <QMap>
#include <QPalette>
namespace
{
const QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 15> darkThemeColor =
{
    QPair<QPalette::ColorRole, QColor>(QPalette::Window, QColor(53, 53, 53)),
    QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, Qt::white),
    QPair<QPalette::ColorRole, QColor>(QPalette::Base, QColor(42, 42, 42)),
    QPair<QPalette::ColorRole, QColor>(QPalette::AlternateBase, QColor(66, 66, 66)),
    QPair<QPalette::ColorRole, QColor>(QPalette::ToolTipBase, Qt::white),
    QPair<QPalette::ColorRole, QColor>(QPalette::ToolTipText, QColor(53, 53, 53)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Text, Qt::white),
    QPair<QPalette::ColorRole, QColor>(QPalette::Dark, QColor(35, 35, 35)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Shadow, QColor(20, 20, 20)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Button, QColor(53, 53, 53)),
    QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText, Qt::white),
    QPair<QPalette::ColorRole, QColor>(QPalette::BrightText, Qt::red),
    QPair<QPalette::ColorRole, QColor>(QPalette::Link, QColor(66, 66, 66)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(42, 130, 218)),
    QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText, Qt::gray),
};

const QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 5> darkThemeDisabledColor =
{
    QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Text, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(80, 80, 80)),
    QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText, QColor(127, 127, 127)),
};

const QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 15> astraThemeColor =
{
    QPair<QPalette::ColorRole, QColor>(QPalette::Window, QColor(255, 255, 255)), // цвет фона приложения
    QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, Qt::black),//цвет текста
    QPair<QPalette::ColorRole, QColor>(QPalette::Base, QColor(242, 242, 242)), //цвет фона листвиджетов и линеедитов
    QPair<QPalette::ColorRole, QColor>(QPalette::AlternateBase, QColor(173, 0, 0)),//цвет скролла
    QPair<QPalette::ColorRole, QColor>(QPalette::Text, Qt::black), //цвет текста в лайн дитах
    QPair<QPalette::ColorRole, QColor>(QPalette::Shadow, QColor(137, 0, 0)),//заголовок
    QPair<QPalette::ColorRole, QColor>(QPalette::Button, QColor(218, 220, 225)),//цвет к
    QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText, QColor(15, 0, 5)),
    QPair<QPalette::ColorRole, QColor>(QPalette::BrightText, QColor(17, 100, 19)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(86, 0, 0)), //обводочка
    QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText, Qt::white),//поле ид пользователя в листе
    QPair<QPalette::ColorRole, QColor>(QPalette::Dark, Qt::white),//не используется
    QPair<QPalette::ColorRole, QColor>(QPalette::ToolTipBase, QColor(158, 158, 158)),//подстветочка
    QPair<QPalette::ColorRole, QColor>(QPalette::LinkVisited, QColor(176, 85, 85)),//не используется
    QPair<QPalette::ColorRole, QColor>(QPalette::Link, QColor(255, 255, 255)), //тайтл у листов
};

const QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 5> astraThemeDisabledColor =
{
    QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Text, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(80, 80, 80)),
    QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText, QColor(127, 127, 127)),
};

}

enum class ThemesNames
{
    BlackTheme = 0,
    AstraRedTheme = 1,
    AstraBlueTheme = 2
};

Q_DECLARE_METATYPE(ThemesNames)

static const QMap<ThemesNames, QPair<QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 15>, QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 5>>> themesList =
{
    {ThemesNames::BlackTheme, qMakePair(darkThemeColor, darkThemeDisabledColor)},
    {ThemesNames::AstraRedTheme, qMakePair(astraThemeColor, astraThemeDisabledColor)}
};

#endif // STYLES_THEMES_THEMECOLORS_H
