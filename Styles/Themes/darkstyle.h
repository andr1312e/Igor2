#ifndef STYLES_DARKSTYLE_H
#define STYLES_DARKSTYLE_H

#include <QFile>
#include <QFont>
#include <QPalette>
#include <QProxyStyle>
#include <QApplication>
#include <QStyleFactory>


class DarkStyle : public QProxyStyle
{
    Q_OBJECT

public:
    DarkStyle();
    ~DarkStyle();
    void polish(QPalette &palette) override;
    void polish(QApplication *app) override;

private:
    QStyle *styleBase(QStyle *style = Q_NULLPTR) const;

    QFile *m_qfDarkstyle;
    bool styleApplyed=false;
};

#endif // STYLES_DARKSTYLE_H
