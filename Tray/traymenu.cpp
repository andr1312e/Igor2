#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent)
    : QMenu(parent)
    , m_buttonsAnimatingCurve(QEasingCurve::OutBack)
    , m_rowAnimationCurve(QEasingCurve::OutQuad)
    , m_backgroundImage(QWidget::size(), QImage::Format_ARGB32_Premultiplied)
    , m_availableGeometry(QGuiApplication::primaryScreen()->availableGeometry())
{
    setObjectName(QStringLiteral("TrayMenu"));
    setWindowFlag(Qt::Popup, true);
    setAttribute(Qt::WA_StyledBackground);
    m_mainHorizontalLayout = new QHBoxLayout(this);
    m_mainVerticalLayout = new QVBoxLayout();
    m_mainHorizontalLayout->addLayout(m_mainVerticalLayout);
    m_mainHorizontalLayout->setMargin(0);
    m_mainHorizontalLayout->setSpacing(0);

    setStyleSheet(QStringLiteral("#TrayMenu {; border: none; border-radius:5px; }"));
}

TrayMenu::~TrayMenu()
{

}

void TrayMenu::AddButtonToMenu(QAction *action)
{
    TrayMenuItem* const addedActionMenuItem = CreateMenuItem(action);
    addedActionMenuItem->SetTooltipText(action->toolTip());
    connect(addedActionMenuItem, &QPushButton::clicked, action, &QAction::trigger);
    connect(addedActionMenuItem, &QPushButton::clicked, this, [=](){ButtonClicked(addedActionMenuItem);});
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
    QLabel* const label = new QLabel(text);
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
        return m_rowHorizontalLayoutsList.back();
    else
        return m_mainVerticalLayout;
}

void TrayMenu::InstertLabelIntoMenu(QLabel *labelWidget)
{
    if (Q_NULLPTR!=labelWidget)
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
    TrayMenuItem*const item = new TrayMenuItem(this);
    item->setFixedHeight(1);
    item->setPaddings(32, 32, 0, 0);
    item->setDisabled(true);

    m_mainVerticalLayout->addWidget(item);
    m_horizontalSeparatorsList.push_back(item);
}

//bool TrayMenu::event(QEvent *event)
//{
//        Log4Qt::Logger::rootLogger()->info("showEvent" + QString::number(event->type()));
//    return false;
//}

void TrayMenu::OnUpdateViewColors()
{
    const QPalette palette=QApplication::palette();
    const QColor newBackGroundColor=palette.color(QPalette::Window);
    const QColor newHoverBackgroudColor=palette.color(QPalette::Highlight);  // при наведении фон
    const QColor newTextColor=palette.color(QPalette::WindowText);
    m_normalBackGroud = newBackGroundColor;
    MakeTransparentImage();
    for (TrayMenuItem *horizontalSeparate: m_horizontalSeparatorsList)
    {
        horizontalSeparate->SetNormalColor(newTextColor);
    }
    for (TrayMenuItem *item:m_trayMenuItems)
    {
         item->SetNormalColor(newBackGroundColor);
         item->SetHoverColor(newHoverBackgroudColor);
         item->SetTextColor(newTextColor);
    }
}

void TrayMenu::ButtonClicked(TrayMenuItem *item)
{
    for (const QHBoxLayout* rowLayout: qAsConst(m_rowHorizontalLayoutsList))
    {

        bool finden=false;
        for (int i=0; i<rowLayout->count(); ++i)
        {
            TrayMenuItem* itemAtLayout=static_cast<TrayMenuItem*>(rowLayout->itemAt(i)->widget());
            if(itemAtLayout==item)
            {
                finden=true;
                break;
            }
        }
        if(finden)
        {
            for (int i=0; i<rowLayout->count(); ++i)
            {
                TrayMenuItem* const itemInLayout=static_cast<TrayMenuItem*>(rowLayout->itemAt(i)->widget());
                if(itemInLayout==item)
                {
                    QString currentActiveIconPath=itemInLayout->GetIconPath();
                    if (currentActiveIconPath.indexOf(QStringLiteral("refr"))==-1)
                    {
                        if (currentActiveIconPath.endsWith(m_notCheckedImage))
                        {
                            currentActiveIconPath.remove(currentActiveIconPath.lastIndexOf('_')+1, 3);
                        }
                        itemInLayout->SetIconPath(currentActiveIconPath);
                    }
                    else
                    {
                        TrayMenuItem* const playButton=static_cast<TrayMenuItem*>(rowLayout->itemAt(i+1)->widget());
                        if (Q_NULLPTR!=playButton)
                        {
                            QString playButtonIconPath=playButton->GetIconPath();
                            if (playButtonIconPath.endsWith(m_notCheckedImage))
                            {
                                playButtonIconPath.remove(playButtonIconPath.lastIndexOf('_')+1, 3);
                            }
                            playButton->SetIconPath(playButtonIconPath);
                            return;
                        }
                        else
                        {
                            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Последняя кнопка должна быть включить контроль перезапуска")).toUtf8().constData());
                        }
                    }
                }
                else
                {
                    QString iconPath=itemInLayout->GetIconPath();
                    if(iconPath.endsWith(m_checkedImage))
                    {
                        iconPath.insert(iconPath.lastIndexOf('_')+1, QStringLiteral("not"));
                        itemInLayout->SetIconPath(iconPath);
                    }
                }
            }
            return;
        }
    }

}

/**
 * Все способы создания пунктов меню
 */
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

void TrayMenu::CalculateWidgetRectOnScreen()
{
    // Автоматически регулировать диапазон на основе высоты и ширины
    const int widgetHeight=QWidget::height();
    const int widgetWidht=QWidget::width();

    const QScreen* const screen = QGuiApplication::primaryScreen();
    const QRect screenGeometry = screen->geometry();
    const int heightScreen = screenGeometry.height();
    const int widthScreen = screenGeometry.width();

    const QPoint topLeftPosition=QPoint(widthScreen-widgetWidht-100, heightScreen-widgetHeight-55);

    // переместить окно
    QWidget::move(topLeftPosition);
    Log4Qt::Logger::rootLogger()->info(QString("topLeftPosition" + QString::number(topLeftPosition.x()) + "  " + QString::number(topLeftPosition.y())));
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
    path.addRoundedRect(0, 0, QWidget::width(), QWidget::height(), m_borderRadius, m_borderRadius);//заполняем белым все кроме углов, они прозрачные
    pixmapPainter.fillPath(path, Qt::white);
    QWidget::setMask(pixmap.mask());
}

void TrayMenu::MakeTransparentImage()
{
    QColor newBackgroundColor(m_normalBackGroud);
    qDebug()<< Q_FUNC_INFO << "   " << newBackgroundColor.alpha();
    newBackgroundColor.setAlpha(m_alphaValue);
    m_backgroundImage.fill(newBackgroundColor);
}

/**
 * Записываем в m_backgroundPixmap готовый QPixmap с альфа каналом размытия
 */
void TrayMenu::ChangeBackgroundImageAplha(int &alpha)
{
    if(!m_backgroundImage.isNull())
    {
        for (int y = 0; y < m_backgroundImage.height(); ++y)
        {
            QRgb *row = reinterpret_cast<QRgb*>(m_backgroundImage.scanLine(y));
            for (int x = 0; x < m_backgroundImage.width(); ++x) {
                if(y==4 && x%10==0)
                {
                    qDebug ()  << "aplha Before " << ((unsigned char*)&row[x])[3] << " and new " << alpha;
                }
                ((unsigned char*)&row[x])[3] = alpha;
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
void TrayMenu::ShowAnimationPlay()
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
    AnimateDisclosureOfAllButtons(startPosition)->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(m_showAnimateDurationInMiliseconds, this, [&]{
        AnimateDisclosureHorizontalSeparates();
    });

    //включаем кнопки и режим анимации выключаем после анимации кнопок
    QTimer::singleShot(m_showAnimateDurationInMiliseconds*2, this, [&]{
        m_mainVerticalLayout->setEnabled(true);
        m_isAnimationMode = false;

    });
}

/**
 * HiddenButtonAnimationStart скрываем кнопки при закрытии
 */
void TrayMenu::HiddenAnimationPlay()
{
    m_isAnimationMode = true;
    m_mainVerticalLayout->setEnabled(false);

    HideHorizontalSeparates();
    const QPoint startPosition=CalculateStartAndEndAnimationPostionFromUserMousePos();
    AnimateFoldingOfAllButtons(startPosition)->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(m_hideAnimateDurationInMiliseconds, this, [&]
    {
        AnimateFoldingHorizontalSeparates();
    });

    QTimer::singleShot(m_hideAnimateDurationInMiliseconds*2, this, [&]{
        m_isAnimationMode = false;
    });
}

void TrayMenu::AnimateDisclosureHorizontalSeparates()
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

QParallelAnimationGroup* TrayMenu::AnimateDisclosureOfAllButtons(const QPoint &startPosition)
{
    QParallelAnimationGroup *const allButtonsAnimation = new QParallelAnimationGroup(Q_NULLPTR);
    for (InteractiveButtonBase *item : m_trayMenuItems)
    {
        //        item->show();
        item->setBlockHover(true);
        QPropertyAnimation* const propertyActionAnimation = new QPropertyAnimation(item, "pos", Q_NULLPTR);
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
        //        label->show();
        QPropertyAnimation* const propertyLabelAnimation = new QPropertyAnimation(label, "pos", this);
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
        QPropertyAnimation* const propertyActionAnimation = new QPropertyAnimation(item, "pos", Q_NULLPTR);
        const QPoint position(QPoint(item->pos()));
        propertyActionAnimation->setStartValue(item->pos());
        propertyActionAnimation->setEndValue(startPosition);
        propertyActionAnimation->setEasingCurve(m_buttonsAnimatingCurve);
        propertyActionAnimation->setDuration(m_hideAnimateDurationInMiliseconds);
        connect(propertyActionAnimation, &QPropertyAnimation::finished, item, [position, item]{
            item->setBlockHover(false);
            item->move(position);
        });
        allButtonsAnimation->addAnimation(propertyActionAnimation);
    }
    for (QLabel *label: m_menuLabels)
    {
        QPropertyAnimation* const propertyLabelAnimation = new QPropertyAnimation(label, "pos", Q_NULLPTR);
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

void TrayMenu::HideHorizontalSeparates()
{
    for (InteractiveButtonBase* horizontalSeparate: m_horizontalSeparatorsList)
    {
        horizontalSeparate->hide();
    }
}

void TrayMenu::showEvent(QShowEvent *event)
{
    Log4Qt::Logger::rootLogger()->info("showEvent");
    if(m_isAnimationMode)
    {
        event->ignore();
    }
    else
    {
        CalculateWidgetRectOnScreen();//Получаем координаты виджета, вдруг юзер разрешение поменяет
        m_currentElementIndex =m_trayMenuItems.end();//Сбрасываем текущий индекс у кнопки
        SetRoundedFormToWidget();
        ShowAnimationPlay();
        QWidget::showEvent(event);
    }
}

void TrayMenu::closeEvent(QCloseEvent *event)
{
    Log4Qt::Logger::rootLogger()->info("showEvent");
    if (!m_isAnimationMode)
    {
        HiddenAnimationPlay();
        QTimer::singleShot(m_hideAnimateDurationInMiliseconds, this, &QWidget::hide);
        QMenu::closeEvent(event);
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
        QWidget::paintEvent(event);
    }
    else
    {
        QPainter painter(this);
        painter.drawImage(0,0, m_backgroundImage);
    }
}


