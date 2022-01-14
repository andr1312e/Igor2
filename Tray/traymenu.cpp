#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent)
    : QMenu(parent)
    , m_buttonsAnimatingCurve(QEasingCurve(QEasingCurve::OutBack))
    , m_rowAnimationCurve(QEasingCurve::OutQuad)
    , m_availableGeometry(QGuiApplication::primaryScreen()->availableGeometry())
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
    qDebug()<< newBackGroundColor.name() << newHoverBackgroudColor.name() << newTextColor.name();
    SetTrayMenuColors(newBackGroundColor, newHoverBackgroudColor, newPressBackgroudColor, newTextColor);

    for (TrayMenuItem *horizontalSeparate: m_horizontalSeparatorsList)
    {
        horizontalSeparate->setNormalColor(newTextColor);
    }
}

void TrayMenu::OnItemMouseEntered(TrayMenuItem *item)
{
    if (m_startShowingAnimationMode)
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

/**
 * Записываем в m_backgroundPixmap готовый QPixmap с афльфа каналом размытия
 */
void TrayMenu::MakeBlurImageOnBackGroundForPaintEvent()
{
    QPixmap pixmap(size());
    QPainter painter(&pixmap);

    QColor newBackgroundColor(m_normalBackGroud);
    newBackgroundColor.setAlpha(m_normalBackGroud.alpha() * (100 - m_blurAlphaValue) / 100);
    painter.fillRect(0, 0, pixmap.width(), pixmap.height(), newBackgroundColor);

    m_backgroundPixmap = pixmap.copy(m_blurRadius, m_blurRadius, pixmap.width()-m_blurRadius*2, pixmap.height()-m_blurRadius*2);
}

/**
 * В меню появляется анимация при раскрытии
 */
void TrayMenu::ShowedButtonAnimationStart()
{
    /**
     * Не заворачивай анимацию в умные указатели!!!!
     * Перед любой анимацией делай виджеты недоступными!!
     */
    // анимация кнопок задается параметром m_buttonsAnimatingCurve a m_rowAnimationCurve отвечает за тип анимаций линий;
    m_mainVerticalLayout->setEnabled(false);
    m_startShowingAnimationMode = true;


    QPoint startPosition = mapFromGlobal(QCursor::pos());
    qDebug()<< m_trayMenuItems.size() << " pos " << startPosition;
    for (InteractiveButtonBase *item : m_trayMenuItems)
    {
        qDebug()<< item->pos();
    }
    if (startPosition.x() < 0)
        startPosition.setX(0);
    else if (startPosition.x() > width())
        startPosition.setX(width());
    if (startPosition.y() < 0)
        startPosition.setY(0);
    else if (startPosition.y() > height())
        startPosition.setY(height());

    for (InteractiveButtonBase *item : m_trayMenuItems)
    {
        item->setBlockHover(true);
        QPropertyAnimation* propertyActionAnimation = new QPropertyAnimation(item, "pos", Q_NULLPTR);
        propertyActionAnimation->setStartValue(startPosition);
        propertyActionAnimation->setEndValue(item->pos());
        propertyActionAnimation->setEasingCurve(m_buttonsAnimatingCurve);
        propertyActionAnimation->setDuration(m_durationToAnimateElementsMiliseconds);
        connect(propertyActionAnimation, &QPropertyAnimation::finished, propertyActionAnimation, &QObject::deleteLater);
        connect(propertyActionAnimation, &QPropertyAnimation::finished, item, [=]{
            item->setBlockHover(false);
        });
        propertyActionAnimation->start();
    }
    for (QLabel *label: m_menuLabels)
    {
        QPropertyAnimation* propertyLabelAnimation = new QPropertyAnimation(label, "pos", Q_NULLPTR);
        propertyLabelAnimation->setStartValue(startPosition);
        propertyLabelAnimation->setEndValue(label->pos());
        propertyLabelAnimation->setEasingCurve(m_buttonsAnimatingCurve);
        propertyLabelAnimation->setDuration(m_durationToAnimateElementsMiliseconds);
        connect(propertyLabelAnimation, &QPropertyAnimation::finished, propertyLabelAnimation, &QObject::deleteLater);
        propertyLabelAnimation->start();
    }

    // скрыть строки разделители и потом анимировать
    for (TrayMenuItem *horizontalSeparate: m_horizontalSeparatorsList)
    {
        horizontalSeparate->hide();
    }

    QTimer::singleShot(m_durationToAnimateElementsMiliseconds, this, [=]{
        for (InteractiveButtonBase* interactiveButton: m_horizontalSeparatorsList)
        {
            interactiveButton->show();
            interactiveButton->setMinimumSize(0, 0);
            QPropertyAnimation *propertyAnimation = new QPropertyAnimation(interactiveButton, "geometry", Q_NULLPTR);
            propertyAnimation->setStartValue(QRect(interactiveButton->geometry().center(), QSize(1,1)));
            propertyAnimation->setEndValue(interactiveButton->geometry());
            propertyAnimation->setEasingCurve(m_rowAnimationCurve);
            propertyAnimation->setDuration(m_durationToAnimateElementsMiliseconds);
            connect(propertyAnimation, &QPropertyAnimation::finished, propertyAnimation, &QObject::deleteLater);
            propertyAnimation->start();
        }
    });
    //включаем кнопки и режим анимации выключаем после анимации кнопок
    QTimer::singleShot(m_durationToAnimateElementsMiliseconds, this, [=]{
        m_mainVerticalLayout->setEnabled(true);
        m_startShowingAnimationMode = false;
    });
}

/**
 * 关闭前显示隐藏动画
 * @param focusIndex 聚焦的item，如果不存在则为-1
 */
void TrayMenu::HiddenButtonAnimationStart(std::vector<TrayMenuItem*>::iterator indexOfButtonInFocusNow)
{
    m_mainVerticalLayout->setEnabled(false);
    m_startShowingAnimationMode = true;
    // Управление анимацией движения
    int up_end = m_trayMenuItems.size() ? -m_trayMenuItems.at(0)->height() : 0;
    int flow_end = height();
    //    int focus_top = focusIndex > -1 ? m_trayMenuItems.at(focusIndex)->pos().y() : 0;

    if (indexOfButtonInFocusNow == m_trayMenuItems.end())
    {
        for (TrayMenuItem * item: m_trayMenuItems)
        {
            item->setBlockHover(true);
            QPropertyAnimation* propertyAnimation = new QPropertyAnimation(item, "pos", Q_NULLPTR);
            propertyAnimation->setEasingCurve(QEasingCurve::OutCubic);
            propertyAnimation->setStartValue(item->pos());
            propertyAnimation->setEndValue(QPoint(0, 0));
            propertyAnimation->setDuration(m_durationToAnimateElementsMiliseconds);
            connect(propertyAnimation, &QPropertyAnimation::finished, propertyAnimation, &QObject::deleteLater);
            connect(propertyAnimation, &QPropertyAnimation::finished, item, [=]{
                item->setBlockHover(false);
                item->move(item->pos());
            });
            propertyAnimation->start();
        }
    }
    else
    {
        //        for (std::vector<TrayMenuItem*>::iterator it=m_trayMenuItems.begin(); it!=m_trayMenuItems.end(); ++it)
        //        {
        //            (*it)->setBlockHover(true);
        //            QPropertyAnimation* propertyAnimation = new QPropertyAnimation(*it, "pos");
        //            propertyAnimation->setStartValue((*it)->pos());
        //            propertyAnimation->setEasingCurve(QEasingCurve::OutCubic);

        //            if (std::distance(it, indexOfButtonInFocusNow)>0) // 上面的项
        //            {
        //                propertyAnimation->setEndValue(QPoint(0, up_end - (focus_top - (*it)->pos().y()) / 8));
        //                propertyAnimation->setDuration(m_durationToAnimateElementsMiliseconds - qAbs(focusIndex-i)*(m_durationToAnimateElementsMiliseconds-dur_min)/up_flow_count);
        //            }
        //            else if (*it == *indexOfButtonInFocusNow) // 中间的项
        //            {
        //                propertyAnimation->setEndValue((*it)->pos());
        //                propertyAnimation->setDuration(m_durationToAnimateElementsMiliseconds);
        //            }
        //            else // 下面的项
        //            {
        //                propertyAnimation->setEndValue(QPoint(0, flow_end + ((*it)->pos().y()-focus_top) / 8));
        //                propertyAnimation->setDuration(m_durationToAnimateElementsMiliseconds - qAbs(i-focusIndex)*(m_durationToAnimateElementsMiliseconds-dur_min)/up_flow_count);
        //            }
        //            connect(propertyAnimation, &QPropertyAnimation::finished, propertyAnimation, &QObject::deleteLater);
        //            connect(propertyAnimation, &QPropertyAnimation::finished, button, [=]{
        //                (*it)->setBlockHover(false);
        //                (*it)->move(pos);
        //            });
        //            propertyAnimation->start();
        //        }
    }

    //    // 第三方控件动画
    //    for (int i = 0; i < m_menuLabels.size(); i++)
    //    {
    //        QWidget* btn = m_menuLabels.at(i);

    //        QPropertyAnimation* ani = new QPropertyAnimation(btn, "pos");
    //        ani->setStartValue(btn->pos());
    //        ani->setEasingCurve(QEasingCurve::OutCubic);
    //        if (focusIndex > -1)
    //        {
    //            if (i < focusIndex) // 上面的项
    //            {
    //                ani->setEndValue(QPoint(0, up_end - (focus_top - btn->pos().y()) / 8));
    //                ani->setDuration(dur_max - qAbs(focusIndex-i)*(dur_max-dur_min)/up_flow_count);
    //            }
    //            else if (i == focusIndex) // 中间的项
    //            {
    //                ani->setEndValue(btn->pos());
    //                ani->setDuration(dur_max);
    //            }
    //            else // 下面的项
    //            {
    //                ani->setEndValue(QPoint(0, flow_end + (btn->pos().y()-focus_top) / 8));
    //                ani->setDuration(dur_max - qAbs(i-focusIndex)*(dur_max-dur_min)/up_flow_count);
    //            }
    //        }
    //        else
    //        {
    //            ani->setEndValue(QPoint(0, up_end));
    //            ani->setDuration(dur_max);
    //        }
    //        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
    //        // connect(ani, &QPropertyAnimation::finished, btn, [=]{
    //        //     btn->move(pos);
    //        // });
    //        ani->start();
    //    }

    //    // 变淡动画（针对Popup，一切透明无效）
    //    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
    //    effect->setOpacity(1);
    //    QWidget::setGraphicsEffect(effect);
    //    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(effect, "opacity");
    //    opacityAnimation->setDuration(dur_max * 0.8);
    //    opacityAnimation->setStartValue(1);
    //    opacityAnimation->setEndValue(0);
    //    connect(opacityAnimation, &QPropertyAnimation::finished, opacityAnimation, &QObject::deleteLater);
    //    connect(opacityAnimation, &QPropertyAnimation::finished, effect, &QObject::deleteLater);
    //    opacityAnimation->start();

    //    // 真正关闭

}

void TrayMenu::showEvent(QShowEvent *event)
{
    CalculateWidgetRectOnScreen();//Получаем координаты виджета, вдруг юзер разрешение поменяет
    m_currentElementIndex =m_trayMenuItems.end();//Сбрасываем текущий индекс у кнопки
    SetRoundedFormToWidget();
    MakeBlurImageOnBackGroundForPaintEvent();
    QWidget::setFocus();
    ShowedButtonAnimationStart();
    QWidget::showEvent(event);
}

void TrayMenu::hideEvent(QHideEvent *event)
{
    HiddenButtonAnimationStart(m_trayMenuItems.end());
    QTimer::singleShot(m_durationToAnimateElementsMiliseconds, this, [=]{
        m_mainVerticalLayout->setEnabled(true);
        m_startShowingAnimationMode = false;
        QWidget::clearFocus();
        QWidget::hideEvent(event);
    });

}
/**
 * Только фоновая картинка с размытием
 */
void TrayMenu::paintEvent(QPaintEvent *event)
{
    if (m_backgroundPixmap.isNull())
    {
        QWidget::paintEvent(event);
    }
    else
    {
        QPainter painter(this);
        painter.drawPixmap(0,0,width(),height(), m_backgroundPixmap);
    }
}


