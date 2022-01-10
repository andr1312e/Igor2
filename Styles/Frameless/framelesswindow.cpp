
#include "framelesswindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QScreen>


FramelessWindow::FramelessWindow(QWidget *parent)
    : QWidget(parent)
    , m_isMousePressed(false)
    , m_isDragTop(false)
    , m_isDragLeft(false)
    , m_isDragRight(false)
    , m_isDragBottom(false)
{

    CreateUI();
    InitUI();
    SetShadowUnderTitleText();
    SetWindowShadow();
    InsertWidgetsIntoLayout();
    ApplyStyles();
    SetObjectNames();
    CreateConnections();

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);

    setMouseTracking(true);

    QApplication::instance()->installEventFilter(this);
}

FramelessWindow::~FramelessWindow()
{
    delete m_verticalLayout;
    delete m_topLayout;
    delete m_mainLayout;

    delete m_textShadow;

    delete m_WindowTitleBar;
    delete m_windowFrame;

    delete m_closeButton;
    delete m_maximizeButton;
    delete m_minimizeButton;
    delete m_zoomButton;
    delete m_icon;
    delete m_titleText;
}

void FramelessWindow::OnRestoreButtonClicked()
{
    m_zoomButton->setVisible(false);
    m_maximizeButton->setVisible(true);
    setWindowState(Qt::WindowNoState);
}

void FramelessWindow::OnMaximizeButtonClicked()
{
    m_zoomButton->setVisible(true);
    m_maximizeButton->setVisible(false);
    setWindowState(Qt::WindowMaximized);
    StyleWindow(true, false);
}

void FramelessWindow::changeEvent(QEvent *event)
{
    if (QEvent::WindowStateChange==event->type())
    {
        if (windowState().testFlag(Qt::WindowNoState))
        {
            m_zoomButton->setVisible(false);
            m_maximizeButton->setVisible(true);
            StyleWindow(true, true);
            event->ignore();
        }
        else
        {
            if (windowState().testFlag(Qt::WindowMaximized))
            {
                m_zoomButton->setVisible(true);
                m_maximizeButton->setVisible(false);
                StyleWindow(true, false);
                event->ignore();
            }
        }
    }
    else
    {
        event->accept();
    }
}

void FramelessWindow::SetMainWidget(QWidget *widget)
{
    m_verticalLayout->addWidget(widget);
}

void FramelessWindow::OnSetWindowTitle(const QString &text)
{
    m_titleText->setText(text);
    QFont titleFont = m_titleText->font();
    titleFont.setPointSize(17);
    m_titleText->setFont(titleFont);
}

void FramelessWindow::OnSetWindowIcon(const QIcon &icon)
{
    m_icon->setPixmap(icon.pixmap(32, 32));
}

void FramelessWindow::StyleWindow(bool bActive, bool isMaximized)
{
    if (bActive) {
        if (isMaximized) {
            //         layout()->setMargin(10);
            m_WindowTitleBar->setStyleSheet(QStringLiteral(
                                                "#windowTitlebar{border: 0px none palette(shadow); "
                                                "border-top-left-radius:5px; border-top-right-radius:5px; "
                                                "background-color:palette(shadow); height:20px;}"));
            m_windowFrame->setStyleSheet(QStringLiteral(
                                             "#windowFrame{border:1px solid palette(highlight); border-radius:5px "
                                             "5px 5px 5px; background-color:palette(Window);}"));
            QGraphicsEffect *oldShadow = m_windowFrame->graphicsEffect();

            if (oldShadow) {
                delete oldShadow;
            }

            QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
            windowShadow->setBlurRadius(9.0);
            windowShadow->setColor(palette().color(QPalette::Highlight));
            windowShadow->setOffset(0.0);
            m_windowFrame->setGraphicsEffect(windowShadow);
        } else {
            layout()->setMargin(0);
            m_WindowTitleBar->setStyleSheet(QStringLiteral(
                                                "#windowTitlebar{border: 0px none palette(shadow); "
                                                "border-top-left-radius:0px; border-top-right-radius:0px; "
                                                "background-color:palette(shadow); height:15px;}"));
            m_windowFrame->setStyleSheet(QStringLiteral(
                                             "#windowFrame{border:1px solid palette(dark); border-radius:0px 0px "
                                             "0px 0px; background-color:palette(Window);}"));
            QGraphicsEffect *oldShadow = m_windowFrame->graphicsEffect();

            if (oldShadow) {
                delete oldShadow;
            }

            m_windowFrame->setGraphicsEffect(nullptr);
        }
    } else {
        if (isMaximized) {
            layout()->setMargin(15);
            m_WindowTitleBar->setStyleSheet(QStringLiteral(
                                                "#windowTitlebar{border: 0px none palette(shadow); "
                                                "border-top-left-radius:5px; border-top-right-radius:5px; "
                                                "background-color:palette(dark); height:15px;}"));
            m_windowFrame->setStyleSheet(QStringLiteral(
                                             "#windowFrame{border:1px solid #000000; border-radius:5px 5px 5px "
                                             "5px; background-color:palette(Window);}"));
            QGraphicsEffect *oldShadow = m_windowFrame->graphicsEffect();

            if (oldShadow) {
                delete oldShadow;
            }

            QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
            windowShadow->setBlurRadius(9.0);
            windowShadow->setColor(palette().color(QPalette::Shadow));
            windowShadow->setOffset(0.0);
            m_windowFrame->setGraphicsEffect(windowShadow);
        } else {
            layout()->setMargin(0);
            m_WindowTitleBar->setStyleSheet(QStringLiteral(
                                                "#titlebarWidget{border: 0px none palette(shadow); "
                                                "border-top-left-radius:0px; border-top-right-radius:0px; "
                                                "background-color:palette(dark); height:15px;}"));
            m_windowFrame->setStyleSheet(QStringLiteral(
                                             "#windowFrame{border:1px solid palette(shadow); border-radius:0px "
                                             "0px 0px 0px; background-color:palette(Window);}"));
            QGraphicsEffect *oldShadow = m_windowFrame->graphicsEffect();

            if (oldShadow) {
                delete oldShadow;
            }

            m_windowFrame->setGraphicsEffect(nullptr);
        }
    }
}

void FramelessWindow::OnApplicationStateChanged(Qt::ApplicationState state)
{
    if (state == Qt::ApplicationActive) {
        StyleWindow(true, true);
    } else {
        StyleWindow(false, true);
    }
}

void FramelessWindow::OnChangeThemeButtonClicked(bool state)
{    
    if (state)
    {
        m_changeThemePushButton->setIcon(QIcon(":/images/moon"));
        if(m_changeDelegatesView->isChecked())
        {
            m_changeDelegatesView->setIcon(QIcon(":/images/horizontalDelegateWhite"));
        }
        else
        {
            m_changeDelegatesView->setIcon(QIcon(":/images/iconDelegateWhite"));
        }
    } else
    {
        m_changeThemePushButton->setIcon(QIcon(":/images/sun"));
        if(m_changeDelegatesView->isChecked())
        {
            m_changeDelegatesView->setIcon(QIcon(":/images/horizontalDelegateBlack"));
        }
        else
        {
           m_changeDelegatesView->setIcon(QIcon(":/images/iconDelegateBlack"));
        }
    }
    m_changeThemePushButton->setIconSize(QSize(20, 20));
    Q_EMIT ToChangeTheme(state);
}

void FramelessWindow::OnChangeAdditionalSettingsButtonClicked(bool state)
{
//    if (state)
//    {
//        m_changeThemePushButton->setIcon(QIcon(":/images/moon"));
//        if(m_changeDelegatesView->isChecked())
//        {
//            m_changeDelegatesView->setIcon(QIcon(":/images/iconDelegateWhite"));
//        }
//        else
//        {
//           m_changeDelegatesView->setIcon(QIcon(":/images/horizontalDelegateWhite"));
//        }
//    } else
//    {
//        m_changeThemePushButton->setIcon(QIcon(":/images/sun"));
//        if(m_changeDelegatesView->isChecked())
//        {
//            m_changeDelegatesView->setIcon(QIcon(":/images/iconDelegateBlack"));
//        }
//        else
//        {
//           m_changeDelegatesView->setIcon(QIcon(":/images/horizontalDelegateBlack"));
//        }
//    }

//    m_changeThemePushButton->setIconSize(QSize(20, 20));
    Q_EMIT ToHideAdditionalSettings(state);
}

void FramelessWindow::OnChangeDelegatesViewButtonClicked(bool state)
{
    if(state)
    {
        if(m_changeThemePushButton->isChecked())
        {
            m_changeDelegatesView->setIcon(QIcon(":/images/horizontalDelegateWhite"));
        }
        else
        {
            m_changeDelegatesView->setIcon(QIcon(":/images/horizontalDelegateBlack"));
        }
    }
    else
    {
        if(m_changeThemePushButton->isChecked())
        {
            m_changeDelegatesView->setIcon(QIcon(":/images/iconDelegateWhite"));
        }
        else
        {
            m_changeDelegatesView->setIcon(QIcon(":/images/iconDelegateBlack"));
        }
    }
    m_changeDelegatesView->setIconSize(QSize(20, 20));
    Q_EMIT ToSetDelegateView(state);
}

void FramelessWindow::OnWindowDraggerDoubleClicked()
{
    if (windowState().testFlag(Qt::WindowNoState)) {
        OnMaximizeButtonClicked();
    } else if (windowState().testFlag(Qt::WindowMaximized)) {
        OnRestoreButtonClicked();
    }
}

void FramelessWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void FramelessWindow::checkBorderDragging(QMouseEvent *event)
{
    if (isMaximized()) {
        return;
    }

    QPoint globalMousePos = event->globalPos();

    if (m_isMousePressed) {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect availGeometry = screen->availableGeometry();
        int h = availGeometry.height();
        int w = availGeometry.width();
        QList<QScreen *> screenlist = screen->virtualSiblings();

        if (screenlist.contains(screen)) {
            QSize sz = QApplication::desktop()->size();
            h = sz.height();
            w = sz.width();
        }

        // верх право
        if (m_isDragTop && m_isDragRight) {
            int diff =
                    globalMousePos.x() - (m_startGeometry.x() + m_startGeometry.width());
            int neww = m_startGeometry.width() + diff;
            diff = globalMousePos.y() - m_startGeometry.y();
            int newy = m_startGeometry.y() + diff;

            if (neww > 0 && newy > 0 && newy < h - 50) {
                QRect newg = m_startGeometry;
                newg.setWidth(neww);
                newg.setX(m_startGeometry.x());
                newg.setY(newy);
                setGeometry(newg);
            }
        }
        // верх лево
        else if (m_isDragTop && m_isDragLeft) {
            int diff = globalMousePos.y() - m_startGeometry.y();
            int newy = m_startGeometry.y() + diff;
            diff = globalMousePos.x() - m_startGeometry.x();
            int newx = m_startGeometry.x() + diff;

            if (newy > 0 && newx > 0) {
                QRect newg = m_startGeometry;
                newg.setY(newy);
                newg.setX(newx);
                setGeometry(newg);
            }
        }
        // низ справа
        else if (m_isDragBottom && m_isDragLeft) {
            int diff =
                    globalMousePos.y() - (m_startGeometry.y() + m_startGeometry.height());
            int newh = m_startGeometry.height() + diff;
            diff = globalMousePos.x() - m_startGeometry.x();
            int newx = m_startGeometry.x() + diff;

            if (newh > 0 && newx > 0) {
                QRect newg = m_startGeometry;
                newg.setX(newx);
                newg.setHeight(newh);
                setGeometry(newg);
            }
        } else if (m_isDragTop) {
            int diff = globalMousePos.y() - m_startGeometry.y();
            int newy = m_startGeometry.y() + diff;

            if (newy > 0 && newy < h - 50) {
                QRect newg = m_startGeometry;
                newg.setY(newy);
                setGeometry(newg);
            }
        } else if (m_isDragLeft) {
            int diff = globalMousePos.x() - m_startGeometry.x();
            int newx = m_startGeometry.x() + diff;

            if (newx > 0 && newx < w - 50) {
                QRect newg = m_startGeometry;
                newg.setX(newx);
                setGeometry(newg);
            }
        } else if (m_isDragRight) {
            int diff =
                    globalMousePos.x() - (m_startGeometry.x() + m_startGeometry.width());
            int neww = m_startGeometry.width() + diff;

            if (neww > 0) {
                QRect newg = m_startGeometry;
                newg.setWidth(neww);
                newg.setX(m_startGeometry.x());
                setGeometry(newg);
            }
        } else if (m_isDragBottom) {
            int diff =
                    globalMousePos.y() - (m_startGeometry.y() + m_startGeometry.height());
            int newh = m_startGeometry.height() + diff;

            if (newh > 0) {
                QRect newg = m_startGeometry;
                newg.setHeight(newh);
                newg.setY(m_startGeometry.y());
                setGeometry(newg);
            }
        }
    } else {
        // Мышь не зажата
        if (LeftBorderHit(globalMousePos) && TopBorderHit(globalMousePos)) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (RightBorderHit(globalMousePos) && TopBorderHit(globalMousePos)) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (LeftBorderHit(globalMousePos) &&
                   BottomBorderHit(globalMousePos)) {
            setCursor(Qt::SizeBDiagCursor);
        } else {
            if (TopBorderHit(globalMousePos)) {
                setCursor(Qt::SizeVerCursor);
            } else if (LeftBorderHit(globalMousePos)) {
                setCursor(Qt::SizeHorCursor);
            } else if (RightBorderHit(globalMousePos)) {
                setCursor(Qt::SizeHorCursor);
            } else if (BottomBorderHit(globalMousePos)) {
                setCursor(Qt::SizeVerCursor);
            } else {
                m_isDragTop = false;
                m_isDragLeft = false;
                m_isDragRight = false;
                m_isDragBottom = false;
                setCursor(Qt::ArrowCursor);
            }
        }
    }
}

// pos in global virtual desktop coordinates
bool FramelessWindow::LeftBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();

    if (pos.x() >= rect.x() && pos.x() <= rect.x() + CONST_DRAG_BORDER_SIZE) {
        return true;
    }

    return false;
}

bool FramelessWindow::RightBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    int tmp = rect.x() + rect.width();

    if (pos.x() <= tmp && pos.x() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }

    return false;
}

bool FramelessWindow::TopBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();

    if (pos.y() >= rect.y() && pos.y() <= rect.y() + CONST_DRAG_BORDER_SIZE) {
        return true;
    }

    return false;
}

bool FramelessWindow::BottomBorderHit(const QPoint &pos)
{
    const QRect &rect = this->geometry();
    int tmp = rect.y() + rect.height();

    if (pos.y() <= tmp && pos.y() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }

    return false;
}

void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    if (isMaximized()) {
        return;
    }

    m_isMousePressed = true;
    m_startGeometry = this->geometry();

    QPoint globalMousePos = mapToGlobal(QPoint(event->x(), event->y()));

    if (LeftBorderHit(globalMousePos) && TopBorderHit(globalMousePos)) {
        m_isDragTop = true;
        m_isDragLeft = true;
        setCursor(Qt::SizeFDiagCursor);
    } else if (RightBorderHit(globalMousePos) && TopBorderHit(globalMousePos)) {
        m_isDragRight = true;
        m_isDragTop = true;
        setCursor(Qt::SizeBDiagCursor);
    } else if (LeftBorderHit(globalMousePos) && BottomBorderHit(globalMousePos)) {
        m_isDragLeft = true;
        m_isDragBottom = true;
        setCursor(Qt::SizeBDiagCursor);
    } else {
        if (TopBorderHit(globalMousePos)) {
            m_isDragTop = true;
            setCursor(Qt::SizeVerCursor);
        } else if (LeftBorderHit(globalMousePos)) {
            m_isDragLeft = true;
            setCursor(Qt::SizeHorCursor);
        } else if (RightBorderHit(globalMousePos)) {
            m_isDragRight = true;
            setCursor(Qt::SizeHorCursor);
        } else if (BottomBorderHit(globalMousePos)) {
            m_isDragBottom = true;
            setCursor(Qt::SizeVerCursor);
        }
    }
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (isMaximized()) {
        return;
    }

    m_isMousePressed = false;
    bool bSwitchBackCursorNeeded =
            m_isDragTop || m_isDragLeft || m_isDragRight || m_isDragBottom;
    m_isDragTop = false;
    m_isDragLeft = false;
    m_isDragRight = false;
    m_isDragBottom = false;

    if (bSwitchBackCursorNeeded) {
        setCursor(Qt::ArrowCursor);
    }
}

bool FramelessWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (isMaximized()) {
        return QWidget::eventFilter(obj, event);
    }

    if (event->type() == QEvent::MouseMove) {
        QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);

        if (pMouse) {
            checkBorderDragging(pMouse);
        }
    }

    else if (event->type() == QEvent::MouseButtonPress && obj == this) {
        QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);

        if (pMouse) {
            mousePressEvent(pMouse);
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        if (m_isMousePressed) {
            QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);

            if (pMouse) {
                mouseReleaseEvent(pMouse);
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void FramelessWindow::CreateUI()
{
    m_mainLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();

    m_windowFrame = new QWidget();

    m_verticalLayout = new QVBoxLayout();
    m_WindowTitleBar = new WindowTitleBar(this);
    m_changeDelegatesView=new QPushButton(this);
    m_changeThemePushButton=new QPushButton(this);
    m_changeAdditionalSettingsView= new QPushButton(this);
    m_closeButton = new CloseWindowButton(this);
    m_maximizeButton = new MaximizeWindowButton(this);
    m_minimizeButton = new MinimizeWindowButton(this);
    m_zoomButton = new RestoreWindowButton(this);
    m_icon = new QLabel();
    m_titleText = new QLabel();
    m_zoomButton->setVisible(false);
}

void FramelessWindow::InitUI()
{
    m_minimizeButton->setToolTip("Свернуть");
    m_maximizeButton->setToolTip("Развернуть окно программы на весь экран");
    m_zoomButton->setToolTip("Выйти из полноэкранного режима");
    m_closeButton->setToolTip("Закрыть");


    m_changeDelegatesView->setCheckable(true);
    m_changeDelegatesView->setChecked(true);
    m_changeDelegatesView->setFlat(true);
    m_changeDelegatesView->setDefault(false);
    m_changeDelegatesView->setAutoDefault(false);
    m_changeDelegatesView->setFocusPolicy(Qt::NoFocus);
    m_changeDelegatesView->setIcon(QIcon(":/images/horizontalDelegateWhite"));
    m_changeDelegatesView->setStyleSheet("border: 0px;");
    m_changeDelegatesView->setIconSize(QSize(20, 20));
    m_changeDelegatesView->setToolTip("Изменить вид пользователей и программ");

    m_changeThemePushButton->setCheckable(true);
    m_changeThemePushButton->setChecked(true);
    m_changeThemePushButton->setFlat(true);
    m_changeThemePushButton->setDefault(false);
    m_changeThemePushButton->setAutoDefault(false);
    m_changeThemePushButton->setFocusPolicy(Qt::NoFocus);
    m_changeThemePushButton->setIcon(QIcon(":/images/moon"));
    m_changeThemePushButton->setStyleSheet("border: 0px;");
    m_changeThemePushButton->setIconSize(QSize(20, 20));
    m_changeThemePushButton->setToolTip("Изменить тему");



    m_changeAdditionalSettingsView->setCheckable(true);
    m_changeAdditionalSettingsView->setChecked(false);
    m_changeAdditionalSettingsView->setFlat(true);
    m_changeAdditionalSettingsView->setDefault(false);
    m_changeAdditionalSettingsView->setAutoDefault(false);
    m_changeAdditionalSettingsView->setFocusPolicy(Qt::NoFocus);
    m_changeAdditionalSettingsView->setIcon(QIcon(":/images/settingsWhite"));
    m_changeAdditionalSettingsView->setStyleSheet("border: 0px;");
    m_changeAdditionalSettingsView->setIconSize(QSize(20, 20));
    m_changeAdditionalSettingsView->setToolTip("Убрать/показать панель отображения ролей");

}

void FramelessWindow::SetShadowUnderTitleText()
{
    m_textShadow = new QGraphicsDropShadowEffect();
    m_textShadow->setBlurRadius(2.0);
    m_textShadow->setColor(Qt::white);
    m_textShadow->setOffset(0.1);
    m_titleText->setGraphicsEffect(m_textShadow);
}

void FramelessWindow::SetWindowShadow()
{
    QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
    windowShadow->setBlurRadius(9.0);
    windowShadow->setColor(Qt::darkBlue);
    windowShadow->setOffset(0.0);
    m_windowFrame->setGraphicsEffect(windowShadow);
}

void FramelessWindow::InsertWidgetsIntoLayout()
{
    m_topLayout->addWidget(m_icon, 1);
    m_topLayout->addWidget(m_titleText, 2);
    m_topLayout->addWidget(m_changeDelegatesView);
    m_topLayout->addWidget(m_changeThemePushButton);
    m_topLayout->addWidget(m_changeAdditionalSettingsView);
    m_topLayout->addWidget(m_minimizeButton);
    m_topLayout->addWidget(m_zoomButton);
    m_topLayout->addWidget(m_maximizeButton);
    m_topLayout->addWidget(m_closeButton);
    m_WindowTitleBar->setLayout(m_topLayout);

    m_verticalLayout->addWidget(m_WindowTitleBar);
    m_windowFrame->setLayout(m_verticalLayout);

    m_mainLayout->addWidget(m_windowFrame);

    setLayout(m_mainLayout);
}

void FramelessWindow::ApplyStyles()
{
    m_windowFrame->setAutoFillBackground(false);

    m_verticalLayout->setSpacing(0);
    m_verticalLayout->setContentsMargins(11, 11, 11, 11);
    m_verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    m_verticalLayout->setContentsMargins(1, 1, 1, 1);

    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(11, 11, 11, 11);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
}

void FramelessWindow::SetObjectNames()
{
    m_windowFrame->setObjectName("windowFrame");
    m_WindowTitleBar->setObjectName("windowTitlebar");
    m_maximizeButton->setObjectName("maximizeButton");
    m_minimizeButton->setObjectName("minimizeButton");
    m_closeButton->setObjectName("closeButton");
    m_zoomButton->setObjectName("restoreButton");
    m_icon->setObjectName("icon");
    m_titleText->setObjectName("titleText");
}

void FramelessWindow::CreateConnections()
{
    connect(m_minimizeButton, &QToolButton::clicked, [&]() {
        setWindowState(Qt::WindowMinimized);
    });
    connect(m_maximizeButton, &QToolButton::clicked, this, &FramelessWindow::OnMaximizeButtonClicked);
    connect(m_zoomButton, &QToolButton::clicked, this, &FramelessWindow::OnRestoreButtonClicked);
    connect(m_closeButton, &QToolButton::clicked, [&]() {
        close();
    });
    connect(m_WindowTitleBar, &WindowTitleBar::ToDoubleClicked, this, &FramelessWindow::OnWindowDraggerDoubleClicked);
    connect(m_changeThemePushButton, &QPushButton::clicked, this, &FramelessWindow::OnChangeThemeButtonClicked);
    connect(m_changeAdditionalSettingsView, &QPushButton::clicked, this, &FramelessWindow::OnChangeAdditionalSettingsButtonClicked);
    connect(m_changeDelegatesView,  &QPushButton::clicked, this, &FramelessWindow::OnChangeDelegatesViewButtonClicked);
}
