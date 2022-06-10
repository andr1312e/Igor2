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
    void ToUpdateViewColors(ThemesNames themeName);

public Q_SLOTS:
    void OnChangeTheme(ThemesNames currentThemeName);
public:
    ThemesNames GetThemeName() const noexcept;
private:
    void FillPalette(ThemesNames themeName) noexcept;
private:
    QPalette m_palette;
    QString m_styleSheet;
    ThemesNames m_currentThemeName;
};

#endif // STYLES_THEMES_STYLECHANGER_H
