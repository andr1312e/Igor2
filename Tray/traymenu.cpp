#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPainterPath>
#include "traymenu.h"

QColor TrayMenu::m_normalBackGroud = QColor(255, 255, 255);
QColor TrayMenu::m_hoverBackgroud = QColor(128, 128, 128, 64);
QColor TrayMenu::m_pressBackgroud = QColor(128, 128, 128, 128);
QColor TrayMenu::m_textColor = QColor(0, 0, 0);

QEasingCurve TrayMenu::m_easingCurve = QEasingCurve::OutBack; // OutCubic 也不错

TrayMenu::TrayMenu(QWidget *parent)
    : QMenu(parent)
{
    setObjectName("TrayMenu");
    setFocusPolicy(Qt::StrongFocus); // Получаем фокус, разрешаем клики по кнопкам
    setWindowFlag(Qt::Popup, true);
    setAutoFillBackground(false);  //Если это не установлено, фон будет черным
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


TrayMenu::TrayMenu(bool, QWidget *parent)
    : TrayMenu(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setFocusPolicy(Qt::NoFocus);

    // Сохраните указатель родительского меню и определите положение движения мыши
    m_parentMenu = qobject_cast<TrayMenu*>(parent);
}

TrayMenu::~TrayMenu()
{
    if (finished_func)
    {
        (*finished_func)();
        delete  finished_func;
    }
    for(QAction *action: import_actions)
        action->deleteLater();
}

TrayMenuItem *TrayMenu::AddAction(const QIcon &icon,const QString& text, FuncType clicked)
{
    TrayMenuItem* item = CreateMenuItem(icon, text);
    connect(item, &InteractiveButtonBase::clicked, this, [=]{
        if (m_startShowingAnimationMode)
            return ;
        if (clicked != Q_NULLPTR)
            clicked();
        if (item->isLinger())
            return ;
        Q_EMIT ToTriggerAction(item);
        toHide(m_trayMenuItems.indexOf(item));
    });
    connect(item, &InteractiveButtonBase::signalMouseEnter, this, [=]{ OnItemMouseEntered(item); });
    return item;
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

    m_oneItemToRowMode = true;
    m_addingHorizontMode = true;

    return this;
}

TrayMenu *TrayMenu::EndRow()
{
    m_oneItemToRowMode = false;
    m_addingHorizontMode = false;

    return this;
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

void TrayMenu::AddTextToMenu(const QString &text, int split)
{
    if (split < 0)
    {
        this->split();
        if (layout()->count())
            this->addSpacing(4); // Поскольку он отделен от предыдущего пункта меню, он не может быть слишком компактным, и вам нужно добавить немного места.
    }
    QLabel* label = new QLabel(text, this);
    label->setStyleSheet("margin: 4px; color: gray;");
    InstertLabelIntoMenu(label);
    if (split > 0)
        this->split();
}

TrayMenu *TrayMenu::InstertLabelIntoMenu(QLabel *widget, int stretch, Qt::Alignment alignment)
{
    if (Q_NULLPTR==widget)
        return this;
    QBoxLayout* layoutToInsert = Q_NULLPTR;
    if (m_addingHorizontMode && !m_rowHorizontalLayoutsList.isEmpty()) // Если он добавляет горизонтальную кнопку
    {
        if (split_in_row && m_rowHorizontalLayoutsList.last()->count() > 0)
            AddVerticalSeparator(); // Добавить вертикальный разделитель
        layoutToInsert = m_rowHorizontalLayoutsList.last();
    }
    else
    {
        layoutToInsert = m_mainVerticalLayout;
    }
    layoutToInsert->addWidget(widget, stretch, alignment);
    m_addedLabels.append(widget);
    return this;
}

TrayMenu *TrayMenu::AddLayout(QLayout *layout, int stretch)
{
    if (!layout)
        return this;
    if (m_addingHorizontMode && m_rowHorizontalLayoutsList.size()) // 如果是正在添加横向按钮
    {
        if (split_in_row && m_rowHorizontalLayoutsList.last()->count() > 0)
            AddVerticalSeparator(); // 添加竖向分割线
        m_rowHorizontalLayoutsList.last()->addLayout(layout, stretch);
    }
    else
    {
        m_mainVerticalLayout->addLayout(layout, stretch);
    }
    for (int i = 0; i < layout->count(); i++)
    {
        auto it = layout->itemAt(i);
        auto widget = qobject_cast<QWidget*>(it->widget());
        if (widget) // 如果这个 LayoutItem 是 widget
            m_addedLabels.append(widget);
    }
    return this;
}

TrayMenu *TrayMenu::addLayoutItem(QLayoutItem* item)
{
    GetCurrentLayout()->addItem(item);
    return this;
}

TrayMenu *TrayMenu::addSpacerItem(QSpacerItem *spacerItem)
{
    GetCurrentLayout()->addSpacerItem(spacerItem);
    return this;
}

TrayMenu *TrayMenu::addSpacing(int size)
{
    GetCurrentLayout()->addSpacing(size);
    return this;
}

TrayMenu *TrayMenu::addStretch(int stretch)
{
    GetCurrentLayout()->addStretch(stretch);
    return this;
}

TrayMenu *TrayMenu::addStrut(int size)
{
    GetCurrentLayout()->addStrut(size);
    return this;
}

TrayMenu *TrayMenu::setSpacing(int spacing)
{
    GetCurrentLayout()->setSpacing(spacing);
    return this;
}

TrayMenu *TrayMenu::setStretchFactor(QWidget *widget, int stretch)
{
    m_mainVerticalLayout->setStretchFactor(widget, stretch);
    return this;
}

TrayMenu *TrayMenu::setStretchFactor(QLayout *layout, int stretch)
{
    m_mainVerticalLayout->setStretchFactor(layout, stretch);
    return this;
}

/**
 * 添加水平分割线
 * 不一定需要
 */
TrayMenuItem *TrayMenu::addSeparator()
{
    if (m_addingHorizontMode)
    {
        if (!m_rowHorizontalLayoutsList.last()->count())
            return Q_NULLPTR;
        return AddVerticalSeparator();
    }

    if (!m_mainVerticalLayout->count())
        return Q_NULLPTR;

    TrayMenuItem* item = new TrayMenuItem(this);
    item->setNormalColor(QColor(64, 64, 64, 64));
    item->setFixedHeight(1);
    item->setPaddings(32, 32, 0, 0);
    item->setDisabled(true);

    m_mainVerticalLayout->addWidget(item);
    m_horizontalSeparatorsList.append(item);

    return item;
}

TrayMenu *TrayMenu::split()
{
    addSeparator();
    return this;
}

bool TrayMenu::hasFocus() const
{
    if (QWidget::hasFocus())
        return true;
    if (current_sub_menu && current_sub_menu->hasFocus())
        return true;
    return false;
}

/**
 * 返回菜单项的索引
 * 不包括非菜单项的自定义控件、布局、分割线等
 * 未找到返回-1
 */
int TrayMenu::indexOf(TrayMenuItem *item)
{
    return m_trayMenuItems.indexOf(item);
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
    item->setNormalColor(QColor(64, 64, 64, 64));
    item->setFixedWidth(1);
    item->setPaddings(0, 0, 0, 0);
    item->setDisabled(true);

    m_rowHorizontalLayoutsList.last()->addWidget(item);
    v_separators.append(item);

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
    m_currentElementIndex = -1;

    // 设置背景为圆角矩形
    if (height() > 0 && border_radius) // 没有菜单项的时候为0
    {
        QPixmap pixmap(width(), height());
        pixmap.fill(Qt::transparent);
        QPainter pix_ptr(&pixmap);
        pix_ptr.setRenderHint(QPainter::Antialiasing, true);
        QPainterPath path;
        path.addRoundedRect(0, 0, width(), height(), border_radius, border_radius);
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
            m_backgorudPixmap = pixmap.copy(c, c, pixmap.width()-c*2, pixmap.height()-c*2);
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
void TrayMenu::toHide(int focusIndex)
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
    startAnimationOnHidden(focusIndex);
}

void TrayMenu::toClose()
{
    if (m_parentMenu)
        m_parentMenu->toClose();
    else
        this->close();
}

/**
 * 是因为点击了菜单项结束菜单
 * 还是因为其他原因，比如ESC关闭、鼠标点击其他位置呢
 */
bool TrayMenu::isClosedByClick() const
{
    return closed_by_clicked;
}

/**
 * 菜单结束的时候调用
 * 例如多选，确认多选后可调用此项
 */
TrayMenu* TrayMenu::finished(FuncType func)
{
    this->finished_func = new FuncType(func);
    return this;
}

/**
 * 添加可选菜单，快速添加多个单选项
 * @param texts  文字
 * @param states 选中状态
 * @param func   回调
 * @return
 */
TrayMenu *TrayMenu::addOptions(QList<QString> texts, QList<bool> states, FuncIntType clicked)
{
    //    int si = qMin(texts.size(), states.size());
    //    for (int i = 0; i < si; i++)
    //    {
    //        AddAction(texts.at(i), [=]{
    //            clicked(i);
    //        })->check(states.at(i));
    //    }

    return this;
}

TrayMenu *TrayMenu::addOptions(QList<QString> texts, int select, FuncIntType clicked)
{
    QList<bool>states;
    for (int i = 0; i < texts.size(); i++)
        states << false;

    if (select >= 0 && select < states.size())
        states[select] = true;

    return addOptions(texts, states, clicked);
}

/**
 * 用于单选
 * 取消除了某项之外全部选择
 */
TrayMenu *TrayMenu::singleCheck(TrayMenuItem *item)
{
    uncheckAll(item);
    return this;
}

/**
 * 取消所有 checkable 的项的check
 * @param except 如果不为空，则设置这一项为true（相当于单选）
 * @param begin 开始取消选择的项，默认-1，从头开始
 * @param end   结束取消选择的项（不包括此项），默认-1，直到末尾
 */
TrayMenu *TrayMenu::uncheckAll(TrayMenuItem *except, int begin, int end)
{
    if (begin < 0 || begin >= m_trayMenuItems.size())
        begin = 0;
    if (end < 0 || end > m_trayMenuItems.size())
        end = m_trayMenuItems.size();
    for (int i = begin; i < end; i++)
    {
        auto item = m_trayMenuItems.at(i);
        if (!item->isCheckable())
            continue;
        item->setChecked(false);
    }
    if (except)
        except->setChecked(true);
    return this;
}

/**
 * 返回选中的菜单
 */
QList<TrayMenuItem *> TrayMenu::checkedItems()
{
    QList<TrayMenuItem*> checkeds;
    foreach (auto item, m_trayMenuItems)
    {
        if (item->isCheckable() && item->isChecked())
            checkeds.append(item);
    }
    return checkeds;
}

/**
 * 返回选中项的索引
 * 该索引不包括自定义控件、布局等
 */
QList<int> TrayMenu::checkedIndexes()
{
    QList<int> checkeds;
    for (int i = 0; i < m_trayMenuItems.size(); i++)
    {
        auto item = m_trayMenuItems.at(i);
        if (item->isCheckable() && item->isChecked())
            checkeds.append(i);
    }
    return checkeds;
}

/**
 * 返回选中项中的text列表
 */
QStringList TrayMenu::checkedItemTexts()
{
    QStringList texts;
    foreach (auto item, m_trayMenuItems)
    {
        if (item->isCheckable() && item->isChecked())
            texts.append(item->getText());
    }
    return texts;
}

/**
 * 返回选中项的自定义data列表
 * 如果没设置的话，会是无效的QVariant
 */
QList<QVariant> TrayMenu::checkedItemDatas()
{
    QList<QVariant> datas;
    foreach (auto item, m_trayMenuItems)
    {
        if (item->isCheckable() && item->isChecked())
            datas.append(item->getData());
    }
    return datas;
}

/**
 * 一键设置已有菜单项为单选项
 * 注意，一定要在添加后设置，否则对后添加的项无效
 */
TrayMenu *TrayMenu::setSingleCheck(FuncCheckType clicked)
{
    for (int i = 0; i < m_trayMenuItems.size(); i++)
    {
        auto item = m_trayMenuItems.at(i);
        if (!item->isCheckable())
            item->setCheckable(true);

        item->triggered([=]{
            item->toggle();
            if (clicked)
                clicked(i, item->isChecked());
        });
    }
    return this;
}

/**
 * 一键设置已经菜单项为多选项
 * 注意，一定要在添加后设置，否则对后添加的项无效
 */
TrayMenu *TrayMenu::setMultiCheck(FuncCheckType clicked)
{
    for (int i = 0; i < m_trayMenuItems.size(); i++)
    {
        auto item = m_trayMenuItems.at(i);
        item->setCheckable(true)->linger(); // 多选，点了一下不能消失

        item->triggered([=]{
            item->toggle();
            if (clicked)
                clicked(i, item->isChecked());
        });
    }
    return this;
}

/**
 * 设置右边提示的区域内容
 * 一般是快捷键
 * 尽量在添加菜单项前设置
 */
TrayMenu *TrayMenu::setTipArea(int x)
{
    addin_tip_area = x;
    return this;
}

/**
 * 设置右边提示的区域内容
 * 一般是用来放快捷键
 * 尽量在添加菜单项前设置
 * @param tip 内容是什么不重要，只要等同于需要容纳的最长字符串即可（例如"ctrl+shit+alt+s"）
 */
TrayMenu *TrayMenu::setTipArea(QString longestTip)
{
    QFontMetrics fm(this->font());
    addin_tip_area = fm.horizontalAdvance(longestTip + "Ctrl");
    // 修改现有的
    foreach (auto item, m_trayMenuItems)
        item->setPaddings(item_padding, addin_tip_area > 0 ? tip_area_spacing + addin_tip_area : item_padding, item_padding, item_padding);
    return this;
}

/**
 * 设置是否默认分割同一行的菜单项
 * 如果关闭，可手动使用 split() 或 addVSeparator() 分割
 */
TrayMenu *TrayMenu::setSplitInRow(bool split)
{
    split_in_row = split;
    return this;
}

TrayMenu *TrayMenu::setBorderRadius(int r)
{
    border_radius = r;
    foreach (auto item, m_trayMenuItems)
        item->subMenu() && item->subMenu()->setBorderRadius(r);
    return this;
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
    this->enable_disappear_animation = en;
    foreach (auto item, m_trayMenuItems)
        item->subMenu() && item->subMenu()->setDisappearAnimation(en);
    return this;
}

TrayMenu *TrayMenu::setSubMenuShowOnCursor(bool en)
{
    this->sub_menu_show_on_cursor = en;
    foreach (auto item, m_trayMenuItems)
        item->subMenu() && item->subMenu()->setSubMenuShowOnCursor(en);
    return this;
}

void TrayMenu::OnItemMouseEntered(TrayMenuItem *item)
{
    if (m_startShowingAnimationMode)
        return ;
    int index = m_trayMenuItems.indexOf(item);
    if (using_keyboard && m_currentElementIndex > -1 && m_currentElementIndex < m_trayMenuItems.size() && m_currentElementIndex != index) // 屏蔽键盘操作
        m_trayMenuItems.at(m_currentElementIndex)->discardHoverPress(true);
    m_currentElementIndex = index;

    if (current_sub_menu) // 进入这个action，其他action展开的子菜单隐藏起来
    {
        current_sub_menu->hidden_by_another = true;
        current_sub_menu->hide();
        current_sub_menu = Q_NULLPTR;
    }
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
        if (split_in_row && m_rowHorizontalLayoutsList.last()->count() > 0)
            AddVerticalSeparator(); // Добавить вертикальный разделитель
        m_rowHorizontalLayoutsList.last()->addWidget(itemToAdd);
    }
    else
    {
        m_mainVerticalLayout->addWidget(itemToAdd);
    }
    m_trayMenuItems.append(itemToAdd);
    return itemToAdd;
}

Qt::Key TrayMenu::getShortcutByText(QString text) const
{
    Qt::Key key = Qt::Key_Exit;
    QRegularExpression re("&([\\d\\w])");
    auto match = re.match(text);
    if (match.hasMatch())
    {
        const QChar ch = match.capturedTexts().at(1).at(0); // 快捷键字符串
        if (ch >= '0' && ch <= '9')
            key = (Qt::Key)(Qt::Key_0 + (ch.toLatin1() - '0'));
        else if (ch >= 'a' && ch <= 'z')
            key = (Qt::Key)(Qt::Key_A + (ch.toUpper().toLatin1() - 'A'));
        else if (ch >= 'A' && ch <= 'Z')
            key = (Qt::Key)(Qt::Key_A + (ch.toUpper().toLatin1() - 'A'));
    }
    return key;
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

void TrayMenu::showSubMenu(TrayMenuItem *item)
{
    if (current_sub_menu)
    {
        if (item->subMenu() == current_sub_menu && !current_sub_menu->isHidden()) // 当前显示的就是这个子菜单
            return ;
        current_sub_menu->hidden_by_another = true; // 不隐藏父控件
        current_sub_menu->hide();
    }

    if (item->subMenu()->m_trayMenuItems.count() == 0) // 没有菜单项，不显示
        return ;

    current_sub_menu = item->subMenu();
    QPoint pos(-1, -1);
    QRect avai = m_trayMenuRect;
    if (using_keyboard) // 键盘模式，不是跟随鼠标位置来的
    {
        // 键盘模式，相对于点击项的右边
        QPoint tl = mapToGlobal(item->pos());
        if (tl.x() + item->width() + current_sub_menu->width() < avai.width())
            pos.setX(tl.x() + item->width());
        else
            pos.setX(tl.x() - current_sub_menu->width());
        if (tl.y() + current_sub_menu->height() < avai.height())
            pos.setY(tl.y());
        else
            pos.setY(tl.y() - current_sub_menu->height());
    }
    if (sub_menu_show_on_cursor)
        current_sub_menu->exec(pos);
    else
    {
        auto geom = item->mapToGlobal(QPoint(0, 0));
        auto rect = QRect(geom, item->size());
        current_sub_menu->exec(rect, false, rect.topRight());
    }
    current_sub_menu->setKeyBoardUsed(using_keyboard);
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
    if (m_currentElementIndex > -1 && child && m_trayMenuItems.at(m_currentElementIndex)->subMenu() == child && m_trayMenuItems.at(m_currentElementIndex)->geometry().contains(mapFromGlobal(pos)))
        return false;
    // 在这个菜单内的其他按钮
    return true;
}

void TrayMenu::setKeyBoardUsed(bool use)
{
    using_keyboard = use;
    if (use && m_currentElementIndex == -1 && m_trayMenuItems.size()) // 还没有选中
    {
        m_trayMenuItems.at(m_currentElementIndex = 0)->simulateHover(); // 预先选中第一项
    }
}

/**
 * В меню появляется анимация
 * последовательно от позиции курсора
 */
void TrayMenu::StartButtonAnimationOnShowedMainRectange()
{
    /**
     * Не заворачивай анимацию у умные указатели!!!!
     * Перед любой анимацией делай виджеты недоступными!!
     */
    if (m_buttonAnimationIsEnabled)
    {
        // анимация задается параметром curva
        const QEasingCurve curva=m_easingCurve;
        m_mainVerticalLayout->setEnabled(false);
        m_startShowingAnimationMode = true;


        QPoint startPosition = mapFromGlobal(QCursor::pos());
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
            propertyActionAnimation->setEasingCurve(curva);
            propertyActionAnimation->setDuration(m_durationToAnimateElements);
            connect(&(*propertyActionAnimation), &QPropertyAnimation::finished, item, [=]{
                item->setBlockHover(false);
            });
            propertyActionAnimation->start();
        }
        for (QWidget *label: m_addedLabels)
        {
            QPropertyAnimation* propertyLabelAnimation = new QPropertyAnimation(label, "pos", Q_NULLPTR);
            propertyLabelAnimation->setStartValue(startPosition);
            propertyLabelAnimation->setEndValue(label->pos());
            propertyLabelAnimation->setEasingCurve(curva);
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
                propertyAnimation->setEasingCurve(QEasingCurve::OutQuad);
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
    if (!enable_disappear_animation)
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
    int up_flow_count = focusIndex > -1 ? qMax(focusIndex, m_trayMenuItems.size()-focusIndex-1) : -1;
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
    for (int i = 0; i < m_addedLabels.size(); i++)
    {
        QWidget* btn = m_addedLabels.at(i);

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
    m_mainVerticalLayout->setEnabled(true);
    m_mainHorizontalLayout->invalidate();
    m_mainVerticalLayout->activate(); //активации небудет, если кеш не очищен

    // Автоматически регулировать диапазон на основе прямого и доступного
    QRect widgetRect = geometry();
    widgetRect.moveTo(QPoint(1750, 550));

    QRect expt=QRect(QPoint(1750, 750), QSize(150, 200));
    if (widgetRect.left() <= expt.right() && widgetRect.right() > expt.right())
        widgetRect.moveLeft(expt.right());
    widgetRect.moveTop(expt.top());

    // Избегайте положения экрана
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



    m_currentElementIndex = -1;

    // Установите фон в прямоугольник со скругленными углами
    QPixmap pixmap(width(), height());
    pixmap.fill(Qt::transparent);
    QPainter pix_ptr(&pixmap);
    pix_ptr.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), border_radius, border_radius);
    pix_ptr.fillPath(path, Qt::white);
    setMask(pixmap.mask());

    // Захватывать ли изображения с размытым фоном
    // получаем изображение
    QRect rectBlur = this->geometry();
    int radius = qMin(64, qMin(width(), height())); // Радиус размытия, а также граница
    rectBlur.adjust(-radius, -radius, +radius, +radius);
    QScreen* screen = QApplication::screenAt(QCursor::pos());
    if (screen)
    {
        QPixmap bg = screen->grabWindow(QApplication::desktop()->winId(), rectBlur.left(), rectBlur.top(), rectBlur.width(), rectBlur.height());

        // начать размывать
        QT_BEGIN_NAMESPACE
        extern Q_WIDGETS_EXPORT void qt_blurImage( QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
        QT_END_NAMESPACE

        QPixmap pixmap = bg;
        QPainter painter( &pixmap );
        // Заполните полупрозрачным цветом фона, чтобы не быть слишком прозрачным.
        {
            QColor bg_c(m_normalBackGroud);
            bg_c.setAlpha(m_normalBackGroud.alpha() * (100 - m_blurAlphaValue) / 100);
            painter.fillRect(0, 0, pixmap.width(), pixmap.height(), bg_c);
        }
        QImage img = pixmap.toImage(); // img -blur-> painter(pixmap)
        qt_blurImage( &painter, img, radius, true, false );
        // Обрежьте края (после размытия будут черные границы)
        int c = qMin(bg.width(), bg.height());
        c = qMin(c/2, radius);
        m_backgorudPixmap = pixmap.copy(c, c, pixmap.width()-c*2, pixmap.height()-c*2);
    }

    // Некоторые отображаются повторно и требуют повторной инициализации.
    hidden_by_another = false;
    using_keyboard = false;
    closed_by_clicked = false;

    // показать анимацию
    setFocus();
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
        if (m_currentElementIndex >= 0 && m_currentElementIndex <= m_trayMenuItems.size())
            m_trayMenuItems.at(m_currentElementIndex)->discardHoverPress(false);
    }
}

void TrayMenu::keyPressEvent(QKeyEvent *event)
{
    const int keyCode = event->key();
    for (TrayMenuItem* item: m_trayMenuItems)
    {
        if (item->isKey((Qt::Key)keyCode))
        {
            m_startShowingAnimationMode = false; // При показе щелчок недействителен, поэтому отключите его.
            item->simulateStatePress(); // Убедитесь, что это сочетание клавиш для этого действия
            return ;
        }
    }

    // Отклик клавиши меню
    if (event->modifiers() != Qt::NoModifier || m_trayMenuItems.size() == 0)
        return QWidget::keyPressEvent(event);

    switch (keyCode) {
    case Qt::Key_Up:
    {
        if (m_currentElementIndex < 0 || m_currentElementIndex >= m_trayMenuItems.size())
            m_currentElementIndex = m_trayMenuItems.size()-1;
        else
        {
            m_trayMenuItems.at(m_currentElementIndex--)->discardHoverPress(true);
            if (m_currentElementIndex < 0)
                m_currentElementIndex = m_trayMenuItems.size()-1;
        }
        auto focusToHoriFirst = [&]{
            // Если это горизонтальная кнопка, она должна фокусироваться на первом элементе; если первый элемент недоступен, задержка по порядку
            if (m_currentElementIndex > 0 && m_trayMenuItems.at(m_currentElementIndex-1)->pos().y() == m_trayMenuItems.at(m_currentElementIndex)->pos().y())
            {
                int last_index = m_currentElementIndex;
                // Получить первый элемент текущей строки
                int y = m_trayMenuItems.at(m_currentElementIndex)->pos().y();
                while (m_currentElementIndex > 0 && m_trayMenuItems.at(m_currentElementIndex-1)->pos().y() == y)
                    m_currentElementIndex--;
                int first_index = m_currentElementIndex;
                // Перейти к первому доступному элементу
                while (m_currentElementIndex < last_index && !m_trayMenuItems.at(m_currentElementIndex)->isEnabled())
                    m_currentElementIndex++;
                // Если эта строка вообще недоступна, принудительно вернуться к первому элементу
                if (m_currentElementIndex == last_index)
                    m_currentElementIndex = first_index;
            }
        };
        focusToHoriFirst();
        // найти настоящий предыдущий элемент
        m_trayMenuItems.at(m_currentElementIndex)->simulateHover();
        using_keyboard = true;
        return ;
    }
    case Qt::Key_Down:
    {
        if (m_currentElementIndex < 0 || m_currentElementIndex >= m_trayMenuItems.size())
            m_currentElementIndex = 0;
        else
        {
            m_trayMenuItems.at(m_currentElementIndex++)->discardHoverPress(true);
            if (m_currentElementIndex >= m_trayMenuItems.size())
                m_currentElementIndex = 0;
        }
        auto focusIgnoreHorizoneRest = [&]{
            // 跳过同一行后面所有（至少要先聚焦在这一行的第二项）
            if (m_currentElementIndex > 0 && m_currentElementIndex < m_trayMenuItems.size()-1 && m_trayMenuItems.at(m_currentElementIndex-1)->pos().y() == m_trayMenuItems.at(m_currentElementIndex)->pos().y())
            {
                int y = m_trayMenuItems.at(m_currentElementIndex)->pos().y();
                // 先判断前面是不是有可以点击的
                int temp_index = m_currentElementIndex;
                bool is_line_second = false;
                while (--temp_index > 0 && m_trayMenuItems.at(temp_index)->pos().y() == y)
                {
                    if (m_trayMenuItems.at(temp_index)->isEnabled())
                    {
                        is_line_second = true;
                        break;
                    }
                }
                if (!is_line_second) // 这是这一行的第一项，可以聚焦
                    return ;
                // 跳过这一行后面所有的按钮
                while (m_currentElementIndex < m_trayMenuItems.size()-1 && m_trayMenuItems.at(m_currentElementIndex+1)->pos().y() == y)
                    m_currentElementIndex++;
                m_currentElementIndex++;
                if (m_currentElementIndex >= m_trayMenuItems.size())
                    m_currentElementIndex = 0;
            }
        };
        focusIgnoreHorizoneRest();
        // 判断 item 是否被禁用
        if (!m_trayMenuItems.at(m_currentElementIndex)->isEnabled())
        {
            int old_index = m_currentElementIndex;
            do {
                m_currentElementIndex++;
                if (m_currentElementIndex >= m_trayMenuItems.size())
                    m_currentElementIndex = 0;
                else
                    focusIgnoreHorizoneRest();
            } while (m_currentElementIndex != old_index && !m_trayMenuItems.at(m_currentElementIndex)->isEnabled());
            if (m_currentElementIndex == old_index) // 如果绕了一圈都不能用，取消
                return ;
        }
        // 找到真正的上一项
        m_trayMenuItems.at(m_currentElementIndex)->simulateHover();
        using_keyboard = true;
        return ;
    }
    case Qt::Key_Left:
        // 移动到左边按钮
        if (m_currentElementIndex > 0 && m_trayMenuItems.at(m_currentElementIndex-1)->pos().y() == m_trayMenuItems.at(m_currentElementIndex)->pos().y())
        {
            m_trayMenuItems.at(m_currentElementIndex--)->discardHoverPress(true);
            // 找到左边第一项能点击的按钮；如果没有，宁可移动到上几行
            if (!m_trayMenuItems.at(m_currentElementIndex)->isEnabled())
            {
                int y = m_trayMenuItems.at(m_currentElementIndex)->pos().y();
                int ori_index = m_currentElementIndex;
                /*while (--current_index >= 0 && !items.at(current_index)->isEnabled()) ;
                if (current_index == -1) // 前面没有能选的了
                {
                    current_index = ori_index + 1; // 恢复到之前选择的那一项
                    return ;
                }*/
                while (--m_currentElementIndex >= 0 && !m_trayMenuItems.at(m_currentElementIndex)->isEnabled() && m_trayMenuItems.at(m_currentElementIndex)->pos().y() == y) ;
                if (m_currentElementIndex < 0 || m_trayMenuItems.at(m_currentElementIndex)->pos().y() != y) // 前面没有能选的了
                    m_currentElementIndex = ori_index + 1; // 恢复到之前选择的那一项
            }
            // 聚焦到这个能点的按钮
            m_trayMenuItems.at(m_currentElementIndex)->simulateHover();
            using_keyboard = true;
        }
        // 退出子菜单
        return ;
    case Qt::Key_Right:
        // 移动到右边按钮
        if (m_currentElementIndex < m_trayMenuItems.size()-1 && m_trayMenuItems.at(m_currentElementIndex+1)->pos().y() == m_trayMenuItems.at(m_currentElementIndex)->pos().y())
        {
            m_trayMenuItems.at(m_currentElementIndex++)->discardHoverPress(true);
            // 找到右边第一项能点击的按钮；如果没有，宁可移动到下几行
            if (!m_trayMenuItems.at(m_currentElementIndex)->isEnabled())
            {
                int y = m_trayMenuItems.at(m_currentElementIndex)->pos().y();
                int ori_index = m_currentElementIndex;
                /*while (++current_index < items.size() && !items.at(current_index)->isEnabled()) ;
                if (current_index == items.size()) // 后面没有能选的了
                {
                    current_index = ori_index - 1; // 恢复到之前选择的那一项
                    return ;
                }*/
                while (++m_currentElementIndex < m_trayMenuItems.size() && !m_trayMenuItems.at(m_currentElementIndex)->isEnabled() && m_trayMenuItems.at(m_currentElementIndex)->pos().y() == y) ;
                if (m_currentElementIndex == m_trayMenuItems.size() || m_trayMenuItems.at(m_currentElementIndex)->pos().y() != y) // 后面没有能选的了
                    m_currentElementIndex = ori_index - 1; // 恢复到之前选择的那一项
            }
            // 聚焦到这个能点的按钮
            m_trayMenuItems.at(m_currentElementIndex)->simulateHover();
            using_keyboard = true;
        }
        // 展开子菜单
        else if (m_currentElementIndex >= 0 && m_currentElementIndex < m_trayMenuItems.size() && m_trayMenuItems.at(m_currentElementIndex)->isSubMenu())
        {
            showSubMenu(m_trayMenuItems.at(m_currentElementIndex));
        }
        return ;
    case Qt::Key_Home:
        if (m_currentElementIndex >= 0 || m_currentElementIndex < m_trayMenuItems.size())
            m_trayMenuItems.at(m_currentElementIndex)->discardHoverPress(true);
        // 聚焦到第一项能点的按钮
        if (!m_trayMenuItems.at(m_currentElementIndex = 0)->isEnabled())
        {
            while (++m_currentElementIndex < m_trayMenuItems.size() && !m_trayMenuItems.at(m_currentElementIndex)->isEnabled());
            if(m_currentElementIndex == m_trayMenuItems.size()) // 没有能点的（不太可能）
                return ;
        }
        m_trayMenuItems.at(m_currentElementIndex)->simulateHover();
        using_keyboard = true;
        return ;
    case Qt::Key_End:
        if (m_currentElementIndex >= 0 || m_currentElementIndex < m_trayMenuItems.size())
            m_trayMenuItems.at(m_currentElementIndex)->discardHoverPress(true);
        // 聚焦到最后一项能点的按钮
        if (!m_trayMenuItems.at(m_currentElementIndex = m_trayMenuItems.size()-1)->isEnabled())
        {
            while (--m_currentElementIndex >= 0 && !m_trayMenuItems.at(m_currentElementIndex)->isEnabled());
            if(m_currentElementIndex == -1) // 没有能点的（不太可能）
                return ;
        }
        m_trayMenuItems.at(m_currentElementIndex)->simulateHover();
        using_keyboard = true;
        return ;
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Space:
        if (m_currentElementIndex >= 0 || m_currentElementIndex < m_trayMenuItems.size())
        {
            auto item = m_trayMenuItems.at(m_currentElementIndex);
            item->simulateStatePress(!item->getState(), false);
        }
        return ;
    }

    return QWidget::keyPressEvent(event);
}

void TrayMenu::paintEvent(QPaintEvent *event)
{
    if (!m_backgorudPixmap.isNull())
    {
        QPainter painter(this);
        painter.drawPixmap(QRect(0,0,width(),height()), m_backgorudPixmap);
        return ;
    }
    QWidget::paintEvent(event);
}

void TrayMenu::SetTrayMenuColors(const QColor& normal, const QColor&  hover, const QColor&  press, const QColor&  text)
{
    m_normalBackGroud = normal;
    m_hoverBackgroud = hover;
    m_pressBackgroud = press;
    m_textColor = text;
}
