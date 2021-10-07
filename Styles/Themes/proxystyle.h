#ifndef STYLES_DARKSTYLE_H
#define STYLES_DARKSTYLE_H

#include <QFile>
#include <QFont>
#include <QPalette>
#include <QProxyStyle>
#include <QApplication>
#include <QStyleFactory>


class ProxyStyle : public QProxyStyle
{
    Q_OBJECT

public:
    ProxyStyle();
    void setTheme(QString &theme);
    ~ProxyStyle();
    void polish(QPalette &palette) override;
    void polish(QApplication *app) override;

public slots:

    void changeTheme(bool state);

private:
    QStyle *styleBase(QStyle *style = Q_NULLPTR) const;

    QFile *m_qfCurrentStyle;
    QString m_styleSheet;
    QApplication *m_myApp;

    bool styleApplyed=false;
};

#endif // STYLES_DARKSTYLE_H
