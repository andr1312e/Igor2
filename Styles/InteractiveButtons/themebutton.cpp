#include "themebutton.h"

ThemeButton::ThemeButton(ThemesNames themeName, QWidget *parent)
    : QPushButton(parent)
{
    FillThemesAndFindCurrentTheme(themeName);
    InitButton();
    FillButton();
    ConnectObjects();
}

ThemeButton::~ThemeButton()
{

}

void ThemeButton::FillThemesAndFindCurrentTheme(const ThemesNames &themeName)
{
    const ThemeItem black(ThemesNames::BlackTheme, QStringLiteral("Изменить тему на красную"), QStringLiteral(":/images/themes/themes_red"));
    const ThemeItem astraRed(ThemesNames::AstraRedTheme, QStringLiteral("Изменить тему на черную"), QStringLiteral(":/images/themes/themes_blacked"));
    m_themeList.append(black);
    m_themeList.append(astraRed);
    QVector<ThemeItem>::ConstIterator themeListIterator = m_themeList.cbegin();
    while (themeListIterator != m_themeList.cend())
    {
        if (themeName == themeListIterator->themeName)
        {
            m_themeItemIterator = themeListIterator;
            return;
        }
    }
    m_themeItemIterator = m_themeList.cbegin();
}

void ThemeButton::InitButton()
{
    setFlat(true);
    setFocusPolicy(Qt::NoFocus);
}

void ThemeButton::FillButton()
{
    setIcon(m_themeItemIterator->themeIcon);
    setIconSize(QSize(30, 30));
    setToolTip(m_themeItemIterator->nextThemeToolTip);
}

void ThemeButton::ConnectObjects()
{
    connect(this, &QPushButton::clicked, this, &ThemeButton::OnButtonPressed);
}

void ThemeButton::OnButtonPressed()
{
    ++m_themeItemIterator;
    if (m_themeList.cend() == m_themeItemIterator)
    {
        m_themeItemIterator = m_themeList.cbegin();
    }
    FillButton();
    Q_EMIT ToChangeTheme(m_themeItemIterator->themeName);
}

ThemesNames ThemeButton::GetCurrentThemeName()
{
    return m_themeItemIterator->themeName;
}
