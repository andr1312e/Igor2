#ifndef STYLES_DARKSTYLE_H
#define STYLES_DARKSTYLE_H

#include <QFile>
#include <QFont>
#include <QPalette>
#include <QProxyStyle>
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QPair>
#include <array>

#include "themecolors.h"

struct Theme
{
private:
    QString styleSheet;
    QPalette palette;
public:
    Theme(QString pathToStyleSheet,const std::array<QPair<QPalette::ColorRole, QColor>, 15> &colors,const std::array<QPair<QPalette::ColorRole, QColor>, 5> disabledColor)
    {
        for (std::array<QPair<QPalette::ColorRole, QColor>, 15>::const_iterator it=colors.cbegin(); it!=colors.cend(); ++it)
        {
            palette.setColor(it->first, it->second);
        }
//        for (std::array<QPair<QPalette::ColorRole, QColor>, 5>::const_iterator it=disabledColor.cbegin(); it!=disabledColor.cend(); ++it)
//        {
//            palette.setColor(QPalette::Disabled, it->first, it->second);
//        }
        QFile m_themeFile(pathToStyleSheet);
        if (m_themeFile.open(QIODevice::ReadOnly| QIODevice::Text))
        {
            styleSheet=QString(m_themeFile.readAll());
            m_themeFile.close();
        }
    }
    ~Theme()
    {

    }

    void ApplyTheme(QApplication *app)
    {
        app->setPalette(palette);
        app->setStyleSheet(styleSheet);
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
    QStyle *styleBase(QStyle *style = Q_NULLPTR) const;

    bool currentThemeIsWhite;

    Theme *m_darkTheme;
    Theme *m_astraTheme;


    QApplication *m_myApp;

};

#endif // STYLES_DARKSTYLE_H
