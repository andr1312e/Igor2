#include "theme.h"

Theme::Theme(const std::array<QPair<QPalette::ColorRole, QColor>, 15> &colors, const std::array<QPair<QPalette::ColorRole, QColor>, 5> &disabledColor)
   : m_palette(new QPalette())
{
   for (const QPair<QPalette::ColorRole, QColor> &roleAndColor : colors) {
      m_palette->setColor(roleAndColor.first, roleAndColor.second);
   }

   for (const QPair<QPalette::ColorRole, QColor> &roleAndColor : disabledColor) {
      m_palette->setColor(QPalette::Disabled, roleAndColor.first, roleAndColor.second);
   }
}

Theme::~Theme()
{
   delete m_palette;
}

void Theme::ApplyTheme(QApplication *app)
{
   app->setPalette(*m_palette);
}
