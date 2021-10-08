#ifndef STYLES_THEMES_STYLECHANGER_H
#define STYLES_THEMES_STYLECHANGER_H

#include <QFile>
#include <QFont>
#include <QPalette>
#include <QProxyStyle>
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QPair>
#include <array>
#include <QPixmapCache>

#include "themecolors.h"

struct Theme
{
private:
    QPalette palette;
public:
    Theme(const std::array<QPair<QPalette::ColorRole, QColor>, 15> &colors,const std::array<QPair<QPalette::ColorRole, QColor>, 5> disabledColor)
    {
        for (std::array<QPair<QPalette::ColorRole, QColor>, 15>::const_iterator it=colors.cbegin(); it!=colors.cend(); ++it)
        {
            palette.setColor(it->first, it->second);
        }
        for (std::array<QPair<QPalette::ColorRole, QColor>, 5>::const_iterator it=disabledColor.cbegin(); it!=disabledColor.cend(); ++it)
        {
            palette.setColor(QPalette::Disabled, it->first, it->second);
        }

    }
    ~Theme()
    {

    }

    void ApplyTheme(QApplication *app)
    {
        app->setPalette(palette);
    }
};

class StyleChanger: public QObject
{
    Q_OBJECT
public:
    StyleChanger(QApplication *app);
    ~StyleChanger();

public slots:

    void changeTheme(bool state);

private:
    bool currentThemeIsWhite;

    Theme *m_darkTheme;
    Theme *m_astraTheme;

    QString m_styleSheet;
    QApplication *m_myApp;
};

#endif // STYLES_THEMES_STYLECHANGER_H
