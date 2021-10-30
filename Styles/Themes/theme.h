#ifndef STYLES_THEMES_THEME_H
#define STYLES_THEMES_THEME_H

#include <QPalette>
#include <QApplication>

class Theme
{
public:
   Theme(const QVector<QPair<QPalette::ColorRole, QColor>> &colors, const QVector<QPair<QPalette::ColorRole, QColor>> &disabledColor);
   ~Theme();
   void ApplyTheme(QApplication *app);
private:
   QPalette *m_palette;
};

#endif // STYLES_THEMES_THEME_H
