#include "stylechanger.h"

StyleChanger::StyleChanger(bool isBlacked, QApplication *app)
    : m_myApp(app)
    , m_darkTheme(new Theme(darkThemeColor, darkThemeDisabledColor))
    , m_whiteTheme(new Theme(astraThemeColor, astraThemeDisabledColor))
{
    if(isBlacked)
    {
        m_darkTheme->ApplyTheme(app);
    }
    else
    {
        m_whiteTheme->ApplyTheme(app);
    }
    QFile styleSheetFile(":/Styles/Themes/style.qss");

    if (styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_styleSheet = styleSheetFile.readAll();
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

void StyleChanger::OnChangeTheme(bool isDarkTheme)
{
    if (isDarkTheme) {
        m_darkTheme->ApplyTheme(m_myApp);
    } else {
        m_whiteTheme->ApplyTheme(m_myApp);
    }
    const QString newTheme=isDarkTheme ? QStringLiteral(" Черную ") : QStringLiteral("Белую");
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Изменили тему на ") + newTheme);
    m_myApp->setStyleSheet(m_styleSheet);
    Q_EMIT ToUpdateViewColors();
}
