#include "traymenu.h"
#include <QDebug>

TrayMenu::TrayMenu(QWidget *parent)
    : QMenu(parent)
    , m_backgroundImage(QWidget::size(), QImage::Format_ARGB32_Premultiplied)
{
    //    OnUpdateViewColors();
    setObjectName(QStringLiteral("TrayMenu"));
    setWindowFlag(Qt::Popup, true);
    setAttribute(Qt::WA_StyledBackground);
    m_mainVerticalLayout = new QVBoxLayout(this);
    m_mainVerticalLayout->setMargin(0);
    m_mainVerticalLayout->setSpacing(1);
}

TrayMenu::~TrayMenu()
{

}

void TrayMenu::AddButtonToMenu(QAction *action)
{
    TrayMenuItem *const addedActionMenuItem = CreateMenuItem(action);
    addedActionMenuItem->setToolTip(action->toolTip());
    connect(addedActionMenuItem, &QPushButton::clicked, action, &QAction::trigger);
    const QString actionIcon=action->objectName();
    if(!actionIcon.isEmpty())
    {
        connect(addedActionMenuItem, &QPushButton::clicked, this, &TrayMenu::OnChangeButtonIcon);
    }
}

void TrayMenu::BeginInsertInRow()
{
    QHBoxLayout *const horizontalLayout = new QHBoxLayout();
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
    QLabel *const label = new QLabel(text, this);
    label->setObjectName(QLatin1Literal("TrayMenuLabel"));
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
    QBoxLayout *layoutToInsert = Q_NULLPTR;
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
    TrayMenuItem *itemToAdd;

    if (action->text().isEmpty())
    {
        itemToAdd =  new TrayMenuItem(this);
    }
    else
    {
        itemToAdd =  new TrayMenuItem(action->text(), this);
    }
    if (!action->objectName().isEmpty()) //если путь к иконке есть
    {
        itemToAdd->SetIcon(action->objectName());//Путь к иконке
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
    TrayMenuItem *const item = new TrayMenuItem(this);
    item->setFixedHeight(1);
    item->SetPaddings(32, 32, 0, 0);
    item->SetDisabled(true);

    m_mainVerticalLayout->addWidget(item);
    m_horizontalSeparatorsList.push_back(item);
}

void TrayMenu::OnUpdateViewColors(ThemesNames themeName)
{
    const QPalette palette = qApp->palette();
    const QColor newBackGroundColor = palette.color(QPalette::Window);
    const QColor newHoverBackgroudColor = palette.color(QPalette::Base); // при наведении фон
    const QColor newTextColor = palette.color(QPalette::Text);
    m_normalBackGroud = newBackGroundColor;
    MakeTransparentImage();
    for (TrayMenuItem *const horizontalSeparate : qAsConst(m_horizontalSeparatorsList))
    {
        horizontalSeparate->SetNormalColor(newTextColor);
    }
    for (TrayMenuItem *const item : qAsConst(m_trayMenuItems))
    {
        item->SetNormalColor(newBackGroundColor);
        item->SetHoverColor(newHoverBackgroudColor);
        item->SetTextColor(newTextColor);
        if (item->HasIcon())
        {
            item->ChangeButtonIconColor(themeName);
        }
    }
}
/**
 * Меняем По клику на кажду кнопку которая в горигонте
 * Идем по
 */
void TrayMenu::OnChangeButtonIcon()
{
    TrayMenuItem *const itemSender = qobject_cast<TrayMenuItem *>(sender());
    const QHBoxLayout *senderWidgetLayout = Q_NULLPTR;
    if (Q_NULLPTR == itemSender)
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Нереализованное поведение")).toUtf8().constData());
    }
    else
    {
        bool findet = false;
        for (const QHBoxLayout  *const layout : m_rowHorizontalLayoutsList)
        {
            for (int widgetIndex = 0; widgetIndex < layout->count(); ++widgetIndex)
            {
                TrayMenuItem *const itemInLayout = qobject_cast<TrayMenuItem *>(layout->itemAt(widgetIndex)->widget());
                if (itemInLayout == itemSender)
                {
                    findet = true;
                    break;
                }
            }
            if (findet)
            {
                senderWidgetLayout = layout;
                break;
            }
        }
        if (Q_NULLPTR  == senderWidgetLayout)
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Нереализованное поведение. Не найден горизонтальный компоновщик")).toUtf8().constData());
        }
        else
        {
            for (int widgetIndex = 0; widgetIndex < senderWidgetLayout->count(); ++widgetIndex)
            {
                TrayMenuItem *const itemInLayout = qobject_cast<TrayMenuItem *>(senderWidgetLayout->itemAt(widgetIndex)->widget());
                if (itemSender == itemInLayout)
                {
                    itemInLayout->ChangeButtonIconEnabled();
                }
                else
                {
                    itemInLayout->ChangeButtonIconDisabled();
                }
            }
        }
    }
}

/**
 * Установите стиль для кнопки (размер, цвет и т. д.)
 */
void TrayMenu::SetStyleToButton(InteractiveButton *button)
{
    button->SetPaddings(m_itemPadding);
    QFont font(button->font());
    font.setWeight(QFont::Medium);
    button->setFont(font);
}

QPoint TrayMenu::CalculateTopLeftWidgetPosOnScreen() const
{
    // Автоматически регулировать диапазон на основе высоты и ширины
    const int widgetHeight = QWidget::height();
    const int widgetWidht = QWidget::width();

    const QScreen *const screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int heightScreen = screenGeometry.height();
    const int widthScreen = screenGeometry.width();

    const QPoint topLeftPosition = QPoint(widthScreen - widgetWidht - 100, heightScreen - widgetHeight - 55);
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
    qDebug() << "ShowAnimationPlay";
    // анимация кнопок задается параметром m_buttonsAnimatingCurve a m_rowAnimationCurve отвечает за тип анимаций линий;
    m_mainVerticalLayout->setEnabled(false);
    //Анимировать показать менюшку
    AnimateExpansionOfAllButtons()->start(QAbstractAnimation::DeleteWhenStopped);
    //включаем кнопки и режим анимации выключаем после анимации кнопок
    QTimer::singleShot(m_showAnimateDurationInMiliseconds, this, [&]
    {
        m_mainVerticalLayout->setEnabled(true);
    });
}

void TrayMenu::HiddenAnimationPlay()
{
    m_mainVerticalLayout->setEnabled(false);
    AnimateFoldingOfAllButtons()->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(m_showAnimateDurationInMiliseconds, this, [&]
    {
        m_mainVerticalLayout->setEnabled(true);
    });
}

QParallelAnimationGroup *TrayMenu::AnimateExpansionOfAllButtons()
{
    QParallelAnimationGroup *const allButtonsAnimation = new QParallelAnimationGroup(Q_NULLPTR);
    for (InteractiveButton *const item : qAsConst(m_trayMenuItems))
    {
        QGraphicsOpacityEffect *const graphicsButtonOpacity = new QGraphicsOpacityEffect(Q_NULLPTR);
        graphicsButtonOpacity->setOpacity(1);
        item->setGraphicsEffect(graphicsButtonOpacity);
        QPropertyAnimation *const propertyActionAnimation = new QPropertyAnimation(graphicsButtonOpacity, "opacity", Q_NULLPTR);
        propertyActionAnimation->setStartValue(0);
        propertyActionAnimation->setEndValue(1);
        propertyActionAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyActionAnimation);
    }
    for (QLabel *const label : qAsConst(m_menuLabels))
    {
        QGraphicsOpacityEffect *const graphicsButtonOpacity = new QGraphicsOpacityEffect(Q_NULLPTR);
        graphicsButtonOpacity->setOpacity(1);
        label->setGraphicsEffect(graphicsButtonOpacity);
        QPropertyAnimation *const propertyLabelAnimation = new QPropertyAnimation(graphicsButtonOpacity, "opacity", Q_NULLPTR);
        propertyLabelAnimation->setStartValue(0);
        propertyLabelAnimation->setEndValue(1);
        propertyLabelAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyLabelAnimation);
    }
    for (TrayMenuItem *const horizontalSeparate : qAsConst(m_horizontalSeparatorsList))
    {
        horizontalSeparate->setMinimumSize(1, 1);
        QPropertyAnimation *const propertyAnimation = new QPropertyAnimation(horizontalSeparate, "geometry", Q_NULLPTR);
        propertyAnimation->setStartValue(QRect(horizontalSeparate->geometry().center(), QSize(1, 1)));
        if (m_isFirstAnimation)
        {
            horizontalSeparate->SetEndAnimationGeometry(horizontalSeparate->geometry());
            propertyAnimation->setEndValue(horizontalSeparate->geometry());
        }
        else
        {
            propertyAnimation->setEndValue(horizontalSeparate->GetEndAnimationRect());
        }
        propertyAnimation->setEasingCurve(m_rowAnimationCurve);
        propertyAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyAnimation);
    }
    m_isFirstAnimation = false;
    return allButtonsAnimation;
}

void TrayMenu::showEvent(QShowEvent *event)
{
    const QPoint topLeftPosition = CalculateTopLeftWidgetPosOnScreen(); //Получаем координаты виджета, вдруг юзер разрешение поменяет
    QWidget::move(topLeftPosition);
    SetRoundedFormToWidget();
    MakeTransparentImage();
    QWidget::setFocus();
    ShowAnimationPlay();
    QWidget::showEvent(event);
}

void TrayMenu::closeEvent(QCloseEvent *event)
{
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
        painter.drawImage(0, 0, m_backgroundImage);
    }
}

QParallelAnimationGroup *TrayMenu::AnimateFoldingOfAllButtons()
{
    QParallelAnimationGroup *const allButtonsAnimation = new QParallelAnimationGroup(Q_NULLPTR);
    for (InteractiveButton *const item : qAsConst(m_trayMenuItems))
    {
        QGraphicsOpacityEffect *const graphicsButtonOpacity = new QGraphicsOpacityEffect(Q_NULLPTR);
        graphicsButtonOpacity->setOpacity(1);
        item->setGraphicsEffect(graphicsButtonOpacity);
        QPropertyAnimation *const propertyActionAnimation = new QPropertyAnimation(graphicsButtonOpacity, "opacity", Q_NULLPTR);
        propertyActionAnimation->setStartValue(1);
        propertyActionAnimation->setEndValue(0);
        propertyActionAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyActionAnimation);
    }
    for (QLabel *const label : qAsConst(m_menuLabels))
    {
        QGraphicsOpacityEffect *const graphicsButtonOpacity = new QGraphicsOpacityEffect(Q_NULLPTR);
        graphicsButtonOpacity->setOpacity(1);
        label->setGraphicsEffect(graphicsButtonOpacity);
        QPropertyAnimation *const propertyLabelAnimation = new QPropertyAnimation(graphicsButtonOpacity, "opacity", Q_NULLPTR);
        propertyLabelAnimation->setStartValue(1);
        propertyLabelAnimation->setEndValue(0);
        propertyLabelAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyLabelAnimation);
    }
    for (TrayMenuItem *const horizontalSeparate : qAsConst(m_horizontalSeparatorsList))
    {
        horizontalSeparate->setMinimumSize(1, 1);
        QPropertyAnimation *const propertyAnimation = new QPropertyAnimation(horizontalSeparate, "geometry", Q_NULLPTR);
        propertyAnimation->setStartValue(horizontalSeparate->GetEndAnimationRect());
        propertyAnimation->setEndValue(QRect(horizontalSeparate->geometry().center(), QSize(1, 1)));
        propertyAnimation->setEasingCurve(m_rowAnimationCurve);
        propertyAnimation->setDuration(m_showAnimateDurationInMiliseconds);
        allButtonsAnimation->addAnimation(propertyAnimation);
    }
    return allButtonsAnimation;
}
