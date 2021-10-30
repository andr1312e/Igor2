#include "theme.h"

Theme::Theme(const QVector<QPair<QPalette::ColorRole, QColor> > &colors, const QVector<QPair<QPalette::ColorRole, QColor> > &disabledColor)
   : m_palette(new QPalette())
{
   for (QVector<QPair<QPalette::ColorRole, QColor>>::const_iterator it = colors.cbegin(); it != colors.cend(); ++it) {
      m_palette->setColor(it->first, it->second);
   }

   for (QVector<QPair<QPalette::ColorRole, QColor>>::const_iterator it = disabledColor.cbegin(); it != disabledColor.cend(); ++it) {
      m_palette->setColor(QPalette::Disabled, it->first, it->second);
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
