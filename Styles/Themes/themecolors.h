#ifndef THEMECOLORS_H
#define THEMECOLORS_H

#endif // THEMECOLORS_H
#include <array>
#include <QPalette>

const std::array<QPair<QPalette::ColorRole, QColor>, 15> darkThemeColor={
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
    QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText,Qt::white),
    QPair<QPalette::ColorRole, QColor>(QPalette::BrightText, Qt::red),
    QPair<QPalette::ColorRole, QColor>(QPalette::Link, QColor(42, 130, 218)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(42, 130, 218)),
    QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText, Qt::white),
};

const std::array<QPair<QPalette::ColorRole, QColor>, 5> darkThemeDisabledColor={
    QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Text, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText,QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(80, 80, 80)),
    QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText,QColor(127, 127, 127)),
};

const std::array<QPair<QPalette::ColorRole, QColor>, 15> astraThemeColor={
    QPair<QPalette::ColorRole, QColor>(QPalette::Window, QColor(225, 226, 225)),// цвет фона приложения
    QPair<QPalette::ColorRole, QColor>(QPalette::Base, QColor(245, 245, 245)),//цвет фона листвиджетов и линеедитов
    QPair<QPalette::ColorRole, QColor>(QPalette::AlternateBase, QColor(127, 0, 0)),//цвет скролла
    QPair<QPalette::ColorRole, QColor>(QPalette::Dark, QColor("#b33c2a")),//цвет скролла при наведении
    QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, Qt::black),//цвет текста
    QPair<QPalette::ColorRole, QColor>(QPalette::Text, Qt::green), //цвет текста в лайн дитах
    QPair<QPalette::ColorRole, QColor>(QPalette::Shadow, QColor(127, 0, 0)),//заголовок
    QPair<QPalette::ColorRole, QColor>(QPalette::Button, QColor(127, 0, 0)),
    QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText,QColor(225, 226, 225)),
    QPair<QPalette::ColorRole, QColor>(QPalette::BrightText, QColor(17, 100, 19)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(74, 0, 0)),//обводочка
    QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText, Qt::black),
    QPair<QPalette::ColorRole, QColor>(QPalette::ToolTipText, QColor(53, 127, 53)),//не используется
    QPair<QPalette::ColorRole, QColor>(QPalette::LinkVisited, Qt::green),//не используется
    QPair<QPalette::ColorRole, QColor>(QPalette::Link, Qt::white),//не используется
};

const std::array<QPair<QPalette::ColorRole, QColor>, 5> astraThemeDisabledColor={
    QPair<QPalette::ColorRole, QColor>(QPalette::WindowText, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Text, QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::ButtonText,QColor(127, 127, 127)),
    QPair<QPalette::ColorRole, QColor>(QPalette::Highlight, QColor(80, 80, 80)),
    QPair<QPalette::ColorRole, QColor>(QPalette::HighlightedText,QColor(127, 127, 127)),
};
