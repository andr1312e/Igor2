#ifndef STYLES_INTERACTIVEBUTTONS_THEMEBUTTON_H
#define STYLES_INTERACTIVEBUTTONS_THEMEBUTTON_H

#include <QLinkedList>
#include <QPushButton>

#include "Structs/themeitem.h"
#include "Styles/Themes/themecolors.h"

class ThemeButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ThemeButton(ThemesNames themeName, QWidget *parent);
    ~ThemeButton();
private:
    void FillThemesAndFindCurrentTheme(const ThemesNames &themeName);
    void InitButton();
    void FillButton();
    void ConnectObjects();
Q_SIGNALS:
    void ToChangeTheme(ThemesNames themeName);
private Q_SLOTS:
    void OnButtonPressed();
public:
    ThemesNames GetCurrentThemeName();
private:
    QVector< ThemeItem> m_themeList;
    QVector< ThemeItem>::const_iterator m_themeItemIterator;
};

#endif // STYLES_INTERACTIVEBUTTONS_THEMEBUTTON_H
