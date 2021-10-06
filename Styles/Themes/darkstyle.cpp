#include "darkstyle.h"

#include <QWidget>
#include <QDebug>

DarkStyle::DarkStyle()
    : QProxyStyle(styleBase())
    , m_qfDarkstyle(new QFile())
{

}

DarkStyle::~DarkStyle()
{
    delete m_qfDarkstyle;
}

QStyle *DarkStyle::styleBase(QStyle *style) const {
    if (style==Q_NULLPTR)
    {
        return QStyleFactory::create(QStringLiteral("Fusion"));
    }
    else
    {
        return style;
    }
}

void DarkStyle::polish(QPalette &palette) {
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

void DarkStyle::polish(QApplication *app)
{
    qDebug()<< "polish";
    if (!styleApplyed)
    {
        qDebug()<< "polish";
        m_qfDarkstyle->setFileName(":/Styles/style.qss");
        if (m_qfDarkstyle->open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString qsStylesheet = QString(m_qfDarkstyle->readAll());
            app->setStyleSheet(qsStylesheet);
            m_qfDarkstyle->close();
        }
        styleApplyed=true;
    }
}