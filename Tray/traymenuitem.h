#ifndef FACILEMENUITEM_H
#define FACILEMENUITEM_H

#include "Styles/InteractiveButtons/interactivebuttonbase.h"
#include "Styles/Themes/stylechanger.h"

class TrayMenu;
class TrayMenuItem;

class TrayMenuItem : public InteractiveButton
{
    Q_OBJECT
public:
    explicit TrayMenuItem(QWidget *parent);
    explicit TrayMenuItem(const QString &m_buttonText, QWidget *parent);
    explicit TrayMenuItem(const QIcon &pixmapPath, QWidget *parent);
    explicit TrayMenuItem(const QIcon &pixmapPath, const QString &m_buttonText, QWidget *parent);
    ~TrayMenuItem();

public:
    void ChangeButtonIconColor(ThemesNames themeName);
    void ChangeButtonIconEnabled();
public:
    bool IsChecked() const noexcept;
    void SetEndAnimationGeometry(const QRect &endAnimationGeometry) noexcept;
    const QRect &GetEndAnimationRect() const noexcept;

private:
    QRect m_endAnimationGeometry;
    bool checkable = false;
};

#endif // FACILEMENUITEM_H
