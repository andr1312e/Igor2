#ifndef STYLES_FRAMELESSWINDOW_H
#define STYLES_FRAMELESSWINDOW_H

#include <QLabel>
#include <QToolButton>
#include <QWidget>
#include <QFontDatabase>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QBoxLayout>
#include <QGraphicsDropShadowEffect>

#include "Styles/InteractiveButtons/minimizewindowbutton.h"
#include "Styles/InteractiveButtons/maximizewindowbutton.h"
#include "Styles/InteractiveButtons/restorewindowbutton.h"
#include "Styles/InteractiveButtons/closewindowbutton.h"
#include "Styles/InteractiveButtons/themebutton.h"
#include "Styles/Frameless/windowtitlebar.h"
#include "Styles/Themes/themecolors.h"

class FramelessWindow : public QWidget
{
    Q_OBJECT
public:

    explicit FramelessWindow(ThemesNames themeName, QWidget *parent);
    virtual ~FramelessWindow();

private:
    void SetFont();
    void CreateUI(ThemesNames themeName);
    void InitUI();
    void SetShadowUnderTitleText();
    void SetWindowShadow();
    void InsertWidgetsIntoLayout();
    void ApplyStyles();
    void SetObjectNames();
    void CreateConnections();

Q_SIGNALS:
    void ToChangeTheme(ThemesNames themeName);

public Q_SLOTS:
    void OnSetWindowTitle(const QString &text);

private Q_SLOTS:
    void OnApplicationStateChanged(Qt::ApplicationState state);
    void OnRestoreButtonClicked();
    void OnMaximizeButtonClicked();
    void OnWindowDraggerDoubleClicked();

private:
    bool LeftBorderHit(const QPoint &pos) const noexcept;
    bool RightBorderHit(const QPoint &pos) const noexcept;
    bool TopBorderHit(const QPoint &pos) const noexcept;
    bool BottomBorderHit(const QPoint &pos) const noexcept;
    void StyleWindow(bool isActive, bool isMaximized);
protected:
    virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void checkBorderDragging(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
public:
    void SetMainWidget(QWidget *widget);
private:
    WindowTitleBar *m_WindowTitleBar;
    QWidget *m_windowFrame;
    QVBoxLayout *m_mainLayout;
    QVBoxLayout *m_verticalLayout;
    QHBoxLayout *m_topLayout;
    QLabel *m_infoButton;
    ThemeButton *m_changeThemePushButton;

    CloseWindowButton *m_closeButton;
    MaximizeWindowButton *m_maximizeButton;
    MinimizeWindowButton *m_minimizeButton;
    RestoreWindowButton *m_zoomButton;
    QLabel *m_icon;
    QLabel *m_titleText;

    QGraphicsDropShadowEffect *m_textShadow;

    QRect m_startGeometry;
    const quint8 m_dragBorderSize = 15;
    bool m_isMousePressed;
    bool m_isDragTop;
    bool m_isDragLeft;
    bool m_isDragRight;
    bool m_isDragBottom;
};

#endif  // STYLES_FRAMELESSWINDOW_H
