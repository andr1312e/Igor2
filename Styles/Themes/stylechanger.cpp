#include "stylechanger.h"

#include <QWidget>
#include <QDebug>

StyleChanger::StyleChanger(QApplication *app)
    : currentThemeIsWhite(true)
    , m_myApp(app)
{
    m_darkTheme=new Theme(darkThemeColor, darkThemeDisabledColor);
    m_astraTheme=new Theme(astraThemeColor, astraThemeDisabledColor);
    m_darkTheme->ApplyTheme(app);
    QFile styleSheetFile(":/Styles/Themes/style.qss");
    if (styleSheetFile.open(QIODevice::ReadOnly| QIODevice::Text))
    {
        m_styleSheet=QString(styleSheetFile.readAll());
        styleSheetFile.close();
    }
    else
    {
        qFatal("styleSheet не найден");
    }
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
    m_myApp->setStyleSheet(m_styleSheet);
}
