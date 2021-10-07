#include "proxystyle.h"

#include <QWidget>
#include <QDebug>

ProxyStyle::ProxyStyle()
    : QProxyStyle(styleBase())
    , m_qfCurrentStyle(new QFile())
{

}

void ProxyStyle::setTheme(QString &theme)
{
    if (theme=="Dark")
    {
        m_qfCurrentStyle->setFileName(QStringLiteral(":/Styles/Themes/styleDark.qss"));
        if (m_qfCurrentStyle->open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_styleSheet = QString(m_qfCurrentStyle->readAll());

        }
    }
    else
    {
        m_qfCurrentStyle->setFileName(QStringLiteral(":/Styles/Themes/styleWhite.qss"));
        if (m_qfCurrentStyle->open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_styleSheet = QString(m_qfCurrentStyle->readAll());
        }
    }
   m_qfCurrentStyle->close();
}

ProxyStyle::~ProxyStyle()
{
    delete m_qfCurrentStyle;
}

QStyle *ProxyStyle::styleBase(QStyle *style) const {
    if (style==Q_NULLPTR)
    {
        return QStyleFactory::create(QStringLiteral("Fusion"));
    }
    else
    {
        return style;
    }
}

void ProxyStyle::polish(QPalette &palette) {
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::WindowText,
                     QColor(127, 127, 127));
    palette.setColor(QPalette::Base, QColor(42, 42, 42));
    palette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, QColor(53, 53, 53));
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    palette.setColor(QPalette::Dark, QColor(35, 35, 35));
    palette.setColor(QPalette::Shadow, QColor(20, 20, 20));
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText,
                     QColor(127, 127, 127));
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText,
                     QColor(127, 127, 127));
}

void ProxyStyle::polish(QApplication *app)
{
    m_myApp=app;
    qDebug()<< "polish 1";
    if (!styleApplyed)
    {
        qDebug()<< "polish 2";
        app->setStyleSheet(m_styleSheet);
        styleApplyed=true;
    }
}

void ProxyStyle::changeTheme(bool state)
{
//   if (state)
//   {
//        unpolish(m_myApp);
//   }
//   else
//   {
//        polish(m_myApp);
//   }
}
