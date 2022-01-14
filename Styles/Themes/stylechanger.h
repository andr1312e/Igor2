#ifndef STYLES_THEMES_STYLECHANGER_H
#define STYLES_THEMES_STYLECHANGER_H

#include <QFile>
#include <QApplication>
#include <QPalette>
#include <QPair>

#include "Styles/Themes/themecolors.h"
#include "Styles/Themes/theme.h"

class StyleChanger: public QObject
{
   Q_OBJECT
public:
   StyleChanger(QApplication *app);
   ~StyleChanger();

Q_SIGNALS:
   void ToUpdateViewColors();

public Q_SLOTS:
   void OnChangeTheme(bool isDarkTheme);

private:
   bool currentThemeIsWhite;

   Theme *m_darkTheme;
   Theme *m_whiteTheme;

   QString m_styleSheet;
   QApplication* const m_myApp;
};

#endif // STYLES_THEMES_STYLECHANGER_H
