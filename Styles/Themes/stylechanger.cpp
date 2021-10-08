#include "stylechanger.h"

#include <QWidget>
#include <QDebug>

StyleChanger::StyleChanger(QApplication *app)
    : m_myApp(app)
{
    m_darkTheme=new Theme(":/Styles/Themes/style.qss", darkThemeColor, darkThemeDisabledColor);
    m_astraTheme=new Theme(":/Styles/Themes/style.qss", astraThemeColor, astraThemeDisabledColor);
}

StyleChanger::~StyleChanger()
{
    delete m_darkTheme;
    delete m_astraTheme;
}

void StyleChanger::changeTheme(bool state)
{
    if (state)
    {
        m_astraTheme->ApplyTheme(m_myApp);
    }
    else
    {
        m_darkTheme->ApplyTheme(m_myApp);
    }
}
