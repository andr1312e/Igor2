#ifndef STYLES_THEMES_THEMECOLORS_H
#define STYLES_THEMES_THEMECOLORS_H

#include <QPalette>

const QVector<QPair<QPalette::ColorRole, QColor>> darkThemeColor = {
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

const QVector<QPair<QPalette::ColorRole, QColor>> darkThemeDisabledColor = {
   QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, QColor(127, 127, 127)),
   QPair<QPalette::ColorRole, QColor>(QPalette::Text, QColor(127, 127, 127)),
   QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText, QColor(127, 127, 127)),
   QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(80, 80, 80)),
   QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText, QColor(127, 127, 127)),
};

const QVector<QPair<QPalette::ColorRole, QColor>> astraThemeColor = {
   QPair<QPalette::ColorRole, QColor>(QPalette::Window, QColor(255, 255, 255)), // цвет фона приложения
   QPair<QPalette::ColorRole, QColor>(QPalette::Base, QColor(214, 214, 214)), //цвет фона листвиджетов и линеедитов
   QPair<QPalette::ColorRole, QColor>(QPalette::AlternateBase, QColor(146, 146, 146)),//цвет скролла
   QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, Qt::black),//цвет текста
   QPair<QPalette::ColorRole, QColor>(QPalette::Text, Qt::black), //цвет текста в лайн дитах
   QPair<QPalette::ColorRole, QColor>(QPalette::Shadow, QColor(158, 158, 158)),//заголовок
   QPair<QPalette::ColorRole, QColor>(QPalette::Button, QColor(218, 220, 225)),
   QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText, QColor(15, 0, 5)),
   QPair<QPalette::ColorRole, QColor>(QPalette::BrightText, QColor(17, 100, 19)),
   QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(255, 255, 255)), //обводочка
   QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText, Qt::black),//поле ид пользователя в листе
   QPair<QPalette::ColorRole, QColor>(QPalette::Dark, Qt::white),//не используется
   QPair<QPalette::ColorRole, QColor>(QPalette::ToolTipBase, QColor(158, 158, 158)),//подстветочка
   QPair<QPalette::ColorRole, QColor>(QPalette::LinkVisited, Qt::green),//не используется
   QPair<QPalette::ColorRole, QColor>(QPalette::Link, QColor(255, 255, 255)), //тайтл у листов
};

const QVector<QPair<QPalette::ColorRole, QColor>> astraThemeDisabledColor = {
   QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, QColor(127, 127, 127)),
   QPair<QPalette::ColorRole, QColor>(QPalette::Text, QColor(127, 127, 127)),
   QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText, QColor(127, 127, 127)),
   QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(80, 80, 80)),
   QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText, QColor(127, 127, 127)),
};

#endif // STYLES_THEMES_THEMECOLORS_H
