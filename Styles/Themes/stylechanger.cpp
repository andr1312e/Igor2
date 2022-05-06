#include "stylechanger.h"
#include <QDebug>

StyleChanger::StyleChanger()
    :m_currentThemeName(ThemesNames::BlackTheme)
{
    qApp->setStyle(QStringLiteral("Fusion"));
    QFile styleSheetFile(QStringLiteral(":/Styles/Themes/style.qss"));
    if (styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_styleSheet = QString(styleSheetFile.readAll());
        styleSheetFile.close();
    }
    else
    {
        qFatal("Каскадная таблица стилей не найден");
    }
}

StyleChanger::~StyleChanger()
{

}

void StyleChanger::OnChangeTheme(ThemesNames currentThemeName)
{
    FillPalette(currentThemeName);
    qApp->setPalette(m_palette);
    qApp->setStyleSheet(m_styleSheet);
    m_currentThemeName=currentThemeName;
    Q_EMIT ToUpdateViewColors();
}

void StyleChanger::FillPalette(ThemesNames themeName)
{
    const QPair<QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 15>, QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 5>> pair=themesList.value(themeName);
    const QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 15> colors=pair.first;
    for (const QPair<QPalette::ColorRole, QColor> &roleAndColor : colors) {
        m_palette.setColor(roleAndColor.first, roleAndColor.second);
    }
    const QVarLengthArray<QPair<QPalette::ColorRole, QColor>, 5> disabledColor=pair.second;
    for (const QPair<QPalette::ColorRole, QColor> &roleAndColor : disabledColor) {
        m_palette.setColor(QPalette::Disabled, roleAndColor.first, roleAndColor.second);
    }
}
