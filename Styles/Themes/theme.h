#ifndef STYLES_THEMES_THEME_H
#define STYLES_THEMES_THEME_H

#include <QPalette>
#include <QApplication>

class Theme
{
public:
   Theme(const std::array<QPair<QPalette::ColorRole, QColor>, 15> &colors, const std::array<QPair<QPalette::ColorRole, QColor>, 5> &disabledColor);
   ~Theme();
   void ApplyTheme(QApplication *app);
private:
   QPalette *const m_palette;
};

#endif // STYLES_THEMES_THEME_H
