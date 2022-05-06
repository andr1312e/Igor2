#ifndef STYLES_THEMES_STYLECHANGER_H
#define STYLES_THEMES_STYLECHANGER_H

#include <QFile>
#include <QApplication>
#include <QPalette>
#include <QPair>

#include "Styles/Themes/themecolors.h"

class StyleChanger: public QObject
{
    Q_OBJECT
public:
    StyleChanger();
    ~StyleChanger();

Q_SIGNALS:
    void ToUpdateViewColors();

public Q_SLOTS:
    void OnChangeTheme(ThemesNames currentThemeName);
public:
    ThemesNames GetThemeName() const {return m_currentThemeName;}
private:
    void FillPalette(ThemesNames themeName);
private:
    QPalette m_palette;
    QString m_styleSheet;
    ThemesNames m_currentThemeName;
};

#endif // STYLES_THEMES_STYLECHANGER_H
