#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent)
    : QMenu(parent)
    , m_buttonsAnimatingCurve(QEasingCurve::OutBack)
    , m_rowAnimationCurve(QEasingCurve::OutQuad)
    , m_availableGeometry(QGuiApplication::primaryScreen()->availableGeometry())
    , m_backgroundImage(QWidget::size(), QImage::Format_ARGB32_Premultiplied)
{
    OnUpdateViewColors();
    setObjectName(QStringLiteral("TrayMenu"));
    setWindowFlag(Qt::Popup, true);
    setAttribute(Qt::WA_StyledBackground);
    m_mainHorizontalLayout = new QHBoxLayout(this);
    m_mainVerticalLayout = new QVBoxLayout();
    setLayout(m_mainHorizontalLayout);
    m_mainHorizontalLayout->addLayout(m_mainVerticalLayout);
    m_mainHorizontalLayout->setMargin(0);
    m_mainHorizontalLayout->setSpacing(0);

    setStyleSheet("#TrayMenu {; border: none; border-radius:5px; }");
}

TrayMenu::~TrayMenu()
{

}

void TrayMenu::SetTrayMenuColors(const QColor& normalColor, const QColor&  hoverColor, const QColor&  pressColor, const QColor&  textColor)
{
    m_normalBackGroud = normalColor;
    m_hoverBackgroud = hoverColor;
    m_pressBackgroud = pressColor;
    m_textColor = textColor;
}

void TrayMenu::AddAction(QAction *action)
{
    TrayMenuItem* addedActionMenuItem = CreateMenuItem(action->icon(), action->text());
    addedActionMenuItem->tooltip(action->toolTip());
    connect(addedActionMenuItem, &InteractiveButtonBase::ToMouseEnter, this, [=]{ OnItemMouseEntered(addedActionMenuItem);});
}

TrayMenu *TrayMenu::BeginInsertInRow()
{
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    m_rowHorizontalLayoutsList.push_back(horizontalLayout);
    m_mainVerticalLayout->addLayout(horizontalLayout);

    m_addingHorizontMode = true;

    return this;
}

TrayMenu *TrayMenu::EndInsertInRow()
{
    m_addingHorizontMode = false;

    return this;
}

void TrayMenu::AddTextToMenu(const QString &text)
{
    QLabel* label = new QLabel(text, this);
    label->setStyleSheet("margin: 4px;");
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
        return m_rowHorizontalLayoutsList.back();
    else
        return m_mainVerticalLayout;
}

TrayMenu *TrayMenu::InstertLabelIntoMenu(QLabel *labelWidget)
{
    if (Q_NULLPTR==labelWidget)
        return this;
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
    return this;
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
    const QPalette palette=QApplication::palette();
    const QColor newBackGroundColor=palette.color(QPalette::Window);
    const QColor newHoverBackgroudColor=palette.color(QPalette::Base);  // при наведении фон
    const QColor newPressBackgroudColor=palette.color(QPalette::AlternateBase);  // при нажатии фон
    const QColor newTextColor=palette.color(QPalette::Text);
    //    qDebug()<< newBackGroundColor.name() << newHoverBackgroudColor.name() << newTextColor.name();
    SetTrayMenuColors(newBackGroundColor, newHoverBackgroudColor, newPressBackgroudColor, newTextColor);
    MakeTransparentImage();
    for (TrayMenuItem *horizontalSeparate: m_horizontalSeparatorsList)
    {
        horizontalSeparate->setNormalColor(newTextColor);
    }
}

void TrayMenu::OnItemMouseEntered(TrayMenuItem *item)
{
    if (m_isAnimationMode)
        return;
    std::vector<TrayMenuItem*>::iterator currentItemIterator= std::find(m_trayMenuItems.begin(), m_trayMenuItems.end(), item);
    if (currentItemIterator!=m_trayMenuItems.end() && m_currentElementIndex != currentItemIterator) // 屏蔽键盘操作
    {
        if(m_currentElementIndex!=m_trayMenuItems.end())
        {
            m_currentElementIndex.operator*()->discardHoverPress(true);
        }
    }
    m_currentElementIndex = currentItemIterator;
}

/**
 * Все способы создания пунктов меню
 */
TrayMenuItem *TrayMenu::CreateMenuItem(const QIcon &icon, const QString &text)
{
    TrayMenuItem* itemToAdd;

    if (icon.isNull())
    {
        itemToAdd =  new TrayMenuItem(text, this);
    }
    else
    {
        if (text.isEmpty())
        {
            itemToAdd = new TrayMenuItem(icon, this);
        }
        else
        {
            itemToAdd =  new TrayMenuItem(icon, text, this);
        }
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

/**
 * Установите стиль для кнопки (размер, цвет и т. д.)
 */
void TrayMenu::SetStyleToButton(InteractiveButtonBase *button)
{
    button->setPaddings(m_itemPadding);
    button->setNormalColor(Qt::transparent);
    button->setHoverColor(m_hoverBackgroud);
    button->setPressColor(m_pressBackgroud);
    button->setTextColor(m_textColor);

    QFont font(button->font());
    font.setWeight(QFont::Medium);
    button->setFont(font);
}

void TrayMenu::CalculateWidgetRectOnScreen()
{
    // Автоматически регулировать диапазон на основе высоты и ширины
    const int widgetHeight=QWidget::height();
    const int widgetWidht=QWidget::width();

    const QScreen *screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int heightScreen = screenGeometry.height();
    const int widthScreen = screenGeometry.width();

    const QPoint topLeftPosition=QPoint(widthScreen-widgetWidht-100, heightScreen-widgetHeight-55);

    // переместить окно
    QWidget::move(topLeftPosition);
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

void TrayMenu::MakeTransparentImage()
{
    QColor newBackgroundColor(m_normalBackGroud);
    newBackgroundColor.setAlpha(m_alphaValue);
    m_backgroundImage.fill(newBackgroundColor);
}

/**
 * Записываем в m_backgroundPixmap готовый QPixmap с афльфа каналом размытия
 */
void TrayMenu::ChangeBackgroundImageAplha()
{
    if(!m_backgroundImage.isNull())
    {
        for (int y = 0; y < m_backgroundImage.height(); ++y)
        {
            QRgb *row = (QRgb*)m_backgroundImage.scanLine(y);
            for (int x = 0; x < m_backgroundImage.width(); ++x) {
                ((unsigned char*)&row[x])[3] = m_alphaValue;
            }
        }
    }
}

QPoint TrayMenu::CalculateStartAndEndAnimationPostionFromUserMousePos() const
{
    QPoint startPosition(mapFromGlobal(QCursor::pos()));
    const QRect widgetRect(QWidget::rect());
    if(widgetRect.contains(startPosition))//нажали на кнопку
    {
        if (startPosition.x() < 0)
            startPosition.setX(0);
        else if (startPosition.x() > width())
            startPosition.setX(width());
        if (startPosition.y() < 0)
            startPosition.setY(0);
        else if (startPosition.y() > height())
            startPosition.setY(height());
        return startPosition;
    }
    else
    {
        if(startPosition.x()<widgetRect.x())//если точка слева
        {
            if(startPosition.x()+QWidget::width()>widgetRect.x())//точка слева но надписи могут выступать
            {
                startPosition.setX(startPosition.x()-QWidget::width());
            }
        }
        else
        {
            if(startPosition.x()-QWidget::width()<widgetRect.right())//точка справа, надписи могут выступать
            {
                startPosition.setX(startPosition.x()+QWidget::width());
            }
        }
    }
    return  startPosition;
}

/**
 * В меню появляется анимация при раскрытии
 */
void TrayMenu::ShowStartAnimation()
{
    /**
     * Не заворачивай анимацию в умные указатели!!!!
     * Перед любой анимацией делай виджеты недоступными!!
     */
    // анимация кнопок задается параметром m_buttonsAnimatingCurve a m_rowAnimationCurve отвечает за тип анимаций линий;

    m_mainVerticalLayout->setEnabled(false);
    m_isAnimationMode = true;

    const QPoint startPosition=CalculateStartAndEndAnimationPostionFromUserMousePos();
    HideHorizontalSeparates();

    DisclosureOfAllButtonsAnimate(startPosition)->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(m_showAnimateDurationInMiliseconds, this, [=]{
        DisclosureHorizontalSeparatesAnimate();
    });

    //включаем кнопки и режим анимации выключаем после анимации кнопок
    QTimer::singleShot(m_showAnimateDurationInMiliseconds*2, this, [=]{
        m_mainVerticalLayout->setEnabled(true);
        m_isAnimationMode = false;

    });
}

/**
 * HiddenButtonAnimationStart скрываем кнопки при закрытии
 */
void TrayMenu::HiddenButtonAnimationStart()
{
    m_isAnimationMode = true;
    m_mainVerticalLayout->setEnabled(false);

    //    AnimateFoldingHorizontalSeparates();
    QTimer::singleShot(m_hideAnimateDurationInMiliseconds, this, [=]
    {
        HideHorizontalSeparates();
        AnimateWindowDisappearance();
        //        const QPoint startPosition=CalculateStartAndEndAnimationPostionFromUserMousePos();
        //        AnimateFoldingOfAllButtons(startPosition)->start(QAbstractAnimation::DeleteWhenStopped);
    });

    QTimer::singleShot(2*m_showAnimateDurationInMiliseconds, this, [=]{
        m_isAnimationMode = false;
    });
}

void TrayMenu::DisclosureHorizontalSeparatesAnimate()
{
    for (InteractiveButtonBase* interactiveButton: m_horizontalSeparatorsList)
    {
        interactiveButton->show();
        interactiveButton->setMinimumSize(0, 0);
        QPropertyAnimation *propertyAnimation = new QPropertyAnimation(interactiveButton, "geometry", Q_NULLPTR);
        propertyAnimation->setStartValue(QRect(interactiveButton->geometry().center(), QSize(1,1)));
        propertyAnimation->setEndValue(interactiveButton->geometry());
        propertyAnimation->setEasingCurve(m_rowAnimationCurve);
        propertyAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        connect(propertyAnimation, &QPropertyAnimation::finished, propertyAnimation, &QObject::deleteLater);
        propertyAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

QParallelAnimationGroup* TrayMenu::DisclosureOfAllButtonsAnimate(const QPoint &startPosition)
{
    QParallelAnimationGroup *const allButtonsAnimation = new QParallelAnimationGroup(Q_NULLPTR);
    for (InteractiveButtonBase *item : m_trayMenuItems)
    {
        item->setBlockHover(true);
        QPropertyAnimation* propertyActionAnimation = new QPropertyAnimation(item, "pos", Q_NULLPTR);
        propertyActionAnimation->setStartValue(startPosition);
        propertyActionAnimation->setEndValue(item->pos());
        propertyActionAnimation->setEasingCurve(m_buttonsAnimatingCurve);
        propertyActionAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        connect(propertyActionAnimation, &QPropertyAnimation::finished, item, [=]{
            item->setBlockHover(false);
        });
        allButtonsAnimation->addAnimation(propertyActionAnimation);
    }
    for (QLabel *label: m_menuLabels)
    {
        QPropertyAnimation* propertyLabelAnimation = new QPropertyAnimation(label, "pos", this);
        propertyLabelAnimation->setStartValue(startPosition);
        propertyLabelAnimation->setEndValue(label->pos());
        propertyLabelAnimation->setEasingCurve(m_buttonsAnimatingCurve);
        propertyLabelAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyLabelAnimation);
    }
    return allButtonsAnimation;
}

void TrayMenu::AnimateFoldingHorizontalSeparates()
{
    for (InteractiveButtonBase* horizontalSeparate: m_horizontalSeparatorsList)
    {
        horizontalSeparate->setMinimumSize(0, 0);
        QPropertyAnimation* const horizontalSepareteHideAnimation = new QPropertyAnimation(horizontalSeparate, "geometry", Q_NULLPTR);
        horizontalSepareteHideAnimation->setEndValue(QRect(horizontalSeparate->geometry().center(), QSize(1,1)));
        horizontalSepareteHideAnimation->setEasingCurve(m_rowAnimationCurve);
        horizontalSepareteHideAnimation->setDuration(m_hideAnimateDurationInMiliseconds);
        horizontalSepareteHideAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

QParallelAnimationGroup* TrayMenu::AnimateFoldingOfAllButtons(const QPoint &startPosition)
{
    QParallelAnimationGroup *const allButtonsAnimation = new QParallelAnimationGroup(Q_NULLPTR);
    for (InteractiveButtonBase *item : m_trayMenuItems)
    {
        item->setBlockHover(true);
        QPropertyAnimation* propertyActionAnimation = new QPropertyAnimation(item, "pos", Q_NULLPTR);
        const QPoint *position=new QPoint(item->pos());
        propertyActionAnimation->setStartValue(item->pos());
        propertyActionAnimation->setEndValue(startPosition);
        propertyActionAnimation->setEasingCurve(m_buttonsAnimatingCurve);
        propertyActionAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        connect(propertyActionAnimation, &QPropertyAnimation::finished, item, [=]{
            item->setBlockHover(false);
            item->move(*position);
            delete position;
        });
        allButtonsAnimation->addAnimation(propertyActionAnimation);
    }
    for (QLabel *label: m_menuLabels)
    {
        QPropertyAnimation* propertyLabelAnimation = new QPropertyAnimation(label, "pos", Q_NULLPTR);
        const QPoint position(label->pos());
        propertyLabelAnimation->setStartValue(label->pos());
        propertyLabelAnimation->setEndValue(startPosition);
        propertyLabelAnimation->setEasingCurve(m_buttonsAnimatingCurve);
        propertyLabelAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        connect(propertyLabelAnimation, &QPropertyAnimation::finished, label, [position, label]{
            label->move(position);
        });
        allButtonsAnimation->addAnimation(propertyLabelAnimation);
    }
    return allButtonsAnimation;
}

void TrayMenu::AnimateWindowDisappearance()
{
    QPropertyAnimation *windowDisappearanceAnimation=new QPropertyAnimation(this, "m_alphaValue",this);
    windowDisappearanceAnimation->setStartValue(60);
    windowDisappearanceAnimation->setEndValue(0);
    windowDisappearanceAnimation->setDuration(m_hideAnimateDurationInMiliseconds);
    windowDisappearanceAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

int TrayMenu::GetAlphaValue() const
{
    return m_alphaValue;
}

void TrayMenu::SetAlphaValue(int alphaValue)
{
    qDebug()<< Q_FUNC_INFO << " " << alphaValue;
    m_alphaValue = alphaValue;
    ChangeBackgroundImageAplha();
    update();
}

void TrayMenu::HideHorizontalSeparates()
{
    for (InteractiveButtonBase* horizontalSeparate: m_horizontalSeparatorsList)
    {
        horizontalSeparate->hide();
    }
}

void TrayMenu::showEvent(QShowEvent *event)
{
    if(!m_isAnimationMode)
    {
        CalculateWidgetRectOnScreen();//Получаем координаты виджета, вдруг юзер разрешение поменяет
        m_currentElementIndex =m_trayMenuItems.end();//Сбрасываем текущий индекс у кнопки
        SetRoundedFormToWidget();
        ChangeBackgroundImageAplha();
        QWidget::setFocus();
        ShowStartAnimation();
        QWidget::showEvent(event);
    }
    else
    {
        event->ignore();
    }
}

void TrayMenu::closeEvent(QCloseEvent *event)
{
    if (!m_isAnimationMode)
    {
//        HiddenButtonAnimationStart();
//        QWidget::clearFocus();
//        QTimer::singleShot(m_showAnimateDurationInMiliseconds*1.5, this, &QWidget::hide);
    }
    event->ignore();
}

/**
 * Только фоновая картинка с размытием
 */
void TrayMenu::paintEvent(QPaintEvent *event)
{
    if (m_backgroundImage.isNull())
    {
        qDebug()<< Q_FUNC_INFO << " backgroud picture should not be a null";
        event->ignore();
    }
    else
    {
        QPainter painter(this);
        painter.drawImage(0,0, m_backgroundImage);
    }
}


