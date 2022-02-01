#ifndef STYLES_THEMES_STYLECHANGER_H
#define STYLES_THEMES_STYLECHANGER_H

#include <QFile>
#include <QApplication>
#include <QPalette>
#include <QPair>

#include "Logging/logger.h"

#include "Styles/Themes/themecolors.h"
#include "Styles/Themes/theme.h"

class StyleChanger: public QObject
{
   Q_OBJECT
public:
   StyleChanger(bool isBlacked, QApplication *app);
   ~StyleChanger();

Q_SIGNALS:
   void ToUpdateViewColors();

public Q_SLOTS:
   void OnChangeTheme(bool isDarkTheme);

private:
   QApplication* const m_myApp;
   const Theme* const m_darkTheme;
   const Theme* const m_whiteTheme;

   QByteArray m_styleSheet;

};

#endif // STYLES_THEMES_STYLECHANGER_H
