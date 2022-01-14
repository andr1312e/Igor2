#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPainterPath>
#include "traymenu.h"

TrayMenu::TrayMenu(QWidget *parent)
    : QMenu(parent)
    , m_buttonsAnimatingCurve(QEasingCurve(QEasingCurve::OutBack))
    , m_rowAnimationCurve(QEasingCurve::OutQuad)
{
    OnUpdateViewColors();
    setObjectName("TrayMenu");
    setFocusPolicy(Qt::StrongFocus); // Получаем фокус, разрешаем клики по кнопкам
    setWindowFlag(Qt::Popup, true);
    setAttribute(Qt::WA_StyledBackground);
    m_mainHorizontalLayout = new QHBoxLayout(this);
    m_mainVerticalLayout = new QVBoxLayout();
    setLayout(m_mainHorizontalLayout);
    m_mainHorizontalLayout->addLayout(m_mainVerticalLayout);
    m_mainHorizontalLayout->setMargin(0);
    m_mainHorizontalLayout->setSpacing(0);

    setStyleSheet("#TrayMenu { background: "+QVariant(m_normalBackGroud).toString()+"; border: none; border-radius:5px; }");

    setMouseTracking(true);

    // получить размер окна
    m_trayMenuRect = QGuiApplication::primaryScreen()->availableGeometry();
    m_trayMenuHeight = m_trayMenuRect.height();
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
    TrayMenuItem* addedActionMenuItem = AddAction(action->icon(), action->text(), [=]{ action->trigger(); });
    addedActionMenuItem->tooltip(action->toolTip());
}

TrayMenu *TrayMenu::BeginRow()
{
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    m_rowHorizontalLayoutsList.append(horizontalLayout);
    m_mainVerticalLayout->addLayout(horizontalLayout);

    m_manyItemToRowMode = true;
    m_addingHorizontMode = true;

    return this;
}

TrayMenu *TrayMenu::EndRow()
{
    m_manyItemToRowMode = false;
    m_addingHorizontMode = false;

    return this;
}

void TrayMenu::AddTextToMenu(const QString &text, int split)
{
    if (split < 0)
    {
        this->split();
        if (layout()->count())
            this->AddSpacing(4); // Поскольку он отделен от предыдущего пункта меню, он не может быть слишком компактным, и вам нужно добавить немного места.
    }
    QLabel* label = new QLabel(text, this);
    label->setStyleSheet("margin: 4px;");
    InstertLabelIntoMenu(label);
    if (split > 0)
        this->split();
}

TrayMenuItem *TrayMenu::AddAction(const QIcon &icon,const QString& text, FuncType clicked)
{
    TrayMenuItem* newItem = CreateMenuItem(icon, text);
    connect(newItem, &QAbstractButton::clicked, this, [=]{
        if (m_startShowingAnimationMode)
            return;
        if (clicked != Q_NULLPTR)
            clicked();
        if (newItem->isLinger())
            return ;
        toHide(std::find(m_trayMenuItems.begin(), m_trayMenuItems.end(), newItem));
    });
    connect(newItem, &InteractiveButtonBase::signalMouseEnter, this, [=]{ OnItemMouseEntered(newItem); });
    return newItem;
}

/**
  * Получить текущий макет
  * Если он выкладывается горизонтально, вернуть макет (дочерний элемент) горизонтального макета
  * в противном случае вернуть общий макет вертикального макета
  */
QBoxLayout *TrayMenu::GetCurrentLayout() const
{
    if (m_addingHorizontMode && m_rowHorizontalLayoutsList.size())
        return m_rowHorizontalLayoutsList.last();
    else
        return m_mainVerticalLayout;
}

TrayMenu *TrayMenu::InstertLabelIntoMenu(QLabel *labelWidget, int stretch, Qt::Alignment alignment)
{
    if (Q_NULLPTR==labelWidget)
        return this;
    QBoxLayout* layoutToInsert = Q_NULLPTR;
    if (m_addingHorizontMode && !m_rowHorizontalLayoutsList.isEmpty()) // Если он добавляет горизонтальную кнопку
    {
        layoutToInsert = m_rowHorizontalLayoutsList.last();
    }
    else
    {
        layoutToInsert = m_mainVerticalLayout;
    }
    layoutToInsert->addWidget(labelWidget, stretch, alignment);
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

TrayMenu *TrayMenu::split()
{
    AddSeparatorLineHorizontal();
    return this;
}

bool TrayMenu::HasFocus() const
{
    return QWidget::hasFocus();
}

/**
 * 返回菜单项的索引
 * 不包括非菜单项的自定义控件、布局、分割线等
 * 未找到返回-1
 */
std::vector<TrayMenuItem*>::iterator TrayMenu::indexOf(TrayMenuItem *item)
{
    return std::find(m_trayMenuItems.begin(), m_trayMenuItems.end(), item);
}

/**
 * 返回索引对应的菜单项
 * 不包括非菜单项的自定义控件、布局、分割线等
 * 未找到返回Q_NULLPTR
 */
TrayMenuItem *TrayMenu::at(int index) const
{
    if (index < 0 || index >= m_trayMenuItems.size())
        return Q_NULLPTR;
    return m_trayMenuItems.at(index);
}

/**
 * 一行有多个按钮时的竖向分割线
 * 只有添加chip前有效
 */
TrayMenuItem *TrayMenu::AddVerticalSeparator()
{
    TrayMenuItem* item = new TrayMenuItem(this);
    //    item->setNormalColor(QColor(64, 64, 64, 64));
    //    item->setFixedWidth(1);
    //    item->setPaddings(0, 0, 0, 0);
    //    item->setDisabled(true);

    //    m_rowHorizontalLayoutsList.last()->addWidget(item);
    //    v_separators.append(item);

    return item;
}

/**
 * 在鼠标或指定点展开
 * 自动避开屏幕边缘
 */
void TrayMenu::exec(QPoint pos)
{
    if (pos == QPoint(-1,-1))
        pos = QCursor::pos();
    QPoint originPos = pos; // 不包含像素偏移的原始点
    m_mainVerticalLayout->setEnabled(true);
    m_mainVerticalLayout->activate(); // 先调整所有控件大小
    this->adjustSize();

    // setAttribute(Qt::WA_DontShowOnScreen); // 会触发 setMouseGrabEnabled 错误
    // show();
    // hide(); // 直接显示吧
    // setAttribute(Qt::WA_DontShowOnScreen, false);

    int x = pos.x() + 1;
    int y = pos.y() + 1;
    int w = width() + 1;
    int h = height() + 1;
    QRect avai = m_trayMenuRect; // 屏幕大小

    // 如果超过范围，则调整位置
    if (x + w > avai.right())
        x = avai.right() - w;
    if (y + h > avai.bottom())
        y = avai.bottom() - h;
    if (x >= w && pos.x() + w > avai.right())
        x = originPos.x() - w;
    if (y >= h && pos.y() + h > avai.bottom())
        y = originPos.y() - h;

    // 移动窗口
    move(QPoint(x, y));

    execute();
}

/**
 * 确保在指定矩形框之外展开
 * 注意：即使菜单项超过了窗口范围，也不会覆盖这个矩形
 * @param expt     这个是相对整个屏幕的坐标，用 mapToGlobal(geometry())
 * @param vertical 优先左边对齐（出现在下边），还是顶部对齐（出现在右边）
 * @param pos      默认位置。如果在expt外且非边缘，则不受expt影响
 */
void TrayMenu::exec(QRect expt, bool vertical, QPoint pos)
{
    if (pos == QPoint(-1,-1))
        pos = QCursor::pos();
    m_mainVerticalLayout->setEnabled(true);
    m_mainHorizontalLayout->invalidate();
    m_mainVerticalLayout->activate(); // 先调整所有控件大小

    // setAttribute(Qt::WA_DontShowOnScreen); // 会触发 setMouseGrabEnabled 错误
    // show(); // 但直接显示会有一瞬间闪烁情况
    // hide();
    // setAttribute(Qt::WA_DontShowOnScreen, false);

    // 根据 rect 和 avai 自动调整范围
    QRect avai = m_trayMenuRect;
    QRect rect = geometry();
    rect.moveTo(pos);
    if (!vertical) // 优先横向对齐（顶上）
    {
        if (rect.left() <= expt.right() && rect.right() > expt.right())
            rect.moveLeft(expt.right());
        rect.moveTop(expt.top());

        // 避开屏幕位置
        if (expt.left() > rect.width() && rect.right() >= avai.right())
            rect.moveLeft(expt.left() - rect.width());
        if (rect.bottom() >= avai.bottom())
        {
            if (expt.top() > rect.height())
                rect.moveTop(expt.bottom() - rect.height());
            else
                rect.moveTop(avai.bottom() - rect.height());
        }
    }
    else // 优先纵向对齐（左下）
    {
        if (rect.top() <= expt.bottom() && rect.bottom() > expt.bottom())
            rect.moveTop(expt.bottom());
        rect.moveLeft(expt.left());

        // 避开屏幕位置
        if (rect.right() >= avai.right())
        {
            if (expt.left() > rect.width())
                rect.moveLeft(expt.right() - rect.width());
            else
                rect.moveLeft(avai.right() - rect.width());
        }
        if (expt.top() > rect.height() && rect.bottom() >= avai.bottom())
            rect.moveTop(expt.top() - rect.height());
    }

    // 移动窗口
    move(rect.topLeft());

    execute();
}

void TrayMenu::execute()
{
    m_currentElementIndex = m_trayMenuItems.end();

    // 设置背景为圆角矩形
    if (height() > 0 && m_borderRadius) // 没有菜单项的时候为0
    {
        QPixmap pixmap(width(), height());
        pixmap.fill(Qt::transparent);
        QPainter pix_ptr(&pixmap);
        pix_ptr.setRenderHint(QPainter::Antialiasing, true);
        QPainterPath path;
        path.addRoundedRect(0, 0, width(), height(), m_borderRadius, m_borderRadius);
        pix_ptr.fillPath(path, Qt::white);
        setMask(pixmap.mask());
    }

    // 是否捕获背景模糊图片
    if (m_blurAlphaValue > 0)
    {
        // 获取图片
        QRect rect = this->geometry();
        int radius = qMin(64, qMin(width(), height())); // 模糊半径，也是边界
        rect.adjust(-radius, -radius, +radius, +radius);
        QScreen* screen = QApplication::screenAt(QCursor::pos());
        if (screen)
        {
            QPixmap bg = screen->grabWindow(QApplication::desktop()->winId(), rect.left(), rect.top(), rect.width(), rect.height());

            // 开始模糊
            QT_BEGIN_NAMESPACE
            extern Q_WIDGETS_EXPORT void qt_blurImage( QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
            QT_END_NAMESPACE

            QPixmap pixmap = bg;
            QPainter painter( &pixmap );
            // 填充半透明的背景颜色，避免太透
            {
                QColor bg_c(m_normalBackGroud);
                bg_c.setAlpha(m_normalBackGroud.alpha() * (100 - m_blurAlphaValue) / 100);
                painter.fillRect(0, 0, pixmap.width(), pixmap.height(), bg_c);
            }
            QImage img = pixmap.toImage(); // img -blur-> painter(pixmap)
            qt_blurImage( &painter, img, radius, true, false );
            // 裁剪掉边缘（模糊后会有黑边）
            int c = qMin(bg.width(), bg.height());
            c = qMin(c/2, radius);
            m_backgroundPixmap = pixmap.copy(c, c, pixmap.width()-c*2, pixmap.height()-c*2);
        }
    }

    // 有些重复显示的，需要再初始化一遍
    hidden_by_another = false;
    using_keyboard = false;
    closed_by_clicked = false;

    // 显示动画
    QWidget::show();
    setFocus();
    StartButtonAnimationOnShowedMainRectange();
}

/**
 * 逐级隐藏菜单，并带有选中项动画
 */
void TrayMenu::toHide(std::vector<TrayMenuItem*>::iterator focusIndex)
{
    // Рекурсивно очистить меню (включая родительское меню) фокус
    // При закрытии фокус будет передаваться слой за слоем
    // Хотя в целом это не большая проблема, но есть серьезная проблема при переименовании (нельзя потерять фокус)
    TrayMenu* menu = this;
    do {
        menu->setFocusPolicy(Qt::NoFocus);
        menu = menu->m_parentMenu;
    } while (menu);
    this->clearFocus();

    closed_by_clicked = true;
//    startAnimationOnHidden(focusIndex);
}

void TrayMenu::toClose()
{
    if (m_parentMenu)
        m_parentMenu->toClose();
    else
        this->close();
}

TrayMenu *TrayMenu::setAppearAnimation(bool en)
{
    this->m_buttonAnimationIsEnabled = en;
    foreach (auto item, m_trayMenuItems)
        item->subMenu() && item->subMenu()->setAppearAnimation(en);
    return this;
}

TrayMenu *TrayMenu::setDisappearAnimation(bool en)
{
    this->m_enableDisappearAnimation = en;
    foreach (auto item, m_trayMenuItems)
        item->subMenu() && item->subMenu()->setDisappearAnimation(en);
    return this;
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
        return ;
    std::vector<TrayMenuItem*>::iterator currentItemIterator= std::find(m_trayMenuItems.begin(), m_trayMenuItems.end(), item);
    if (currentItemIterator!=m_trayMenuItems.end() && m_currentElementIndex != currentItemIterator) // 屏蔽键盘操作
        m_currentElementIndex.operator*()->discardHoverPress(true);
    m_currentElementIndex = currentItemIterator;
}

/**
 * Все способы создания пунктов меню
 */
TrayMenuItem *TrayMenu::CreateMenuItem(QIcon icon, QString text)
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

    setActionButton(itemToAdd, m_addingHorizontMode);

    if (m_addingHorizontMode && !m_rowHorizontalLayoutsList.isEmpty()) // Если он добавляет горизонтальную кнопку
    {
        //        if (split_in_row && m_rowHorizontalLayoutsList.last()->count() > 0)
        //            AddVerticalSeparator(); // Добавить вертикальный разделитель
        m_rowHorizontalLayoutsList.last()->addWidget(itemToAdd);
    }
    else
    {
        m_mainVerticalLayout->addWidget(itemToAdd);
    }
    m_trayMenuItems.push_back(itemToAdd);
    return itemToAdd;
}

/**
 * Установите кнопку действия одинаково (размер, цвет и т. д.)
 * @param btn     кнопка
 * @param isChip  Будь то маленькая кнопка (более одной строки, без пробела справа)
 */
void TrayMenu::setActionButton(InteractiveButtonBase *btn, bool isChip)
{
    // 设置尺寸
    if (isChip)
    {
        btn->setPaddings(item_padding);
    }
    else
    {
        btn->setPaddings(item_padding, addin_tip_area > 0 ? tip_area_spacing + addin_tip_area : item_padding, item_padding, item_padding);
    }

    // 设置颜色
    btn->setNormalColor(Qt::transparent);
    btn->setHoverColor(m_hoverBackgroud);
    btn->setPressColor(m_pressBackgroud);
    btn->setTextColor(m_textColor);

    QFont font(btn->font());
    font.setWeight(QFont::Medium);
    btn->setFont(font);
}

/**
 * 根据 mouseMove 事件，判断鼠标的位置
 * 如果是在父菜单另一个item，则隐藏子菜单（接着触发关闭信号）
 * 如果是在父菜单的父菜单，则关闭子菜单后，父菜单也依次关闭
 * @param pos   鼠标位置（全局）
 * @param child 子菜单
 * @return      是否在父或递归父菜单的 geometry 中
 */
bool TrayMenu::isCursorInArea(QPoint pos, TrayMenu *child)
{
    // 不在这范围内
    if (!geometry().contains(pos))
    {
        // 在自己的父菜单那里
        if ( m_parentMenu->isCursorInArea(pos, this)) // 如果这也是子菜单（已展开），则递归遍历父菜单
        {
            hidden_by_another = true;
            QTimer::singleShot(0, this, [=]{
                close(); // 把自己也隐藏了
            });
            return true;
        }
        return false;
    }
    // 如果是正展开的这个子项按钮
    if (m_currentElementIndex!=m_trayMenuItems.end() && child && m_currentElementIndex.operator*()->subMenu() == child && m_currentElementIndex.operator*()->geometry().contains(mapFromGlobal(pos)))
        return false;
    // 在这个菜单内的其他按钮
    return true;
}

/**
 * В меню появляется анимация
 * последовательно от позиции курсора
 */
void TrayMenu::StartButtonAnimationOnShowedMainRectange()
{
    /**
     * Не заворачивай анимацию в умные указатели!!!!
     * Перед любой анимацией делай виджеты недоступными!!
     */
    if (m_buttonAnimationIsEnabled)
    {
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
            propertyActionAnimation->setDuration(m_durationToAnimateElements);
            connect(&(*propertyActionAnimation), &QPropertyAnimation::finished, item, [=]{
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
            propertyLabelAnimation->setDuration(m_durationToAnimateElements);
            connect(propertyLabelAnimation, &QPropertyAnimation::finished, propertyLabelAnimation, &QObject::deleteLater);
            propertyLabelAnimation->start();
        }

        // скрыть строки разделители и потом анимировать
        for (TrayMenuItem *horizontalSeparate: m_horizontalSeparatorsList)
        {
            horizontalSeparate->hide();
        }

        QTimer::singleShot(m_durationToAnimateElements, this, [=]{
            for (InteractiveButtonBase* interactiveButton: m_horizontalSeparatorsList)
            {
                interactiveButton->show();
                interactiveButton->setMinimumSize(0, 0);
                QPropertyAnimation *propertyAnimation = new QPropertyAnimation(interactiveButton, "geometry", Q_NULLPTR);
                propertyAnimation->setStartValue(QRect(interactiveButton->geometry().center(), QSize(1,1)));
                propertyAnimation->setEndValue(interactiveButton->geometry());
                propertyAnimation->setEasingCurve(m_rowAnimationCurve);
                propertyAnimation->setDuration(m_durationToAnimateElements);
                connect(propertyAnimation, &QPropertyAnimation::finished, propertyAnimation, &QObject::deleteLater);
                propertyAnimation->start();
            }
        });
        //включаем кнопки и режим анимации выключаем после анимации кнопок
        QTimer::singleShot(m_durationToAnimateElements, this, [=]{
            m_mainVerticalLayout->setEnabled(true);
            m_startShowingAnimationMode = false;
        });
    }
    else
    {
        return ;
    }
}

/**
 * 关闭前显示隐藏动画
 * @param focusIndex 聚焦的item，如果不存在则为-1
 */
void TrayMenu::startAnimationOnHidden(int focusIndex)
{
    if (!m_enableDisappearAnimation)
    {
        if (focusIndex > -1)
        {
            // Подождите, пока закончится анимация щелчка.
            QTimer::singleShot(100, [=]{
                close();
            });
        }
        else
        {
            close();
        }
        return ;
    }

    m_startShowingAnimationMode = true;
    // Управление анимацией движения
    m_mainVerticalLayout->setEnabled(false);
    int dur_min =100, dur_max = 200;
    int up_flow_count = focusIndex > -1 ? std::max(focusIndex, (int)m_trayMenuItems.size()-focusIndex-1) : -1;
    int up_end = m_trayMenuItems.size() ? -m_trayMenuItems.at(0)->height() : 0;
    int flow_end = height();
    int focus_top = focusIndex > -1 ? m_trayMenuItems.at(focusIndex)->pos().y() : 0;
    for (int i = 0; i < m_trayMenuItems.size(); i++)
    {
        InteractiveButtonBase* btn = m_trayMenuItems.at(i);
        // QPoint pos = btn->pos();
        btn->setBlockHover(true);
        QPropertyAnimation* ani = new QPropertyAnimation(btn, "pos");
        ani->setStartValue(btn->pos());
        ani->setEasingCurve(QEasingCurve::OutCubic);
        if (focusIndex > -1)
        {
            if (i < focusIndex) // 上面的项
            {
                ani->setEndValue(QPoint(0, up_end - (focus_top - btn->pos().y()) / 8));
                ani->setDuration(dur_max - qAbs(focusIndex-i)*(dur_max-dur_min)/up_flow_count);
            }
            else if (i == focusIndex) // 中间的项
            {
                ani->setEndValue(btn->pos());
                ani->setDuration(dur_max);
            }
            else // 下面的项
            {
                ani->setEndValue(QPoint(0, flow_end + (btn->pos().y()-focus_top) / 8));
                ani->setDuration(dur_max - qAbs(i-focusIndex)*(dur_max-dur_min)/up_flow_count);
            }
        }
        else
        {
            ani->setEndValue(QPoint(0, up_end));
            ani->setDuration(dur_max);
        }
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        connect(ani, &QPropertyAnimation::finished, btn, [=]{
            btn->setBlockHover(false);
            // btn->move(pos);
        });
        ani->start();
    }

    // 第三方控件动画
    for (int i = 0; i < m_menuLabels.size(); i++)
    {
        QWidget* btn = m_menuLabels.at(i);

        QPropertyAnimation* ani = new QPropertyAnimation(btn, "pos");
        ani->setStartValue(btn->pos());
        ani->setEasingCurve(QEasingCurve::OutCubic);
        if (focusIndex > -1)
        {
            if (i < focusIndex) // 上面的项
            {
                ani->setEndValue(QPoint(0, up_end - (focus_top - btn->pos().y()) / 8));
                ani->setDuration(dur_max - qAbs(focusIndex-i)*(dur_max-dur_min)/up_flow_count);
            }
            else if (i == focusIndex) // 中间的项
            {
                ani->setEndValue(btn->pos());
                ani->setDuration(dur_max);
            }
            else // 下面的项
            {
                ani->setEndValue(QPoint(0, flow_end + (btn->pos().y()-focus_top) / 8));
                ani->setDuration(dur_max - qAbs(i-focusIndex)*(dur_max-dur_min)/up_flow_count);
            }
        }
        else
        {
            ani->setEndValue(QPoint(0, up_end));
            ani->setDuration(dur_max);
        }
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        // connect(ani, &QPropertyAnimation::finished, btn, [=]{
        //     btn->move(pos);
        // });
        ani->start();
    }

    // 变淡动画（针对Popup，一切透明无效）
    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(1);
    setGraphicsEffect(effect);
    QPropertyAnimation* opa_ani = new QPropertyAnimation(effect, "opacity");
    opa_ani->setDuration(dur_max * 0.8);
    opa_ani->setStartValue(1);
    opa_ani->setEndValue(0);
    connect(opa_ani, &QPropertyAnimation::finished, this, [=]{
        opa_ani->deleteLater();
        effect->deleteLater();
    });
    opa_ani->start();

    // 真正关闭
    QTimer::singleShot(dur_max, this, [=]{
        m_startShowingAnimationMode = false;
        // 挨个还原之前的位置（不知道为什么main_vlayout不能恢复了）
        m_mainVerticalLayout->setEnabled(true);
        m_mainVerticalLayout->activate(); // 恢复原来的位置

        close();
    });
}

void TrayMenu::showEvent(QShowEvent *event)
{
    //    m_mainVerticalLayout->setEnabled(true);
    //    m_mainHorizontalLayout->invalidate();
    //    m_mainVerticalLayout->activate(); //активации небудет, если кеш не очищен

    // Автоматически регулировать диапазон на основе высоты и ширины
    QRect widgetRect = geometry();
    widgetRect.moveTo(QPoint(1750, 550));

    QRect expt=QRect(QPoint(1750, 750), QSize(150, 200));
    if (widgetRect.left() <= expt.right() && widgetRect.right() > expt.right())
        widgetRect.moveLeft(expt.right());
    widgetRect.moveTop(expt.top());

    // Избегайте положения вне экрана
    if (expt.left() > widgetRect.width() && widgetRect.right() >= m_trayMenuRect.right())
        widgetRect.moveLeft(expt.left() - widgetRect.width());
    if (widgetRect.bottom() >= m_trayMenuRect.bottom())
    {
        if (expt.top() > widgetRect.height())
            widgetRect.moveTop(expt.bottom() - widgetRect.height());
        else
            widgetRect.moveTop(m_trayMenuRect.bottom() - widgetRect.height());
    }

    // переместить окно
    move(widgetRect.topLeft());



    m_currentElementIndex =m_trayMenuItems.end();

    // Установите фон в прямоугольник со скругленными углами
    QPixmap pixmap(size());
    pixmap.fill(Qt::transparent);
    QPainter pix_ptr(&pixmap);
    pix_ptr.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), m_borderRadius, m_borderRadius);
    pix_ptr.fillPath(path, Qt::white);
    setMask(pixmap.mask());

    // Захватывать ли изображения с размытым фоном
    // получаем изображение
    widgetRect = geometry();
    int radius = qMin(64, qMin(width(), height())); // Радиус размытия, а также граница
    widgetRect.adjust(-radius, -radius, +radius, +radius);
    QScreen* screen = QApplication::screenAt(QCursor::pos());
    if (screen)
    {
        QPixmap backGroundImage = screen->grabWindow(QApplication::desktop()->winId(), widgetRect.left(), widgetRect.top(), widgetRect.width(), widgetRect.height());

        // начать размывать
        QT_BEGIN_NAMESPACE
        extern Q_WIDGETS_EXPORT void qt_blurImage( QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
        QT_END_NAMESPACE

        QPixmap pixmap = backGroundImage;
        QPainter painter( &pixmap );
        // Заполните полупрозрачным цветом фона, чтобы не быть слишком прозрачным.
        {
            QColor backGroundColor(m_normalBackGroud);
            backGroundColor.setAlpha(m_normalBackGroud.alpha() * (100 - m_blurAlphaValue) / 100);
            painter.fillRect(0, 0, pixmap.width(), pixmap.height(), backGroundColor);
        }
        QImage img = pixmap.toImage(); // img -blur-> painter(pixmap)
        qt_blurImage( &painter, img, radius, true, false );
        // Обрежьте края (после размытия будут черные границы)
        int c = qMin(backGroundImage.width(), backGroundImage.height());
        c = qMin(c/2, radius);
        m_backgroundPixmap = pixmap.copy(c, c, pixmap.width()-c*2, pixmap.height()-c*2);
    }

    // Некоторые отображаются повторно и требуют повторной инициализации.
    hidden_by_another = false;
    using_keyboard = false;
    closed_by_clicked = false;

    // устанавливаем фокус на этот винжет
    setFocus();
    // показать анимацию
    StartButtonAnimationOnShowedMainRectange();

    QWidget::showEvent(event);

    m_mainVerticalLayout->setEnabled(true);
    m_mainVerticalLayout->invalidate(); // активация будет ложной, если кеш не очищен
    m_mainVerticalLayout->activate();
}

void TrayMenu::hideEvent(QHideEvent *event)
{
    this->clearFocus();
    this->close(); // Подменю закрыто, само  меню себя не закроет, нужно закрывать вручную
    return QWidget::hideEvent(event);
}

void TrayMenu::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    QPoint pos = QCursor::pos();
    if (m_parentMenu && m_parentMenu->isCursorInArea(pos, this)) // В родительском классе скрыть сам по себе
    {
        this->hide();
        m_parentMenu->setFocus();
    }

    if (using_keyboard)
    {
        using_keyboard = false;
        if (m_currentElementIndex!= m_trayMenuItems.end())
        {
            (*m_currentElementIndex)->discardHoverPress(false);
        }
    }
}

void TrayMenu::keyPressEvent(QKeyEvent *event)
{
    return QWidget::keyPressEvent(event);
}

void TrayMenu::paintEvent(QPaintEvent *event)
{
    if (!m_backgroundPixmap.isNull())
    {
        QPainter painter(this);
        painter.drawPixmap(QRect(0,0,width(),height()), m_backgroundPixmap);
        return ;
    }
    QWidget::paintEvent(event);
}


