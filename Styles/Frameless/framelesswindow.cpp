
#include "framelesswindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>
#include <QScreen>


FramelessWindow::FramelessWindow(QWidget *parent)
    : QWidget(parent)
    , m_bMousePressed(false)
    , m_bDragTop(false)
    , m_bDragLeft(false)
    , m_bDragRight(false)
    , m_bDragBottom(false)
{

    initUI();
    setShadowUnderTitleText();
    setWindowShadow();
    insertWidgetsIntoLayout();
    applyStyles();
    setObjectNames();
    createConnections();

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground);

    setMouseTracking(true);

    QApplication::instance()->installEventFilter(this);
}

FramelessWindow::~FramelessWindow()
{

}

void FramelessWindow::restoreButtonClicked() {
    m_zoomButton->setVisible(false);
    m_maximizeButton->setVisible(true);
    setWindowState(Qt::WindowNoState);
}

void FramelessWindow::maximizeButtonClicked() {
    m_zoomButton->setVisible(true);
    m_maximizeButton->setVisible(false);
    setWindowState(Qt::WindowMaximized);
    styleWindow(true, false);
}

void FramelessWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (windowState().testFlag(Qt::WindowNoState)) {
            m_zoomButton->setVisible(false);
            m_maximizeButton->setVisible(true);
            styleWindow(true, true);
            event->ignore();
        } else if (windowState().testFlag(Qt::WindowMaximized)) {
            m_zoomButton->setVisible(true);
            m_maximizeButton->setVisible(false);
            styleWindow(true, false);
            event->ignore();
        }
    }
    event->accept();
}

void FramelessWindow::setAdminGUI(QWidget *w) {
    m_vertivalLayout->addWidget(w);
}

void FramelessWindow::setWindowTitle(const QString &text) {
    m_titleText->setText(text);
    QFont titleFont=m_titleText->font();
    titleFont.setPointSize(17);
    m_titleText->setFont(titleFont);
}

void FramelessWindow::setWindowIcon(const QIcon &icon) {
    m_icon->setPixmap(icon.pixmap(32, 32));
}

void FramelessWindow::styleWindow(bool bActive, bool bNoState) {
    if (bActive) {
        if (bNoState) {
            layout()->setMargin(15);
            m_WindowTitleBar->setStyleSheet(QStringLiteral(
                                               "#windowTitlebar{border: 0px none palette(shadow); "
                                               "border-top-left-radius:5px; border-top-right-radius:5px; "
                                               "background-color:palette(shadow); height:20px;}"));
            m_windowFrame->setStyleSheet(QStringLiteral(
                                             "#windowFrame{border:1px solid palette(highlight); border-radius:5px "
                                             "5px 5px 5px; background-color:palette(Window);}"));
            QGraphicsEffect *oldShadow = m_windowFrame->graphicsEffect();
            if (oldShadow) delete oldShadow;
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
                                               "background-color:palette(shadow); height:20px;}"));
            m_windowFrame->setStyleSheet(QStringLiteral(
                                             "#windowFrame{border:1px solid palette(dark); border-radius:0px 0px "
                                             "0px 0px; background-color:palette(Window);}"));
            QGraphicsEffect *oldShadow = m_windowFrame->graphicsEffect();
            if (oldShadow) delete oldShadow;
            m_windowFrame->setGraphicsEffect(nullptr);
        }
    } else {
        if (bNoState)
        {
            layout()->setMargin(15);
            m_WindowTitleBar->setStyleSheet(QStringLiteral(
                                               "#windowTitlebar{border: 0px none palette(shadow); "
                                               "border-top-left-radius:5px; border-top-right-radius:5px; "
                                               "background-color:palette(dark); height:20px;}"));
            m_windowFrame->setStyleSheet(QStringLiteral(
                                             "#windowFrame{border:1px solid #000000; border-radius:5px 5px 5px "
                                             "5px; background-color:palette(Window);}"));
            QGraphicsEffect *oldShadow = m_windowFrame->graphicsEffect();
            if (oldShadow) delete oldShadow;
            QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
            windowShadow->setBlurRadius(9.0);
            windowShadow->setColor(palette().color(QPalette::Shadow));
            windowShadow->setOffset(0.0);
            m_windowFrame->setGraphicsEffect(windowShadow);
        }
        else
        {
            layout()->setMargin(0);
            m_WindowTitleBar->setStyleSheet(QStringLiteral(
                                               "#titlebarWidget{border: 0px none palette(shadow); "
                                               "border-top-left-radius:0px; border-top-right-radius:0px; "
                                               "background-color:palette(dark); height:20px;}"));
            m_windowFrame->setStyleSheet(QStringLiteral(
                                             "#windowFrame{border:1px solid palette(shadow); border-radius:0px "
                                             "0px 0px 0px; background-color:palette(Window);}"));
            QGraphicsEffect *oldShadow = m_windowFrame->graphicsEffect();
            if (oldShadow) delete oldShadow;
            m_windowFrame->setGraphicsEffect(nullptr);
        }
    }
}

void FramelessWindow::on_applicationStateChanged(Qt::ApplicationState state) {
    if (state == Qt::ApplicationActive) {
        styleWindow(true, true);
    } else {
        styleWindow(false, true);
    }
}

void FramelessWindow::windowDraggerDoubleClicked() {
    if (windowState().testFlag(Qt::WindowNoState)) {
        maximizeButtonClicked();
    } else if (windowState().testFlag(Qt::WindowMaximized)) {
        restoreButtonClicked();
    }
}

void FramelessWindow::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event);
}

void FramelessWindow::checkBorderDragging(QMouseEvent *event) {
    if (isMaximized()) {
        return;
    }

    QPoint globalMousePos = event->globalPos();
    if (m_bMousePressed) {
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
        if (m_bDragTop && m_bDragRight) {
            int diff =
                    globalMousePos.x() - (m_StartGeometry.x() + m_StartGeometry.width());
            int neww = m_StartGeometry.width() + diff;
            diff = globalMousePos.y() - m_StartGeometry.y();
            int newy = m_StartGeometry.y() + diff;
            if (neww > 0 && newy > 0 && newy < h - 50) {
                QRect newg = m_StartGeometry;
                newg.setWidth(neww);
                newg.setX(m_StartGeometry.x());
                newg.setY(newy);
                setGeometry(newg);
            }
        }
        // верх лево
        else if (m_bDragTop && m_bDragLeft) {
            int diff = globalMousePos.y() - m_StartGeometry.y();
            int newy = m_StartGeometry.y() + diff;
            diff = globalMousePos.x() - m_StartGeometry.x();
            int newx = m_StartGeometry.x() + diff;
            if (newy > 0 && newx > 0) {
                QRect newg = m_StartGeometry;
                newg.setY(newy);
                newg.setX(newx);
                setGeometry(newg);
            }
        }
        // низ справа
        else if (m_bDragBottom && m_bDragLeft) {
            int diff =
                    globalMousePos.y() - (m_StartGeometry.y() + m_StartGeometry.height());
            int newh = m_StartGeometry.height() + diff;
            diff = globalMousePos.x() - m_StartGeometry.x();
            int newx = m_StartGeometry.x() + diff;
            if (newh > 0 && newx > 0) {
                QRect newg = m_StartGeometry;
                newg.setX(newx);
                newg.setHeight(newh);
                setGeometry(newg);
            }
        } else if (m_bDragTop) {
            int diff = globalMousePos.y() - m_StartGeometry.y();
            int newy = m_StartGeometry.y() + diff;
            if (newy > 0 && newy < h - 50) {
                QRect newg = m_StartGeometry;
                newg.setY(newy);
                setGeometry(newg);
            }
        } else if (m_bDragLeft) {
            int diff = globalMousePos.x() - m_StartGeometry.x();
            int newx = m_StartGeometry.x() + diff;
            if (newx > 0 && newx < w - 50) {
                QRect newg = m_StartGeometry;
                newg.setX(newx);
                setGeometry(newg);
            }
        } else if (m_bDragRight) {
            int diff =
                    globalMousePos.x() - (m_StartGeometry.x() + m_StartGeometry.width());
            int neww = m_StartGeometry.width() + diff;
            if (neww > 0) {
                QRect newg = m_StartGeometry;
                newg.setWidth(neww);
                newg.setX(m_StartGeometry.x());
                setGeometry(newg);
            }
        } else if (m_bDragBottom) {
            int diff =
                    globalMousePos.y() - (m_StartGeometry.y() + m_StartGeometry.height());
            int newh = m_StartGeometry.height() + diff;
            if (newh > 0) {
                QRect newg = m_StartGeometry;
                newg.setHeight(newh);
                newg.setY(m_StartGeometry.y());
                setGeometry(newg);
            }
        }
    } else {
        // Мышь не зажата
        if (leftBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (rightBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (leftBorderHit(globalMousePos) &&
                   bottomBorderHit(globalMousePos)) {
            setCursor(Qt::SizeBDiagCursor);
        } else {
            if (topBorderHit(globalMousePos)) {
                setCursor(Qt::SizeVerCursor);
            } else if (leftBorderHit(globalMousePos)) {
                setCursor(Qt::SizeHorCursor);
            } else if (rightBorderHit(globalMousePos)) {
                setCursor(Qt::SizeHorCursor);
            } else if (bottomBorderHit(globalMousePos)) {
                setCursor(Qt::SizeVerCursor);
            } else {
                m_bDragTop = false;
                m_bDragLeft = false;
                m_bDragRight = false;
                m_bDragBottom = false;
                setCursor(Qt::ArrowCursor);
            }
        }
    }
}

// pos in global virtual desktop coordinates
bool FramelessWindow::leftBorderHit(const QPoint &pos) {
    const QRect &rect = this->geometry();
    if (pos.x() >= rect.x() && pos.x() <= rect.x() + CONST_DRAG_BORDER_SIZE) {
        return true;
    }
    return false;
}

bool FramelessWindow::rightBorderHit(const QPoint &pos) {
    const QRect &rect = this->geometry();
    int tmp = rect.x() + rect.width();
    if (pos.x() <= tmp && pos.x() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }
    return false;
}

bool FramelessWindow::topBorderHit(const QPoint &pos) {
    const QRect &rect = this->geometry();
    if (pos.y() >= rect.y() && pos.y() <= rect.y() + CONST_DRAG_BORDER_SIZE) {
        return true;
    }
    return false;
}

bool FramelessWindow::bottomBorderHit(const QPoint &pos) {
    const QRect &rect = this->geometry();
    int tmp = rect.y() + rect.height();
    if (pos.y() <= tmp && pos.y() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }
    return false;
}

void FramelessWindow::mousePressEvent(QMouseEvent *event) {
    if (isMaximized()) {
        return;
    }

    m_bMousePressed = true;
    m_StartGeometry = this->geometry();

    QPoint globalMousePos = mapToGlobal(QPoint(event->x(), event->y()));

    if (leftBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
        m_bDragTop = true;
        m_bDragLeft = true;
        setCursor(Qt::SizeFDiagCursor);
    } else if (rightBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
        m_bDragRight = true;
        m_bDragTop = true;
        setCursor(Qt::SizeBDiagCursor);
    } else if (leftBorderHit(globalMousePos) && bottomBorderHit(globalMousePos)) {
        m_bDragLeft = true;
        m_bDragBottom = true;
        setCursor(Qt::SizeBDiagCursor);
    } else {
        if (topBorderHit(globalMousePos)) {
            m_bDragTop = true;
            setCursor(Qt::SizeVerCursor);
        } else if (leftBorderHit(globalMousePos)) {
            m_bDragLeft = true;
            setCursor(Qt::SizeHorCursor);
        } else if (rightBorderHit(globalMousePos)) {
            m_bDragRight = true;
            setCursor(Qt::SizeHorCursor);
        } else if (bottomBorderHit(globalMousePos)) {
            m_bDragBottom = true;
            setCursor(Qt::SizeVerCursor);
        }
    }
}

void FramelessWindow::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    if (isMaximized()) {
        return;
    }

    m_bMousePressed = false;
    bool bSwitchBackCursorNeeded =
            m_bDragTop || m_bDragLeft || m_bDragRight || m_bDragBottom;
    m_bDragTop = false;
    m_bDragLeft = false;
    m_bDragRight = false;
    m_bDragBottom = false;
    if (bSwitchBackCursorNeeded) {
        setCursor(Qt::ArrowCursor);
    }
}

bool FramelessWindow::eventFilter(QObject *obj, QEvent *event) {
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
        if (m_bMousePressed) {
            QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
            if (pMouse) {
                mouseReleaseEvent(pMouse);
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void FramelessWindow::initUI()
{
    m_mainLayout=new QVBoxLayout();
    m_topLayout=new QHBoxLayout();

    m_windowFrame=new QWidget();

    m_vertivalLayout=new QVBoxLayout();
    m_WindowTitleBar= new WindowTitleBar();
    m_closeButton=new CloseWindowButton(nullptr);
    m_maximizeButton=new MaximizeWindowButton(nullptr);
    m_minimizeButton=new MinimizeWindowButton(nullptr);
    m_zoomButton=new RestoreWindowButton(nullptr);
    m_icon=new QLabel();
    m_titleText=new QLabel();
    m_titleText->setAlignment(Qt::AlignLeft);
    m_zoomButton->setVisible(false);
}

void FramelessWindow::setShadowUnderTitleText()
{
    QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect;
    textShadow->setBlurRadius(2.0);
    textShadow->setColor(Qt::white);
    textShadow->setOffset(0.1);
    m_titleText->setGraphicsEffect(textShadow);
}

void FramelessWindow::setWindowShadow()
{
    QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
    windowShadow->setBlurRadius(9.0);
    windowShadow->setColor(Qt::darkBlue);
    windowShadow->setOffset(0.0);
    m_windowFrame->setGraphicsEffect(windowShadow);
}

void FramelessWindow::insertWidgetsIntoLayout()
{
    m_topLayout->addWidget(m_icon);
    m_topLayout->addWidget(m_titleText);
    m_topLayout->addWidget(m_minimizeButton);
    m_topLayout->addWidget(m_zoomButton);
    m_topLayout->addWidget(m_maximizeButton);
    m_topLayout->addWidget(m_closeButton);
    m_WindowTitleBar->setLayout(m_topLayout);

    m_vertivalLayout->addWidget(m_WindowTitleBar);
    m_windowFrame->setLayout(m_vertivalLayout);

    m_mainLayout->addWidget(m_windowFrame);

    setLayout(m_mainLayout);
}

void FramelessWindow::applyStyles()
{
    m_windowFrame->setAutoFillBackground(false);

    m_vertivalLayout->setSpacing(0);
    m_vertivalLayout->setContentsMargins(11, 11, 11, 11);
    m_vertivalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    m_vertivalLayout->setContentsMargins(1, 1, 1, 1);

    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(11, 11, 11, 11);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
}

void FramelessWindow::setObjectNames()
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

void FramelessWindow::createConnections()
{
    connect(m_minimizeButton, &QToolButton::clicked, [&](){setWindowState(Qt::WindowMinimized);});
    connect(m_maximizeButton, &QToolButton::clicked, this, &FramelessWindow::maximizeButtonClicked);
    connect(m_zoomButton, &QToolButton::clicked, this, &FramelessWindow::restoreButtonClicked);
    connect(m_closeButton, &QToolButton::clicked, [&](){close();});
    connect(m_WindowTitleBar, &WindowTitleBar::doubleClicked, this, &FramelessWindow::windowDraggerDoubleClicked);
}
