#include "traymenu.h"
#include <QDebug>

TrayMenu::TrayMenu(QWidget *parent)
    : QMenu(parent)
    , m_backgroundImage(QWidget::size(), QImage::Format_ARGB32_Premultiplied)
{
    OnUpdateViewColors();
    setObjectName(QStringLiteral("TrayMenu"));
    setWindowFlag(Qt::Popup, true);
    setAttribute(Qt::WA_StyledBackground);
    m_mainVerticalLayout = new QVBoxLayout(this);
    m_mainVerticalLayout->setMargin(0);
    m_mainVerticalLayout->setSpacing(1);

    setStyleSheet(QStringLiteral("#TrayMenu {; border: none; border-radius:5px; }"));
}

TrayMenu::~TrayMenu()
{

}

void TrayMenu::AddButtonToMenu(QAction *action)
{
    TrayMenuItem* const addedActionMenuItem = CreateMenuItem(action);
    addedActionMenuItem->setToolTip(action->toolTip());
    connect(addedActionMenuItem, &QPushButton::clicked, action, &QAction::trigger);
    connect(addedActionMenuItem, &QPushButton::clicked, this, &TrayMenu::OnChangeButtonIcon);
}

void TrayMenu::BeginInsertInRow()
{
    QHBoxLayout* const horizontalLayout = new QHBoxLayout();
    m_rowHorizontalLayoutsList.push_back(horizontalLayout);
    m_mainVerticalLayout->addLayout(horizontalLayout);
    m_addingHorizontMode = true;
}

void TrayMenu::EndInsertInRow()
{
    m_addingHorizontMode = false;
}

void TrayMenu::AddTextToMenu(const QString &text)
{
    QLabel* const label = new QLabel(text, this);
    label->setStyleSheet(QStringLiteral("margin: 4px;"));
    InstertLabelIntoMenu(label);
}
/**
  * Получить текущий макет
  * Если он выкладывается горизонтально, вернуть макет (дочерний элемент) горизонтального макета
  * в противном случае вернуть общий макет вертикального макета
  */
QBoxLayout *TrayMenu::GetCurrentLayout() const
{
    if (m_addingHorizontMode && !m_rowHorizontalLayoutsList.empty())
    {
        return m_rowHorizontalLayoutsList.back();
    }
    else
    {
        return m_mainVerticalLayout;
    }
}

void TrayMenu::InstertLabelIntoMenu(QLabel *labelWidget)
{
    QBoxLayout* layoutToInsert = Q_NULLPTR;
    if (m_addingHorizontMode && !m_rowHorizontalLayoutsList.empty()) // Если он добавляет горизонтальную кнопку
    {
        layoutToInsert = m_rowHorizontalLayoutsList.back();
    }
    else
    {
        layoutToInsert = m_mainVerticalLayout;
    }
    layoutToInsert->addWidget(labelWidget);
    m_menuLabels.push_back(labelWidget);
}

TrayMenuItem *TrayMenu::CreateMenuItem(QAction *action)
{
    TrayMenuItem* itemToAdd;

    if (action->text().isEmpty())
    {
        itemToAdd =  new TrayMenuItem(this);
    }
    else
    {
        itemToAdd =  new TrayMenuItem(action->text(), this);
    }
    if(!action->objectName().isEmpty())//если путь к иконке есть
    {
        itemToAdd->SetIconPath(action->objectName());//Путь к иконке
    }
    SetStyleToButton(itemToAdd);

    if (m_addingHorizontMode && !m_rowHorizontalLayoutsList.empty()) // Если он добавляет горизонтальную кнопку
    {
        m_rowHorizontalLayoutsList.back()->addWidget(itemToAdd);
    }
    else
    {
        m_mainVerticalLayout->addWidget(itemToAdd);
    }
    m_trayMenuItems.push_back(itemToAdd);
    return itemToAdd;
}

void TrayMenu::AddSpacing(int size)
{
    GetCurrentLayout()->addSpacing(size);
}

/**
 * Добавить горизонтальную разделительную линию
 * не обязательно требуется
 */
void TrayMenu::AddSeparatorLineHorizontal()
{
    TrayMenuItem* item = new TrayMenuItem(this);
    item->setFixedHeight(1);
    item->setPaddings(32, 32, 0, 0);
    item->setDisabled(true);

    m_mainVerticalLayout->addWidget(item);
    m_horizontalSeparatorsList.push_back(item);
}

void TrayMenu::OnUpdateViewColors()
{
    const QPalette palette=qApp->palette();
    const QColor newBackGroundColor=palette.color(QPalette::Window);
    const QColor newHoverBackgroudColor=palette.color(QPalette::Base);  // при наведении фон
    const QColor newTextColor=palette.color(QPalette::Text);
    //    qDebug()<< newBackGroundColor.name() << newHoverBackgroudColor.name() << newTextColor.name();
    m_normalBackGroud = newBackGroundColor;
    MakeTransparentImage();
    for (TrayMenuItem * const horizontalSeparate: qAsConst(m_horizontalSeparatorsList))
    {
        horizontalSeparate->SetNormalColor(newTextColor);
    }
    for (TrayMenuItem *const item:qAsConst(m_trayMenuItems))
    {
        item->SetNormalColor(newBackGroundColor);
        item->SetHoverColor(newHoverBackgroudColor);
        item->SetTextColor(newTextColor);
    }
}

void TrayMenu::OnChangeButtonIcon()
{
    TrayMenuItem * const item=qobject_cast<TrayMenuItem*>(sender());
    if(Q_NULLPTR==item)
    {
        qFatal("ffdf");
    }
    else
    {

    }
}

/**
 * Установите стиль для кнопки (размер, цвет и т. д.)
 */
void TrayMenu::SetStyleToButton(InteractiveButtonBase *button)
{
    button->setPaddings(m_itemPadding);
    QFont font(button->font());
    font.setWeight(QFont::Medium);
    button->setFont(font);
}

QPoint TrayMenu::CalculateTopLeftWidgetPosOnScreen() const
{
    // Автоматически регулировать диапазон на основе высоты и ширины
    const int widgetHeight=QWidget::height();
    const int widgetWidht=QWidget::width();

    const QScreen *screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int heightScreen = screenGeometry.height();
    const int widthScreen = screenGeometry.width();

    const QPoint topLeftPosition=QPoint(widthScreen-widgetWidht-100, heightScreen-widgetHeight-55);
    return topLeftPosition;
}

/**
 * создаем прямоугольник со скругленными углами, ставим его как маску виджета
 */
void TrayMenu::SetRoundedFormToWidget()
{
    QPixmap pixmap(size());//создаем картинку
    pixmap.fill(Qt::transparent);//заполняем прозрачным все
    QPainter pixmapPainter(&pixmap);
    pixmapPainter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), m_borderRadius, m_borderRadius);//заполняем белым все кроме углов, они прозрачные
    pixmapPainter.fillPath(path, Qt::white);
    QWidget::setMask(pixmap.mask());
}

/**
 * Записываем в m_backgroundPixmap готовый QPixmap с афльфа каналом размытия
 */
void TrayMenu::MakeTransparentImage()
{
    QColor newBackgroundColor(m_normalBackGroud);
    newBackgroundColor.setAlpha(m_alphaValue);
    m_backgroundImage.fill(newBackgroundColor);
}

/**
 * В меню появляется анимация при раскрытии
 * Не заворачивай анимацию в умные указатели!!!!
 * Перед любой анимацией делай виджеты недоступными!!
 */
void TrayMenu::ShowAnimationPlay()
{
    // анимация кнопок задается параметром m_buttonsAnimatingCurve a m_rowAnimationCurve отвечает за тип анимаций линий;
    m_mainVerticalLayout->setEnabled(false);
    //Анимировать показать менюшку
    AnimateDisclosureOfAllButtons()->start(QAbstractAnimation::DeleteWhenStopped);
    //включаем кнопки и режим анимации выключаем после анимации кнопок
    QTimer::singleShot(m_showAnimateDurationInMiliseconds, this, [&]{
        m_mainVerticalLayout->setEnabled(true);
    });
}

void TrayMenu::HiddenAnimationPlay()
{
    m_mainVerticalLayout->setEnabled(false);
    AnimateFoldingOfAllButtons()->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(m_showAnimateDurationInMiliseconds, this, [&]{
        m_mainVerticalLayout->setEnabled(true);
    });
}

QParallelAnimationGroup *TrayMenu::AnimateDisclosureOfAllButtons()
{
    QParallelAnimationGroup *const allButtonsAnimation = new QParallelAnimationGroup(Q_NULLPTR);
    for (InteractiveButtonBase *const item : qAsConst(m_trayMenuItems))
    {
        QGraphicsOpacityEffect * const graphicsButtonOpacity = new QGraphicsOpacityEffect(Q_NULLPTR);
        graphicsButtonOpacity->setOpacity(1);
        item->setGraphicsEffect(graphicsButtonOpacity);
        QPropertyAnimation* const propertyActionAnimation = new QPropertyAnimation(graphicsButtonOpacity, "opacity", Q_NULLPTR);
        propertyActionAnimation->setStartValue(0);
        propertyActionAnimation->setEndValue(1);
        propertyActionAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyActionAnimation);
    }
    for (QLabel *const label: qAsConst(m_menuLabels))
    {
        QGraphicsOpacityEffect * const graphicsButtonOpacity = new QGraphicsOpacityEffect(Q_NULLPTR);
        graphicsButtonOpacity->setOpacity(1);
        label->setGraphicsEffect(graphicsButtonOpacity);
        QPropertyAnimation* const propertyLabelAnimation = new QPropertyAnimation(graphicsButtonOpacity, "opacity", Q_NULLPTR);
        propertyLabelAnimation->setStartValue(0);
        propertyLabelAnimation->setEndValue(1);
        propertyLabelAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyLabelAnimation);
    }
    for (InteractiveButtonBase* const horizontalSeparate: qAsConst(m_horizontalSeparatorsList))
    {
        horizontalSeparate->show();
        horizontalSeparate->setMinimumSize(0, 0);
        QPropertyAnimation * const propertyAnimation = new QPropertyAnimation(horizontalSeparate, "geometry", Q_NULLPTR);
        propertyAnimation->setStartValue(QRect(horizontalSeparate->geometry().center(), QSize(1,1)));
        propertyAnimation->setEndValue(horizontalSeparate->geometry());
        propertyAnimation->setEasingCurve(m_rowAnimationCurve);
        propertyAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyAnimation);
    }
    return allButtonsAnimation;
}

void TrayMenu::showEvent(QShowEvent *event)
{
    const QPoint topLeftPosition=CalculateTopLeftWidgetPosOnScreen();//Получаем координаты виджета, вдруг юзер разрешение поменяет
    QWidget::move(topLeftPosition);
    SetRoundedFormToWidget();
    MakeTransparentImage();
    QWidget::setFocus();
    ShowAnimationPlay();
    QWidget::showEvent(event);
}

void TrayMenu::closeEvent(QCloseEvent *event)
{
    //    qDebug()<< Q_FUNC_INFO;
    event->ignore();
    HiddenAnimationPlay();
    QWidget::clearFocus();
    QTimer::singleShot(m_showAnimateDurationInMiliseconds, this, &QWidget::hide);
}
/**
 * Только фоновая картинка с размытием
 */
void TrayMenu::paintEvent(QPaintEvent *event)
{
    if (m_backgroundImage.isNull())
    {
        QWidget::paintEvent(event);
    }
    else
    {
        QPainter painter(this);
        painter.drawImage(0,0, m_backgroundImage);
    }
}

QParallelAnimationGroup *TrayMenu::AnimateFoldingOfAllButtons()
{
    QParallelAnimationGroup *const allButtonsAnimation = new QParallelAnimationGroup(Q_NULLPTR);
    for (InteractiveButtonBase *const item : qAsConst(m_trayMenuItems))
    {
        QGraphicsOpacityEffect * const graphicsButtonOpacity = new QGraphicsOpacityEffect(Q_NULLPTR);
        graphicsButtonOpacity->setOpacity(1);
        item->setGraphicsEffect(graphicsButtonOpacity);
        QPropertyAnimation* const propertyActionAnimation = new QPropertyAnimation(graphicsButtonOpacity, "opacity", Q_NULLPTR);
        propertyActionAnimation->setStartValue(1);
        propertyActionAnimation->setEndValue(0);
        propertyActionAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyActionAnimation);
    }
    for (QLabel *const label: qAsConst(m_menuLabels))
    {
        QGraphicsOpacityEffect * const graphicsButtonOpacity = new QGraphicsOpacityEffect(Q_NULLPTR);
        graphicsButtonOpacity->setOpacity(1);
        label->setGraphicsEffect(graphicsButtonOpacity);
        QPropertyAnimation* const propertyLabelAnimation = new QPropertyAnimation(graphicsButtonOpacity, "opacity", Q_NULLPTR);
        propertyLabelAnimation->setStartValue(1);
        propertyLabelAnimation->setEndValue(0);
        propertyLabelAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyLabelAnimation);
    }
    for (InteractiveButtonBase* const horizontalSeparate: qAsConst(m_horizontalSeparatorsList))
    {
        horizontalSeparate->setMinimumSize(0, 0);
        QPropertyAnimation * const horizontalSepareteHideAnimation = new QPropertyAnimation(horizontalSeparate, "geometry", Q_NULLPTR);
        horizontalSepareteHideAnimation->setStartValue(horizontalSeparate->geometry());
        horizontalSepareteHideAnimation->setEndValue(QRect(horizontalSeparate->geometry().center(), QSize(1,1)));
        horizontalSepareteHideAnimation->setEasingCurve(m_rowAnimationCurve);
        horizontalSepareteHideAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(horizontalSepareteHideAnimation);
    }
    return allButtonsAnimation;
}
