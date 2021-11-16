#include "stylechanger.h"

#include <QWidget>
#include <QDebug>

StyleChanger::StyleChanger(QApplication *app)
   : currentThemeIsWhite(true)
   , m_myApp(app)
{
   m_darkTheme = new Theme(darkThemeColor, darkThemeDisabledColor);
   m_whiteTheme = new Theme(astraThemeColor, astraThemeDisabledColor);
   m_darkTheme->ApplyTheme(app);
   QFile styleSheetFile(":/Styles/Themes/style.qss");

   if (styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      m_styleSheet = QString(styleSheetFile.readAll());
      styleSheetFile.close();
   } else {
      qFatal("Каскадная таблица стилей не найден");
   }
}

StyleChanger::~StyleChanger()
{
   delete m_darkTheme;
   delete m_whiteTheme;
}

void StyleChanger::OnChangeTheme(bool setDarkTheme)
{
   if (setDarkTheme) {
      m_darkTheme->ApplyTheme(m_myApp);
   } else {
      m_whiteTheme->ApplyTheme(m_myApp);
   }

   m_myApp->setStyleSheet(m_styleSheet);
}
