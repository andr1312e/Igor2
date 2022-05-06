#ifndef FACILEMENUITEM_H
#define FACILEMENUITEM_H

#include "Styles/InteractiveButtons/interactivebuttonbase.h"

class TrayMenu;
class TrayMenuItem;

class TrayMenuItem : public InteractiveButtonBase
{
    Q_OBJECT
public:
    explicit TrayMenuItem(QWidget* parent);
    explicit TrayMenuItem(const QString &text, QWidget* parent);
    explicit TrayMenuItem(const QIcon &pixmapPath, QWidget* parent);
    explicit TrayMenuItem(const QIcon &pixmapPath, const QString &text, QWidget* parent);
    ~TrayMenuItem();
public:
    void SetIconPath(const QString &imagePath);
    QString GetIconPath(){ return m_pixmapPath;}
    void SetTooltipText(const QString &tooltipText);
    bool IsChecked();

private:
    bool checkable = false;
    QString m_pixmapPath;
};

#endif // FACILEMENUITEM_H
