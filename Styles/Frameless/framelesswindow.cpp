#include "framelesswindow.h"

FramelessWindow::FramelessWindow(ThemesNames themeName, QWidget *parent)
    : QWidget(parent)
    , m_isMousePressed(false)
    , m_isDragTop(false)
    , m_isDragLeft(false)
    , m_isDragRight(false)
    , m_isDragBottom(false)
{
    SetFont();
    CreateUI(themeName);
    InitUI();
    SetShadowUnderTitleText();
    SetWindowShadow();
    InsertWidgetsIntoLayout();
    ApplyStyles();
    SetObjectNames();
    CreateConnections();

    QWidget::setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    QWidget::setAttribute(Qt::WA_NoSystemBackground, true);
    QWidget::setAttribute(Qt::WA_TranslucentBackground);
    QWidget::setMouseTracking(true);
    QApplication::instance()->installEventFilter(this);
}

void FramelessWindow::CreateUI(ThemesNames themeName)
{
    m_mainLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();

    m_windowFrame = new QWidget();

    m_verticalLayout = new QVBoxLayout();
    m_WindowTitleBar = new WindowTitleBar(this);
    m_infoButton = new QLabel();
    m_changeThemePushButton = new ThemeButton(themeName, this);
    m_closeButton = new CloseWindowButton(this);
    m_maximizeButton = new MaximizeWindowButton(this);
    m_minimizeButton = new MinimizeWindowButton(this);
    m_zoomButton = new RestoreWindowButton(this);
    m_icon = new QLabel();
    m_titleText = new QLabel();
}

void FramelessWindow::InitUI()
{
    m_infoButton->setPixmap(QIcon(QLatin1Literal(":/images/topPanel/faq.png")).pixmap(QSize(30, 30)));
    m_infoButton->setToolTip(QString("Программа управления пользователями РЛС ТИ\nВерсия: %1 От: %2. Qt:%3 \nЭл почта: pilikin@kbknn.ru").arg(APP_VERSION).arg(BUILD_DATE).arg(qVersion()));
    m_minimizeButton->setToolTip(QStringLiteral("Свернуть"));
    m_maximizeButton->setToolTip(QStringLiteral("Развернуть окно программы на весь экран"));
    m_zoomButton->setToolTip(QStringLiteral("Выйти из полноэкранного режима"));
    m_closeButton->setToolTip(QStringLiteral("Закрыть"));
    m_zoomButton->setVisible(false);
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
    QGraphicsDropShadowEffect *const windowShadow = new QGraphicsDropShadowEffect;
    windowShadow->setBlurRadius(9.0);
    windowShadow->setColor(Qt::darkBlue);
    windowShadow->setOffset(0.0);
    m_windowFrame->setGraphicsEffect(windowShadow);
}

void FramelessWindow::InsertWidgetsIntoLayout()
{
    m_topLayout->addWidget(m_icon, 1);
    m_topLayout->addWidget(m_titleText, 2);
    m_topLayout->addWidget(m_infoButton);
    m_topLayout->addWidget(m_changeThemePushButton);
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

    m_topLayout->setContentsMargins(4, 4, 4, 4);
    m_verticalLayout->setSpacing(0);
    m_verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    m_verticalLayout->setContentsMargins(1, 1, 1, 5);

//    m_mainLayout->setContentsMargins(5, 5, 5, 5);
    m_mainLayout->setSpacing(0);
}

void FramelessWindow::SetObjectNames()
{
    m_windowFrame->setObjectName(QLatin1Literal("windowFrame"));
    m_WindowTitleBar->setObjectName(QLatin1Literal("windowTitlebar"));
    m_maximizeButton->setObjectName(QLatin1Literal("maximizeButton"));
    m_minimizeButton->setObjectName(QLatin1Literal("minimizeButton"));
    m_closeButton->setObjectName(QLatin1Literal("closeButton"));
    m_zoomButton->setObjectName(QLatin1Literal("restoreButton"));
    m_icon->setObjectName(QLatin1Literal("icon"));
    m_titleText->setObjectName(QLatin1Literal("titleText"));
}

void FramelessWindow::CreateConnections()
{
    connect(m_minimizeButton, &QToolButton::clicked, [&]()
    {
        setWindowState(Qt::WindowMinimized);
    });
    connect(m_maximizeButton, &QToolButton::clicked, this, &FramelessWindow::OnMaximizeButtonClicked);
    connect(m_zoomButton, &QToolButton::clicked, this, &FramelessWindow::OnRestoreButtonClicked);
    connect(m_closeButton, &QToolButton::clicked, this, &QWidget::close);
    connect(m_WindowTitleBar, &WindowTitleBar::ToDoubleClicked, this, &FramelessWindow::OnWindowDraggerDoubleClicked);
    connect(m_changeThemePushButton, &ThemeButton::ToChangeTheme, this, &FramelessWindow::ToChangeTheme);
}

FramelessWindow::~FramelessWindow()
{
    delete m_verticalLayout;
    delete m_topLayout;
    delete m_mainLayout;

    delete m_closeButton;
    delete m_maximizeButton;
    delete m_minimizeButton;
    delete m_zoomButton;
    delete m_icon;

    delete m_textShadow;
    delete m_titleText;

    delete m_WindowTitleBar;
    delete m_windowFrame;
}

void FramelessWindow::SetFont()
{
    const int id = QFontDatabase::addApplicationFont(QLatin1Literal(":/font/VisueltPro-Light.ttf"));
    const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    const QFont VisueltProFont(family);
    qApp->setFont(VisueltProFont);
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
    if (QEvent::WindowStateChange == event->type())
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
    titleFont.setPointSize(15);
    m_titleText->setFont(titleFont);
    setWindowTitle("УП РЛС ТИ");
}

void FramelessWindow::StyleWindow(bool isActive, bool isMaximized)
{
    if (isActive)
    {
        if (isMaximized)
        {
            m_WindowTitleBar->setStyleSheet(QLatin1Literal(
                                                "#windowTitlebar{border: 0px none palette(shadow); "
                                                "border-top-left-radius:5px; border-top-right-radius:5px; "
                                                "background-color:palette(shadow); height:20px;}"));
            m_windowFrame->setStyleSheet(QLatin1Literal(
                                             "#windowFrame{border:1px solid palette(highlight); border-radius:5px "
                                             "5px 5px 5px; background-color:palette(Window);}"));
            QGraphicsEffect *const oldShadow = m_windowFrame->graphicsEffect();

            if (oldShadow)//Удаляем старый
            {
                delete oldShadow;
            }

            QGraphicsDropShadowEffect *const windowShadow = new QGraphicsDropShadowEffect;
            windowShadow->setBlurRadius(9.0);
            windowShadow->setColor(palette().color(QPalette::Highlight));
            windowShadow->setOffset(0.0);
            m_windowFrame->setGraphicsEffect(windowShadow);
        }
        else
        {
            layout()->setMargin(0);
            m_WindowTitleBar->setStyleSheet(QLatin1Literal(
                                                "#windowTitlebar{border: 0px none palette(shadow); "
                                                "border-top-left-radius:0px; border-top-right-radius:0px; "
                                                "background-color:palette(shadow); height:10px;}"));
            m_windowFrame->setStyleSheet(QLatin1Literal(
                                             "#windowFrame{border:1px solid palette(dark); border-radius:0px 0px "
                                             "0px 0px; background-color:palette(Window);}"));
            QGraphicsEffect *const oldShadow = m_windowFrame->graphicsEffect();

            if (oldShadow)
            {
                delete oldShadow;
            }

            m_windowFrame->setGraphicsEffect(nullptr);
        }
    }
    else
    {
        if (isMaximized)
        {
            layout()->setMargin(15);
            m_WindowTitleBar->setStyleSheet(QLatin1Literal(
                                                "#windowTitlebar{border: 0px none palette(shadow); "
                                                "border-top-left-radius:5px; border-top-right-radius:5px; "
                                                "background-color:palette(dark); height:15px;}"));
            m_windowFrame->setStyleSheet(QLatin1Literal(
                                             "#windowFrame{border:1px solid #000000; border-radius:5px 5px 5px "
                                             "5px; background-color:palette(Window);}"));
            QGraphicsEffect *const oldShadow = m_windowFrame->graphicsEffect();

            if (oldShadow)
            {
                delete oldShadow;
            }

            QGraphicsDropShadowEffect *const windowShadow = new QGraphicsDropShadowEffect;
            windowShadow->setBlurRadius(9.0);
            windowShadow->setColor(palette().color(QPalette::Shadow));
            windowShadow->setOffset(0.0);
            m_windowFrame->setGraphicsEffect(windowShadow);
        }
        else
        {
            layout()->setMargin(0);
            m_WindowTitleBar->setStyleSheet(QLatin1Literal(
                                                "#titlebarWidget{border: 0px none palette(shadow); "
                                                "border-top-left-radius:0px; border-top-right-radius:0px; "
                                                "background-color:palette(dark); height:7px;}"));
            m_windowFrame->setStyleSheet(QLatin1Literal(
                                             "#windowFrame{border:1px solid palette(shadow); border-radius:0px "
                                             "0px 0px 0px; background-color:palette(Window);}"));
            QGraphicsEffect *const oldShadow = m_windowFrame->graphicsEffect();

            if (oldShadow)
            {
                delete oldShadow;
            }

            m_windowFrame->setGraphicsEffect(nullptr);
        }
    }
}

void FramelessWindow::OnApplicationStateChanged(Qt::ApplicationState state)
{
    if (Qt::ApplicationActive == state)
    {
        StyleWindow(true, true);
    }
    else
    {
        StyleWindow(false, true);
    }
}

void FramelessWindow::OnWindowDraggerDoubleClicked()
{
    if (windowState().testFlag(Qt::WindowNoState))
    {
        OnMaximizeButtonClicked();
    }
    else if (windowState().testFlag(Qt::WindowMaximized))
    {
        OnRestoreButtonClicked();
    }
}

void FramelessWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void FramelessWindow::checkBorderDragging(QMouseEvent *event)
{
    if (isMaximized())
    {
        return;
    }

    const QPoint globalMousePos = event->globalPos();

    if (m_isMousePressed)
    {
        QScreen *const screen = QGuiApplication::primaryScreen();
        const QRect availGeometry = screen->availableGeometry();
        int h = availGeometry.height();
        int w = availGeometry.width();
        const QList<QScreen *> screenlist = screen->virtualSiblings();

        if (screenlist.contains(screen))
        {
            const QSize screenSize = QApplication::desktop()->size();
            h = screenSize.height();
            w = screenSize.width();
        }

        // верх право
        if (m_isDragTop && m_isDragRight)
        {
            int diff =
                globalMousePos.x() - (m_startGeometry.x() + m_startGeometry.width());
            const int newWidth = m_startGeometry.width() + diff;
            diff = globalMousePos.y() - m_startGeometry.y();
            const int newHeight = m_startGeometry.y() + diff;

            if (newWidth > 0 && newHeight > 0 && newHeight < h - 50)
            {
                QRect newg = m_startGeometry;
                newg.setWidth(newWidth);
                newg.setX(m_startGeometry.x());
                newg.setY(newHeight);
                setGeometry(newg);
            }
        }
        // верх лево
        else if (m_isDragTop && m_isDragLeft)
        {
            int diff = globalMousePos.y() - m_startGeometry.y();
            const int newy = m_startGeometry.y() + diff;
            diff = globalMousePos.x() - m_startGeometry.x();
            const int newx = m_startGeometry.x() + diff;

            if (newy > 0 && newx > 0)
            {
                QRect newg = m_startGeometry;
                newg.setY(newy);
                newg.setX(newx);
                setGeometry(newg);
            }
        }
        // низ справа
        else if (m_isDragBottom && m_isDragLeft)
        {
            int diff =
                globalMousePos.y() - (m_startGeometry.y() + m_startGeometry.height());
            const int newh = m_startGeometry.height() + diff;
            diff = globalMousePos.x() - m_startGeometry.x();
            const int newx = m_startGeometry.x() + diff;

            if (newh > 0 && newx > 0)
            {
                QRect newg = m_startGeometry;
                newg.setX(newx);
                newg.setHeight(newh);
                setGeometry(newg);
            }
        }
        else if (m_isDragTop)
        {
            const int diff = globalMousePos.y() - m_startGeometry.y();
            const int newy = m_startGeometry.y() + diff;

            if (newy > 0 && newy < h - 50)
            {
                QRect newg = m_startGeometry;
                newg.setY(newy);
                setGeometry(newg);
            }
        }
        else if (m_isDragLeft)
        {
            const int diff = globalMousePos.x() - m_startGeometry.x();
            const int newx = m_startGeometry.x() + diff;

            if (newx > 0 && newx < w - 50)
            {
                QRect newg = m_startGeometry;
                newg.setX(newx);
                setGeometry(newg);
            }
        }
        else if (m_isDragRight)
        {
            const int diff =
                globalMousePos.x() - (m_startGeometry.x() + m_startGeometry.width());
            const int neww = m_startGeometry.width() + diff;

            if (neww > 0)
            {
                QRect newg = m_startGeometry;
                newg.setWidth(neww);
                newg.setX(m_startGeometry.x());
                setGeometry(newg);
            }
        }
        else if (m_isDragBottom)
        {
            const int diff =
                globalMousePos.y() - (m_startGeometry.y() + m_startGeometry.height());
            const int newh = m_startGeometry.height() + diff;

            if (newh > 0)
            {
                QRect newg = m_startGeometry;
                newg.setHeight(newh);
                newg.setY(m_startGeometry.y());
                setGeometry(newg);
            }
        }
    }
    else
    {
        // Мышь не зажата
        if (LeftBorderHit(globalMousePos) && TopBorderHit(globalMousePos))
        {
            setCursor(Qt::SizeFDiagCursor);
        }
        else if (RightBorderHit(globalMousePos) && TopBorderHit(globalMousePos))
        {
            setCursor(Qt::SizeBDiagCursor);
        }
        else if (LeftBorderHit(globalMousePos) &&
                 BottomBorderHit(globalMousePos))
        {
            setCursor(Qt::SizeBDiagCursor);
        }
        else
        {
            if (TopBorderHit(globalMousePos))
            {
                setCursor(Qt::SizeVerCursor);
            }
            else if (LeftBorderHit(globalMousePos))
            {
                setCursor(Qt::SizeHorCursor);
            }
            else if (RightBorderHit(globalMousePos))
            {
                setCursor(Qt::SizeHorCursor);
            }
            else if (BottomBorderHit(globalMousePos))
            {
                setCursor(Qt::SizeVerCursor);
            }
            else
            {
                m_isDragTop = false;
                m_isDragLeft = false;
                m_isDragRight = false;
                m_isDragBottom = false;
                QWidget::setCursor(Qt::ArrowCursor);
            }
        }
    }
}

// pos in global virtual desktop coordinates
bool FramelessWindow::LeftBorderHit(const QPoint &pos) const noexcept
{
    const QRect &rect = geometry();

    if (pos.x() >= rect.x() && pos.x() <= rect.x() + m_dragBorderSize)
    {
        return true;
    }

    return false;
}

bool FramelessWindow::RightBorderHit(const QPoint &pos) const noexcept
{
    const QRect &rect = QWidget::geometry();
    int tmp = rect.x() + rect.width();

    if (pos.x() <= tmp && pos.x() >= (tmp - m_dragBorderSize))
    {
        return true;
    }

    return false;
}

bool FramelessWindow::TopBorderHit(const QPoint &pos) const noexcept
{
    const QRect &rect = QWidget::geometry();

    if (pos.y() >= rect.y() && pos.y() <= rect.y() + m_dragBorderSize)
    {
        return true;
    }

    return false;
}

bool FramelessWindow::BottomBorderHit(const QPoint &pos) const noexcept
{
    const QRect &rect = QWidget::geometry();
    const int tmp = rect.y() + rect.height();

    if (pos.y() <= tmp && pos.y() >= (tmp - m_dragBorderSize))
    {
        return true;
    }

    return false;
}

void FramelessWindow::mousePressEvent(QMouseEvent *event)
{
    if (isMaximized())
    {
        return;
    }

    m_isMousePressed = true;
    m_startGeometry = this->geometry();

    const QPoint globalMousePos = mapToGlobal(QPoint(event->x(), event->y()));

    if (LeftBorderHit(globalMousePos) && TopBorderHit(globalMousePos))
    {
        m_isDragTop = true;
        m_isDragLeft = true;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if (RightBorderHit(globalMousePos) && TopBorderHit(globalMousePos))
    {
        m_isDragRight = true;
        m_isDragTop = true;
        setCursor(Qt::SizeBDiagCursor);
    }
    else if (LeftBorderHit(globalMousePos) && BottomBorderHit(globalMousePos))
    {
        m_isDragLeft = true;
        m_isDragBottom = true;
        setCursor(Qt::SizeBDiagCursor);
    }
    else
    {
        if (TopBorderHit(globalMousePos))
        {
            m_isDragTop = true;
            setCursor(Qt::SizeVerCursor);
        }
        else if (LeftBorderHit(globalMousePos))
        {
            m_isDragLeft = true;
            setCursor(Qt::SizeHorCursor);
        }
        else if (RightBorderHit(globalMousePos))
        {
            m_isDragRight = true;
            setCursor(Qt::SizeHorCursor);
        }
        else if (BottomBorderHit(globalMousePos))
        {
            m_isDragBottom = true;
            setCursor(Qt::SizeVerCursor);
        }
    }
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (isMaximized())
    {
        return;
    }

    m_isMousePressed = false;
    const bool bSwitchBackCursorNeeded =
        m_isDragTop || m_isDragLeft || m_isDragRight || m_isDragBottom;
    m_isDragTop = false;
    m_isDragLeft = false;
    m_isDragRight = false;
    m_isDragBottom = false;

    if (bSwitchBackCursorNeeded)
    {
        setCursor(Qt::ArrowCursor);
    }
}

bool FramelessWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (isMaximized())
    {
        return QWidget::eventFilter(obj, event);
    }

    if (QEvent::MouseMove == event->type())
    {
        QMouseEvent *const pMouse = static_cast<QMouseEvent *>(event);

        if (pMouse)
        {
            checkBorderDragging(pMouse);
        }
    }

    else if (QEvent::MouseButtonPress == event->type() && this == obj)
    {
        QMouseEvent *const pMouse = static_cast<QMouseEvent *>(event);

        if (pMouse)
        {
            mousePressEvent(pMouse);
        }
    }
    else if (QEvent::MouseButtonRelease == event->type())
    {
        if (m_isMousePressed)
        {
            QMouseEvent *const pMouse = static_cast<QMouseEvent *>(event);

            if (pMouse)
            {
                mouseReleaseEvent(pMouse);
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}


