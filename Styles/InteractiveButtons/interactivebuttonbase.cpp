#include "interactivebuttonbase.h"

InteractiveButton::InteractiveButton(QWidget *parent)
    : QPushButton(parent)
    , icon(Q_NULLPTR)
    , m_buttonText("")
    , paint_addin()
    , fore_paddings(4, 4, 4, 4)
    , self_enabled(true)
    , parent_enabled(false)
    , fore_enabled(true)
    , isShowAnimation(false)
    , isShowForeground(true)
    , m_isShowAppearAnimation(false)
    , show_ani_disappearing(false)
    , show_duration(300)
    , show_timestamp(0)
    , hide_timestamp(0)
    , show_ani_progress(0)
    , show_ani_point(0, 0)
    , enter_pos(-1, -1)
    , press_pos(-1, -1)
    , release_pos(-1, -1)
    , mouse_pos(-1, -1)
    , anchor_pos(-1, -1)
    , offset_pos(0, 0)
    , effect_pos(-1, -1)
    , release_offset(0, 0)
    , m_isHovering(false)
    , m_isOnPressing(false)
    , hover_timestamp(0)
    , leave_timestamp(0)
    , press_timestamp(0)
    , release_timestamp(0)
    , hover_bg_duration(300)
    , press_bg_duration(300)
    , click_ani_duration(300)
    , anchor_timer(new QTimer(this))
    , move_speed(5),
      m_iconColor(0, 0, 0), text_color(0, 0, 0),
      normal_bg(0xF2, 0xF2, 0xF2, 0), hover_bg(128, 128, 128, 32), press_bg(128, 128, 128, 64), border_bg(0, 0, 0, 0),
      focus_bg(0, 0, 0, 0), focus_border(0, 0, 0, 0),
      m_hoverSpeed(5), press_start(40), m_pressSpeed(5),
      m_hoverProgress(0), m_pressProgress(0), icon_padding_proper(0.2), icon_text_padding(4), m_iconTextSize(16),
      m_borderWidth(1), m_radiusX(0), m_radiusY(0),
      m_fontSize(0), fixed_fore_pos(false), fixed_fore_size(false), isTextDynamicSize(false), auto_text_color(true), focusing(false),
      click_ani_appearing(false), click_ani_disappearing(false), click_ani_progress(0),
      m_mousePressEventPointer(Q_NULLPTR), m_mouseReleaseEventPointer(Q_NULLPTR),
      unified_geometry(false), _l(0), _t(0), _w(32), _h(32),
      jitter_animation(true), elastic_coefficient(1.2), jitter_duration(300),
      m_hasWaterAnimation(true), m_waterPressDuration(800), m_waterReleaseDuration(400), m_waterFinishDuration(300),
      m_aligment(Qt::AlignCenter), _state(false), m_leaveAfterClick(false), _block_hover(false),
      m_hasDoubleClicked(false), m_doubleClickTimer(Q_NULLPTR), m_doubleIsPrevented(false)
{
    setMouseTracking(true); // Захват событий движения, даже когда мышь не нажата, для дебага убрать

    m_paintModel = PaintModel::None;

    anchor_timer->setInterval(10);
    connect(anchor_timer, &QTimer::timeout, this, &InteractiveButton::OnAnchorTimeOut);

    SetWaterRipple();

    connect(this, &QPushButton::clicked, this, &InteractiveButton::OnClicked);

    //    setFocusPolicy(Qt::NoFocus); // Избегайте кнопки, также получающей фокус табуляции
}

/**
 * кнопка типа текста
 */
InteractiveButton::InteractiveButton(const QString &text, QWidget *parent)
    : InteractiveButton(parent)
{
    SetText(text);
}

/**
 * Кнопка типа значка
 */
InteractiveButton::InteractiveButton(const QIcon &icon, QWidget *parent)
    : InteractiveButton(parent)
{
    SetIcon(icon);
}

/**
 * Кнопки с изменяющимся цветом значка
 */
InteractiveButton::InteractiveButton(const QPixmap &pixmap, QWidget *parent)
    : InteractiveButton(parent)
{
    SetPixmap(pixmap);
}

InteractiveButton::InteractiveButton(const QIcon &icon, const  QString &text, QWidget *parent)
    : InteractiveButton(parent)
{
    SetIcon(icon);
    SetText(text);
}

InteractiveButton::InteractiveButton(const QPixmap &pixmap, const  QString &text, QWidget *parent)
    : InteractiveButton(parent)
{
    SetPixmap(pixmap);
    SetText(text);
}

InteractiveButton::~InteractiveButton()
{
    delete anchor_timer;
}

/**
 * установить текст кнопки
 * @param buttonText текст кнопки
 */
void InteractiveButton::SetText(const QString &buttonText)
{
    m_buttonText = buttonText;
    switch (m_paintModel)
    {
    case PaintModel::None:
    {
        m_paintModel = PaintModel::Text;
        SetAlign(Qt::AlignCenter);
        break;
    }
    case PaintModel::PixmapMask:
    {
        if (pixmap.isNull())
        {
            m_paintModel = PaintModel::Text;
        }
        else
        {
            m_paintModel = PaintModel::PixmapText;
        }
        SetAlign(Qt::AlignLeft | Qt::AlignVCenter);
        const QFontMetrics fm(QWidget::font());
        m_iconTextSize = fm.lineSpacing();
        break;
    }
    case PaintModel::Icon:
    {
        if (buttonText.isEmpty())
        {
            m_paintModel = PaintModel::Icon;
            SetAlign(Qt::AlignCenter);
        }
        else
        {
            if (icon.isNull())
            {
                m_paintModel = PaintModel::Text;
            }
            else
            {
                m_paintModel = PaintModel::IconText;
            }
            SetAlign(Qt::AlignLeft | Qt::AlignVCenter);
            const QFontMetrics fm(QWidget::font());
            m_iconTextSize = fm.lineSpacing();
            break;
        }
    }
    default:
    {
        break;
    }
    }

    if (parent_enabled)
    {
        QPushButton::setText(buttonText);
    }
    // Изменить размер в соответствии со шрифтом
    if (isTextDynamicSize)
    {
        RegulateMinimumSize();
    }
    update();
}

/**
 * 设置 icon 图标
 * @param path 图标路径文本
 */
void InteractiveButton::SetIcon(const QString &path)
{
    m_iconPath = path;
    const QIcon icon(path);
    SetIcon(icon);
}

/**
 * 设置 pixmap 图标
 * @param path 图标路径文本
 */
void InteractiveButton::setPixmapPath(const QString &path)
{
    SetPixmap(QPixmap(path));
}

/**
 * 设置 icon
 * @param icon 图标
 */
void InteractiveButton::SetIcon(const QIcon &icon)
{
    if (m_paintModel == PaintModel::None)
    {
        m_paintModel = PaintModel::Icon;
        SetAlign(Qt::AlignCenter);
    }
    else if (m_paintModel == PaintModel::Text)
    {
        if (m_buttonText.isEmpty())
            m_paintModel = PaintModel::Icon;
        else
            m_paintModel = PaintModel::IconText;
        SetAlign(Qt::AlignLeft | Qt::AlignVCenter);
        QFontMetrics fm(this->font());
        m_iconTextSize = fm.lineSpacing();
    }
    else if (m_paintModel == PaintModel::PixmapMask)
    {
        pixmap = QPixmap();
        m_paintModel = PaintModel::Icon;
    }
    else if (m_paintModel == PaintModel::PixmapText)
    {
        pixmap = QPixmap();
        if (m_buttonText.isEmpty())
            m_paintModel = PaintModel::Icon;
        else
            m_paintModel = PaintModel::IconText;
        SetAlign(Qt::AlignLeft | Qt::AlignVCenter);
        QFontMetrics fm(this->font());
        m_iconTextSize = fm.lineSpacing();
    }
    else if (m_paintModel == PaintModel::IconText && m_buttonText.isEmpty())
    {
        m_paintModel = PaintModel::Icon;
        SetAlign(Qt::AlignCenter);
    }
    this->icon = icon;
    if (parent_enabled)
        QPushButton::setIcon(icon);
    update();
}

/**
 * 设置 Pixmap
 * @param pixmap [description]
 */
void InteractiveButton::SetPixmap(const QPixmap &pixmap)
{
    if (m_paintModel == PaintModel::None)
        m_paintModel = PaintModel::PixmapMask;
    else if (m_paintModel == PaintModel::Text)
    {
        if (m_buttonText.isEmpty())
            m_paintModel = PaintModel::PixmapMask;
        else
            m_paintModel = PaintModel::PixmapText;
        SetAlign(Qt::AlignLeft | Qt::AlignVCenter);
        QFontMetrics fm(this->font());
        m_iconTextSize = fm.lineSpacing();
    }
    else if (m_paintModel == PaintModel::Icon)
    {
        icon = QIcon();
        m_paintModel = PaintModel::PixmapMask;
    }
    else if (m_paintModel == PaintModel::IconText)
    {
        icon = QIcon();
        if (m_buttonText.isEmpty())
            m_paintModel = PaintModel::PixmapMask;
        else
            m_paintModel = PaintModel::PixmapText;
        SetAlign(Qt::AlignLeft | Qt::AlignVCenter);
        QFontMetrics fm(this->font());
        m_iconTextSize = fm.lineSpacing();
    }
    this->pixmap = getMaskPixmap(pixmap, isEnabled() ? m_iconColor : getOpacityColor(m_iconColor));
    if (parent_enabled)
        QPushButton::setIcon(QIcon(pixmap));
    update();
}

/**
 * 设置额外的图标，例如角标
 * @param pixmap 图标
 * @param align  对齐方式
 * @param size   图标尺寸
 */
void InteractiveButton::setPaintAddin(QPixmap pixmap, Qt::Alignment align, QSize size)
{
    QBitmap mask = pixmap.mask();
    pixmap.fill(m_iconColor);
    pixmap.setMask(mask);
    paint_addin = PaintAddin(pixmap, align, size);
    update();
}

/**
 * 设置子类功能是否开启
 * 如果关闭，则相当于默认的 QPushButton
 * @param e 开关
 */
void InteractiveButton::setSelfEnabled(bool e)
{
    self_enabled = e;
}

/**
 * 设置父类（QPushButton）功能是否开启
 * 如果开启，则绘制父类背景、父类前景
 * @param e 开关
 */
void InteractiveButton::setParentEnabled(bool e)
{
    parent_enabled = e;

    // Передаем содержимое подкласса родительскому классу, чтобы подкласс не отображался после его отключения
    if (m_paintModel == PaintModel::Text || m_paintModel == PaintModel::IconText || m_paintModel == PaintModel::PixmapText)
        QPushButton::setText(m_buttonText);
    if (m_paintModel == PaintModel::Icon || m_paintModel == PaintModel::IconText)
        QPushButton::setIcon(icon);
    if (m_paintModel == PaintModel::PixmapMask || m_paintModel == PaintModel::PixmapText)
        QPushButton::setIcon(QIcon(pixmap));
}

/**
 * 设置是否绘制前景图标/文字
 * 关闭后则只绘制背景
 * @param e 开启
 */
void InteractiveButton::setForeEnabled(bool e)
{
    fore_enabled = e;
}

/**
 * 设置鼠标悬浮背景渐变的动画时长
 * @param d 动画时长（毫秒）
 */
void InteractiveButton::setHoverAniDuration(int d)
{
    this->hover_bg_duration = d;
    //    hover_progress = 0; // 重置hover效果
}

/**
 * 设置鼠标按下渐变效果的动画时长
 * @param d 动画时长（毫秒）
 */
void InteractiveButton::setPressAniDuration(int d)
{
    this->press_bg_duration = d;
}

/**
 * 设置单击效果的动画时长
 * @param d 动画时长（毫秒）
 */
void InteractiveButton::setClickAniDuration(int d)
{
    this->click_ani_duration = d;
}

/**
 * 设置水波纹动画时长
 * @param press   按住时时长（时长毫秒）
 * @param release 松开后速度（时长毫秒）
 * @param finish  渐变消失速度（时长毫秒）
 */
void InteractiveButton::setWaterAniDuration(int press, int release, int finish)
{
    this->m_waterPressDuration = press;
    this->m_waterReleaseDuration = release;
    this->m_waterFinishDuration = finish;
}

/**
 * Различные изменения статуса
 * В основном для контроля доступного статуса, установите полупрозрачный, когда недоступен
 */
void InteractiveButton::changeEvent(QEvent *event)
{
    QPushButton::changeEvent(event);

    if (event->type() == QEvent::EnabledChange && m_paintModel == PaintModel::PixmapMask) // 可用状态改变了
    {
        if (isEnabled()) // Восстановлено доступно: прозрачность изменена обратно
        {
            QColor color = m_iconColor;
            color.setAlpha(color.alpha() * 2);
            SetIconColor(color);
        }
        else // Стать недоступным: прозрачность уменьшена вдвое
        {
            QColor color = m_iconColor;
            color.setAlpha(color.alpha() / 2);
            SetIconColor(color);
        }
    }
}

/**
 * 设置水波纹动画是否开启
 * 关闭时，将使用渐变动画
 * @param enable 开关
 */
void InteractiveButton::SetWaterRipple(bool enable)
{
    if (m_hasWaterAnimation == enable)
        return;
    m_hasWaterAnimation = enable;
}

/**
 * 设置抖动效果是否开启
 * 鼠标拖拽移动的距离越长，抖动距离越长、次数越多
 * @param enable 开关
 */
void InteractiveButton::setJitterAni(bool enable)
{
    jitter_animation = enable;
}

/**
 * 设置是否使用统一图标绘制区域
 * 监听图标尺寸大小变化、中心点偏移，计算新的中心坐标位置
 * @param enable 开关
 */
void InteractiveButton::setUnifyGeomerey(bool enable)
{
    unified_geometry = enable;
    _l = _t = 0;
    _w = width();
    _h = height();
}

/**
 * установить цвет фона
 * @param bg 背景颜色
 */
void InteractiveButton::SetBackgroundColor(const QColor &color)
{
    SetNormalColor(color);
    update();
}

/**
 * Установить цвет фона события
 * @param hover 鼠标悬浮时的背景颜色
 * @param press 鼠标按下时的背景颜色
 */
void InteractiveButton::SetBackgroundColor(const QColor &hover, const QColor &press)
{
    if (hover != Qt::black)
        SetHoverColor(hover);
    if (press != Qt::black)
        SetPressColor(press);
    update();
}

/**
 * Установить цвет фона кнопки
 * @param color фоновый цвет
 */
void InteractiveButton::SetNormalColor(const QColor &color)
{
    normal_bg = color;
    update();
}

/**
 * Установить цвет линии границы
 * @param color цвет границы
 */
void InteractiveButton::SetBorderColor(const QColor &color)
{
    border_bg = color;
}

/**
 * Установите цвет фона при наведении мыши
 * @param color фоновый цвет
 */
void InteractiveButton::SetHoverColor(const QColor &color)
{
    hover_bg = color;
}

/**
 * Установите цвет фона, когда мышь удерживается нажатой
 * @param color фоновый цвет
 */
void InteractiveButton::SetPressColor(const QColor &color)
{
    press_bg = color;
}

/**
 * Установите цвет значка (только для изменяющих цвет значков растрового изображения)
 * @param color Цвет значка
 */
void InteractiveButton::SetIconColor(const QColor &color)
{
    m_iconColor = color;

    // нарисовать значок (если есть)
    if (m_paintModel == PaintModel::PixmapMask || m_paintModel == PaintModel::PixmapText)
    {
        pixmap = getMaskPixmap(pixmap, isEnabled() ? m_iconColor : getOpacityColor(m_iconColor));
    }

    // нарисовать дополнительные метки (если есть)
    if (paint_addin.enable)
    {
        paint_addin.pixmap = getMaskPixmap(paint_addin.pixmap, isEnabled() ? m_iconColor : getOpacityColor(m_iconColor));
    }

    update();
}

/**
 * 设置前景文字颜色
 * @param color 文字颜色
 */
void InteractiveButton::SetTextColor(QColor color)
{
    text_color = color;
    update();
}

/**
 * 设置获取焦点时的背景颜色（默认关闭）
 * @param color 背景颜色
 */
void InteractiveButton::setFocusBg(QColor color)
{
    setFocusPolicy(Qt::StrongFocus);
    focus_bg = color;
}

/**
 * 设置获取焦点时的边框颜色（默认关闭）
 * @param color 边框颜色
 */
void InteractiveButton::setFocusBorder(QColor color)
{
    setFocusPolicy(Qt::StrongFocus);
    focus_border = color;
}

/**
 * Установите размер текста (PointSize, переопределить font() размер шрифта)
 */
void InteractiveButton::SetFontSize(int newFontSize)
{
    if (!m_fontSize) // Установите размер шрифта в первый раз, установите его напрямую
    {
        m_fontSize = newFontSize;
        QFont font(this->font());
        font.setPointSize(newFontSize);
        setFont(font);
        update();
    }
    else // Изменить размер шрифта, использовать анимацию масштабирования шрифта
    {
        QPropertyAnimation *const animation = new QPropertyAnimation(this, "font_size");
        animation->setStartValue(m_fontSize);
        animation->setEndValue(newFontSize);
        animation->setDuration(click_ani_duration);
        connect(animation, &QPropertyAnimation::finished, [ = ]
        {
            const QFontMetrics fm(QPushButton::font());
            m_iconTextSize = fm.lineSpacing();
        });
        animation->start(QPropertyAnimation::DeleteWhenStopped);
    }
    // Отрегулируйте минимальный размер кнопки при изменении размера шрифта, чтобы избежать неполного отображения текста.
    if (isTextDynamicSize)
    {
        QFont font;
        font.setPointSize(newFontSize);
        QFontMetrics fms(font);
        setMinimumSize(fms.horizontalAdvance(m_buttonText) + fore_paddings.left + fore_paddings.right, fms.lineSpacing() + fore_paddings.top + fore_paddings.bottom);
    }
    if (m_paintModel != PaintModel::Text)
    {
        const QFontMetrics fm(this->font());
        m_iconTextSize = fm.lineSpacing();
    }
}

/**
 * Получить размер шрифта
 * Используется как параметр свойства для анимации шрифта
 * @return временный размер шрифта
 */
int InteractiveButton::GetFontSizeT()
{
    return m_fontSize;
}

/**
 * Установить временный размер шрифта в анимации
 * Используется как параметр свойства для анимации шрифта
 * @param f временный размер шрифта
 */
void InteractiveButton::SetFontSizeT(int fontSize)
{
    m_fontSize = fontSize;
    QFont font(this->font());
    font.setPointSize(fontSize);
    setFont(font);
    update();
}

/**
 * Если щелчок потеряет фокус, фон не появится, даже если мышь наведется на него
 * Вы можете использовать этот метод, чтобы продолжать поддерживать приостановленное состояние
 */
void InteractiveButton::SetHover()
{
    if (!m_isHovering && inArea(mapFromGlobal(QCursor::pos())))
    {
        InteractiveButton::enterEvent(new QEvent(QEvent::Type::None));
    }
}

/**
 * установить выравнивание
 * @param a Выравнивание
 */
void InteractiveButton::SetAlign(Qt::Alignment aligment)
{
    m_aligment = aligment;
    update();
}

/**
 * Установите бордер радиус
 * @param r 半径
 */
void InteractiveButton::SetRadius(int radius)
{
    m_radiusX = m_radiusY = radius;
}

/**
 * Установите радиус X, Y отдельно
 * @param radiusX X радиус
 * @param radiusY Y радиус
 */
void InteractiveButton::SetRadius(int radiusX, int radiusY)
{
    m_radiusX = radiusX;
    m_radiusY = radiusY;
}

/**
 * Установите толщину линии границы
 * @param x Толщина линии
 */
void InteractiveButton::setBorderWidth(int borderWidth)
{
    m_borderWidth = borderWidth;
}

/**
 * установить недоступное условие
 * отличный от setEnabled(bool)，Два противоположных, не переопределяющих метода
 */
void InteractiveButton::SetDisabled(bool newState)
{
    if (newState == !isEnabled()) //идентичный
    {
        return;
    }
    setEnabled(!newState);

    if (parentWidget() != Q_NULLPTR)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents, newState); // Пролистать
    }

    if (PaintModel::PixmapMask == m_paintModel || PaintModel::PixmapText == m_paintModel)
    {
        pixmap = getMaskPixmap(pixmap, newState ? getOpacityColor(m_iconColor) : m_iconColor);
    }

    update(); // Изменить прозрачность
}

/**
 * Установите отступы для переднего плана и четырех сторон
 * @param l левое поле
 * @param r правое поле
 * @param t верхнее поле
 * @param b Нижнее поле
 */
void InteractiveButton::SetPaddings(int l, int r, int t, int b)
{
    fore_paddings.left = l;
    fore_paddings.right = r;
    fore_paddings.top = t;
    fore_paddings.bottom = b;
    setFixedForeSize();
}

/**
 * 统一设置方向的 paddings
 * @param h 横向
 * @param v 纵向
 */
void InteractiveButton::SetPaddings(int h, int v)
{
    fore_paddings.left = fore_paddings.right = (h + 1) / 2;
    fore_paddings.top = fore_paddings.bottom = (v + 1) / 2;
    setFixedForeSize();
}

/**
 * 统一设置前景和四条边的 paddings
 * @param x 一样大小的四边留白
 */
void InteractiveButton::SetPaddings(int x)
{
    fore_paddings.left = x;
    fore_paddings.right = x;
    fore_paddings.top = x;
    fore_paddings.bottom = x;
    setFixedForeSize();
}

/**
 * 设置Icon模式旁边空多少
 * @param x 0~1.0，越大越空
 */
void InteractiveButton::SetIconPaddingProper(double x)
{
    icon_padding_proper = x;
    const int shortSide = min(width(), height()); // Короткая сторона
    //Для нефиксированных случаев все отступы должны быть изменены после изменения размера.
    const int padding = int(shortSide * icon_padding_proper); //static_cast<int>(short_side * (1 - GOLDEN_RATIO) / 2);
    fore_paddings.left = fore_paddings.top = fore_paddings.right = fore_paddings.bottom = padding;
    update();
}

/**
 * 设置字体大小时是否同步修改按钮的最小尺寸（避免按钮显示不全）
 * @param d 开关
 */
void InteractiveButton::setTextDynamicSize(bool d)
{
    isTextDynamicSize = d;
}

/**
 * 见 setFixedForePos(bool f)
 */
void InteractiveButton::setFixedTextPos(bool f)
{
    fixed_fore_pos = f;
}

/**
 * 设置前景是否固定，而不移动
 * 将去除鼠标移入靠近、抖动效果，统一图标区域大小不变
 * 只包括：鼠标进入/点击，均表现为缩放效果（默认）
 * 不影响任何其他功能
 * @param f [description]
 */
void InteractiveButton::setFixedForePos(bool f)
{
    fixed_fore_pos = f;
}

/**
 * 固定按钮(最小值)为适当尺寸，并且固定四周留白
 * 前景应为文字/图标对应尺寸的最小尺寸
 * @param f     是否固定前景
 * @param addin 留白的像素大小
 */
void InteractiveButton::setFixedForeSize(bool f, int addin)
{
    fixed_fore_size = f;

    if (!f)
        return;
    if (m_paintModel == PaintModel::Text || m_paintModel == PaintModel::IconText || m_paintModel == PaintModel::PixmapText)
    {
        int icon_width = (m_paintModel != PaintModel::Text && icon.isNull()) ? 0 : m_iconTextSize;
        QFont font = this->font();
        if (m_fontSize > 0)
            font.setPointSize(m_fontSize);
        QFontMetrics fm(font);
        int w = fm.horizontalAdvance(m_buttonText);
        w = icon_width + w + quick_sqrt(w / 2) + fore_paddings.left + fore_paddings.right;
        setMinimumSize(
            w + addin,
            fm.lineSpacing() + fore_paddings.top + fore_paddings.bottom + addin);
    }
    else if (m_paintModel == PaintModel::Icon || m_paintModel == PaintModel::PixmapMask)
    {
        int size = height();
        setMinimumSize(size + addin, size + addin);
    }
}

void InteractiveButton::setSquareSize()
{
    setFixedWidth(height());
    setMinimumWidth(height());
    setMaximumWidth(height());
}

/**
 * 设置鼠标单击松开后是否当做移开
 * 避免菜单、弹窗出现后，由于鼠标仍然留在按钮上面，导致依旧显示 hover 背景
 * @param l 开关
 */
void InteractiveButton::setLeaveAfterClick(bool l)
{
    m_leaveAfterClick = l;
}

/**
 * 响应双击事件
 * 注意：会先触发单击事件、再触发双击事件(其实就是懒得做)
 * 建议在 QListWidget 等地方使用！
 * @param e 开关
 */
void InteractiveButton::setDoubleClicked(bool e)
{
    m_hasDoubleClicked = e;

    if (m_doubleClickTimer == Q_NULLPTR)
    {
        m_doubleClickTimer = new QTimer(this);
        m_doubleClickTimer->setInterval(DOUBLE_PRESS_INTERVAL);
        connect(m_doubleClickTimer, &QTimer::timeout, [ = ]
        {
            m_doubleClickTimer->stop();
            Q_EMIT clicked(); // 手动触发单击事件
        });
    }
}

/**
 * Автоматически устанавливать цвет текста во время анимации
 */
void InteractiveButton::setAutoTextColor(bool a)
{
    this->auto_text_color = a;
}

/**
 * Делайте вид, что сфокусировались, когда изначально не были сфокусированы
 * Сделайте другие элементы управления (например, QLineEdit) нажатием клавиши ввода, чтобы вызвать это событие кнопки через слот сигнала
 * Пока не сработает событие изменения фокуса, элемент управления теряет фокус (требуется ручное изменение)
 */
void InteractiveButton::setPretendFocus(bool f)
{
    focusing = f;
    update();
}

/**
 * 如果按钮被做成一个组合，在显示的时候开启动画
 * 一开始鼠标下的按钮一直在hover状态，移开也不会变
 * 开启后临时屏蔽，记得在动画结束后关闭
 */
void InteractiveButton::setBlockHover(bool b)
{
    _block_hover = b;
    if (b && m_isHovering)
        leaveEvent(Q_NULLPTR);
}

/**
 * 是否开启出现动画
 * 鼠标进入按钮区域，前景图标从对面方向缩放出现
 * @param enable 开关
 */
void InteractiveButton::setShowAni(bool enable)
{
    isShowAnimation = enable;

    if (!isShowAnimation) // 关闭隐藏前景
    {
        isShowForeground = true;
    }
    else if (isShowAnimation) // 开启隐藏前景
    {
        if (!m_isHovering && !m_isOnPressing) // 应该是隐藏状态
        {
            m_isShowAppearAnimation = show_ani_disappearing = isShowForeground = false;
            show_ani_progress = 0;
        }
        else // 应该是显示状态
        {
            isShowForeground = true;
            m_isShowAppearAnimation = show_ani_disappearing = false;
            show_ani_progress = 100;
        }
    }
}

/**
 * 按钮前景出现动画
 * 从中心点出现的缩放动画
 */
void InteractiveButton::showForeground()
{
    if (!isShowAnimation)
        return;
    waters.clear();
    if (!anchor_timer->isActive())
        anchor_timer->start();
    if (show_ani_disappearing)
        show_ani_disappearing = false;
    m_isShowAppearAnimation = true;
    show_timestamp = getTimestamp();
    isShowForeground = true;
    show_ani_point = QPoint(0, 0);
}

/**
 * 按钮前景出现动画2
 * 指定方向（笛卡尔坐标），从反方向至中心点
 * @param point 最开始出现的方向（大小不影响，只按 x、y 比例来）
 */
void InteractiveButton::showForeground2(QPoint point)
{
    showForeground();
    if (point == QPoint(0, 0))
        point = mapFromGlobal(QCursor::pos()) - QPoint(width() / 2, height() / 2); // 相对于按钮中心
    show_ani_point = point;

    if (unified_geometry) // 统一出现动画
        updateUnifiedGeometry();
}

/**
 * 隐藏前景
 * 为下一次的出现动画做准备
 */
void InteractiveButton::hideForeground()
{
    if (!isShowAnimation)
        return;
    if (!anchor_timer->isActive())
        anchor_timer->start();
    if (m_isShowAppearAnimation)
        m_isShowAppearAnimation = false;
    show_ani_disappearing = true;
    hide_timestamp = getTimestamp();
}

/**
 * 延迟出现前景
 * 适用于多个按钮连续出现的一套效果
 * @param time  延迟时长（毫秒）
 * @param point 出现方向
 */
void InteractiveButton::delayShowed(int time, QPoint point)
{
    setShowAni(true);
    QTimer::singleShot(time, [ = ]
    {
        showForeground2(point);
        connect(this, &InteractiveButton::ToShowAnimationFinished, [ = ] {
            setShowAni(false);
            disconnect(this, SIGNAL(ToShowAnimationFinished()), Q_NULLPTR, Q_NULLPTR);
        });
    });
}

/**
 * 获取文字
 */
const QString &InteractiveButton::GetText() const noexcept
{
    return m_buttonText;
}

/**
 * 设置菜单
 * 并解决菜单无法监听到 release 的问题
 * @param menu 菜单对象
 */
void InteractiveButton::setMenu(QMenu *menu)
{
    // Значение по умолчанию — не получать событие фокуса
    //, поэтому, если установлено меню, события Release не будет
    //, а анимация водной ряби всегда будет плавать.
    // В событии focusOut моделируется событие освобождения,
    QWidget::setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    QPushButton::setMenu(menu);
}

/**
 * 根据内容，调整最小尺寸（不影响最大值）
 */
void InteractiveButton::RegulateMinimumSize()
{
    int icon_width = (m_paintModel == PaintModel::Text || m_paintModel == PaintModel::None) || icon.isNull() ? 0 : m_iconTextSize;
    if (icon_width && !m_buttonText.isEmpty())
        icon_width += icon_text_padding;
    int w = 0, h = 0;
    if (m_fontSize <= 0)
    {
        QFontMetrics fm(font());
        w = fm.horizontalAdvance(m_buttonText);
        w = icon_width + w + quick_sqrt(w / 2) + fore_paddings.left + fore_paddings.right;
        h = fm.lineSpacing() + fore_paddings.top + fore_paddings.bottom;
    }
    else
    {
        QFont font;
        font.setPointSize(m_fontSize);
        QFontMetrics fm(font);
        w = fm.horizontalAdvance(m_buttonText);
        w = icon_width + w + quick_sqrt(w / 2) + fore_paddings.left + fore_paddings.right;
        h = fm.lineSpacing() + fore_paddings.top + fore_paddings.bottom;
    }
    setMinimumSize(w, h);
}

/**
 * 设置状态
 * 一个用来作为开关效果的属性
 * @param s 状态
 */
void InteractiveButton::setState(bool s)
{
    _state = s;
    update();
}

/**
 * 获取状态
 * @return 状态
 */
bool InteractiveButton::GetCheckedState() const noexcept
{
    return _state;
}

/**
 * 模拟按下开关的效果，并改变状态
 * 如果不使用状态，则出现点击动画
 * @param s 目标状态（默认为false）
 * @param a 鼠标在区域内则点击无效（恐怕再次点击）
 */
void InteractiveButton::simulateStatePress(bool s, bool a)
{
    if (GetCheckedState() == s)
        return;

    // 鼠标悬浮在上方，有两种情况：
    // 1、点击按钮后触发，重复了
    if (a && inArea(mapFromGlobal(QCursor::pos()))) // 点击当前按钮，不需要再模拟了
        return;

    mousePressEvent(new QMouseEvent(QMouseEvent::Type::None, QPoint(width() / 2, height() / 2), Qt::LeftButton, Qt::NoButton, Qt::NoModifier));

    mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::None, QPoint(width() / 2, height() / 2), Qt::LeftButton, Qt::NoButton, Qt::NoModifier));

    // if (!inArea(mapFromGlobal(QCursor::pos()))) // 针对模拟release 后面 // 必定成立
    m_isHovering = false;
}

/**
 * 模拟鼠标悬浮的效果
 * 适用于键盘操作时，模拟鼠标hover状态
 * 用 discardHoverPress 取消状态
 */
void InteractiveButton::simulateHover()
{
    if (!m_isHovering)
    {
        if (_block_hover)
            setBlockHover(false); // 可能已经临时屏蔽掉鼠标 enter 事件，强制hover
        enterEvent(Q_NULLPTR);
    }
}

/**
 * 强制丢弃hover、press状态
 * 适用于悬浮/点击后，弹出模态浮窗
 * 浮窗关闭后调用此方法
 * @param force 如果鼠标仍在此按钮内，是否强制取消hover/press状态
 */
void InteractiveButton::discardHoverPress(bool force)
{
    if (!force && inArea(mapFromGlobal(QCursor::pos()))) // 鼠标还在这范围内
        return;

    if (m_isOnPressing)
    {
        mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::None, QPoint(width() / 2, height() / 2), Qt::LeftButton, Qt::NoButton, Qt::NoModifier));
    }
}

/**
 * 鼠标移入事件，触发 hover 时间戳
 */
void InteractiveButton::enterEvent(QEvent *event)
{
    if (_block_hover) // 临时屏蔽hover事件
    {
        if (event)
            event->accept();
        return;
    }

    if (!anchor_timer->isActive())
    {
        anchor_timer->start();
    }
    m_isHovering = true;
    hover_timestamp = getTimestamp();
    leave_timestamp = 0;
    if (mouse_pos == QPoint(-1, -1))
        mouse_pos = mapFromGlobal(QCursor::pos());
    Q_EMIT ToMouseEnter();

    return QPushButton::enterEvent(event);
}

/**
 * 鼠标移开事件，触发 leave 时间戳
 */
void InteractiveButton::leaveEvent(QEvent *event)
{
    m_isHovering = false;
    if (!m_isOnPressing)
        mouse_pos = QPoint(width() / 2, height() / 2);
    Q_EMIT ToMouseLeave();

    return QPushButton::leaveEvent(event);
}

/**
 * 鼠标按下事件，触发 press 时间戳
 * 添加水波纹动画 waters 队列
 */
void InteractiveButton::mousePressEvent(QMouseEvent *event)
{
    mouse_pos = event->pos();

    if (event->button() == Qt::LeftButton)
    {
        if (!m_isHovering)
            InteractiveButton::enterEvent(new QEvent(QEvent::Type::None));

        m_isOnPressing = true;
        press_pos = mouse_pos;
        // 判断双击事件
        if (m_hasDoubleClicked)
        {
            qint64 last_press_timestamp = press_timestamp;
            press_timestamp = getTimestamp();
            if (release_timestamp + DOUBLE_PRESS_INTERVAL >= press_timestamp && last_press_timestamp + SINGLE_PRESS_INTERVAL > release_timestamp && release_pos == press_pos) // 是双击(判断两次单击的间隔)
            {
                m_doubleIsPrevented = true; // 阻止本次的release识别为单击
                press_timestamp = 0;   // 避免很可能出现的三击、四击...
                m_doubleClickTimer->stop();  // 取消延迟一小会儿的单击信号
                Q_EMIT ToDoubleClicked();
                return;
            }
            else
            {
                m_doubleIsPrevented = false; // 避免有额外的 bug
            }
        }
        else
        {
            press_timestamp = getTimestamp();
        }

        if (m_hasWaterAnimation)
        {
            if (!waters.isEmpty() && waters.constLast().m_releaseTimestamp == 0) // 避免两个按键同时按下
            {
                waters.last().m_releaseTimestamp = getTimestamp();
            }
            waters << Water(press_pos, press_timestamp);
        }
        else // 透明渐变
        {
            if (m_pressProgress < press_start)
                m_pressProgress = press_start; // 直接设置为按下效果初始值（避免按下反应慢）
        }
    }
    m_mousePressEventPointer = event;
    Q_EMIT ToMousePress(event);

    return QPushButton::mousePressEvent(event);
}

/**
 * 鼠标松开事件，触发 release 时间戳
 * 添加抖动动画 jitters 队列
 */
void InteractiveButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_isOnPressing && event->button() == Qt::LeftButton)
    {
        if (!inArea(event->pos()) || m_leaveAfterClick)
        {
            m_isHovering = false;
        }
        m_isOnPressing = false;
        release_pos = event->pos();
        release_timestamp = getTimestamp();

        // 添加抖动效果
        if (jitter_animation)
        {
            setJitter();
        }

        if (m_hasWaterAnimation && waters.size())
        {
            waters.last().m_releaseTimestamp = release_timestamp;
        }

        if (m_hasDoubleClicked)
        {
            if (m_doubleIsPrevented) // 双击的当次release，不参与单击计算
            {
                m_doubleIsPrevented = false;
                return;
            }

            // 应该不是双击的操作
            if (release_pos != press_pos || release_timestamp - press_timestamp >= SINGLE_PRESS_INTERVAL)
            {
            }
            else // 可能是双击，准备
            {
                m_doubleClickTimer->start();
                return; // 禁止单击事件
            }
        }
    }
    else if (m_leaveAfterClick && !m_isOnPressing && m_hasDoubleClicked && m_doubleIsPrevented) // 双击，失去焦点了，pressing 丢失
    {
        return;
    }
    else if (event->button() == Qt::RightButton && event->buttons() == Qt::NoButton)
    {
        if ((release_pos - press_pos).manhattanLength() < QApplication::startDragDistance())
            Q_EMIT ToRightClicked();
    }
    m_mouseReleaseEventPointer = event;
    Q_EMIT ToMouseRelease(event);

    return QPushButton::mouseReleaseEvent(event);
}

/**
 * 鼠标移动事件
 */
void InteractiveButton::mouseMoveEvent(QMouseEvent *event)
{
    if (_block_hover) // 临时屏蔽hover事件
    {
        if (event)
            event->accept();
        return;
    }
    if (m_isHovering == false) // 失去焦点又回来了
    {
        enterEvent(Q_NULLPTR);
    }
    mouse_pos = mapFromGlobal(QCursor::pos());

    return QPushButton::mouseMoveEvent(event);
}

/**
 * 尺寸大小改变事件
 * 同步调整和尺寸有关的所有属性
 */
void InteractiveButton::resizeEvent(QResizeEvent *event)
{
    if (!m_isOnPressing && !m_isHovering)
    {
        mouse_pos = QPoint(width() / 2, height() / 2);
        anchor_pos = mouse_pos;
    }
    m_waterRadius = static_cast<int>(max(width(), height()) * 1.42); // 长边
    // 非固定的情况，尺寸大小变了之后所有 padding 都要变
    if (!fixed_fore_size && (m_paintModel == PaintModel::Icon || m_paintModel == PaintModel::PixmapMask))
    {
        int short_side = min(width(), height());        // 短边
        int padding = int(short_side * icon_padding_proper); //static_cast<int>(short_side * (1 - GOLDEN_RATIO) / 2);
        fore_paddings.left = fore_paddings.top = fore_paddings.right = fore_paddings.bottom = padding;
    }
    _l = _t = 0;
    _w = width();
    _h = height();

    return QPushButton::resizeEvent(event);
}

/**
 * 获得焦点事件
 * 已经取消按钮获取焦点，focusIn和focusOut事件都不会触发
 */
void InteractiveButton::focusInEvent(QFocusEvent *event)
{
    if (!m_isHovering && inArea(mapFromGlobal(QCursor::pos())))
        InteractiveButton::enterEvent(new QEvent(QEvent::Type::None));

    focusing = true;
    Q_EMIT ToFocusIn();

    return QPushButton::focusInEvent(event);
}

/**
 * 失去焦点事件
 * 兼容按住时突然失去焦点（例如弹出菜单、被其他窗口抢走了）
 */
void InteractiveButton::focusOutEvent(QFocusEvent *event)
{
    if (m_isHovering)
    {
        m_isHovering = false;
    }
    if (m_isOnPressing) // 鼠标一直按住，可能在click事件中移动了焦点
    {
        m_isOnPressing = false;
        release_pos = mapFromGlobal(QCursor::pos());
        release_timestamp = getTimestamp();

        if (m_hasWaterAnimation && waters.size())
        {
            waters.last().m_releaseTimestamp = release_timestamp;
        }
    }

    focusing = false;
    Q_EMIT ToFocusOut();

    return QPushButton::focusOutEvent(event);
}


void InteractiveButton::paintEvent(QPaintEvent *event)
{
    if (parent_enabled)
    {
        QPushButton::paintEvent(event);
    }
    if (!self_enabled)
    {
        return;
    }
    QPainter painter(this);


    QPainterPath pathBack = GetBackGroundPainterPath();
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (normal_bg.alpha() != 0) //
    {
        painter.fillPath(pathBack, isEnabled() ? normal_bg : getOpacityColor(normal_bg));
    }
    if (focusing && focus_bg.alpha() != 0) // 焦点背景
    {
        painter.fillPath(pathBack, focus_bg);
    }

    if ((border_bg.alpha() != 0 || (focusing && focus_border.alpha() != 0)) && m_borderWidth > 0)
    {
        painter.save();
        QPen pen;
        pen.setColor((focusing && focus_border.alpha()) ? focus_border : border_bg);
        pen.setWidth(m_borderWidth);
        painter.setPen(pen);
        painter.drawPath(pathBack);
        painter.restore();
    }

    if (m_hoverProgress) // 悬浮背景
    {
        painter.fillPath(pathBack, getOpacityColor(hover_bg, m_hoverProgress / 100.0));
    }

    if (m_pressProgress && !m_hasWaterAnimation) // 按下渐变淡化消失
    {
        painter.fillPath(pathBack, getOpacityColor(press_bg, m_pressProgress / 100.0));
    }
    else if (m_hasWaterAnimation && waters.size()) // 水波纹，且至少有一个水波纹
    {
        paintWaterRipple(painter);
    }

    // ==== 绘制前景 ====
    if (fore_enabled && isShowForeground)
    {
        painter.setPen(isEnabled() ? m_iconColor : getOpacityColor(m_iconColor));
        if (paint_addin.enable)
        {
            int l = fore_paddings.left, t = fore_paddings.top, r = width() - fore_paddings.right, b = height() - fore_paddings.bottom;
            int small_edge = min(height(), width());
            int pw = paint_addin.size.width() ? paint_addin.size.width() : small_edge * 4 / 5;
            int ph = paint_addin.size.height() ? paint_addin.size.height() : small_edge * 4 / 5;
            if (paint_addin.align & Qt::AlignLeft)
                r = l + pw;
            else if (paint_addin.align & Qt::AlignRight)
                l = r - pw;
            else if (paint_addin.align & Qt::AlignHCenter)
            {
                l = width() / 2 - pw / 2;
                r = l + pw;
            }
            if (paint_addin.align & Qt::AlignTop)
                b = t + ph;
            else if (paint_addin.align & Qt::AlignBottom)
                t = b - ph;
            else if (paint_addin.align & Qt::AlignVCenter)
            {
                t = height() / 2 - ph / 2;
                b = t + ph;
            }
            painter.drawPixmap(QRect(l, t, r - l, b - t), paint_addin.pixmap);
        }

        QRectF &rect = paint_rect;
        rect = QRectF(fore_paddings.left + (fixed_fore_pos ? 0 : offset_pos.x()),
                      fore_paddings.top + (fixed_fore_pos ? 0 : offset_pos.y()), // 原来的位置，不包含点击、出现效果
                      width() - fore_paddings.left - fore_paddings.right,
                      height() - fore_paddings.top - fore_paddings.bottom);

        // 抖动出现动画
        if ((m_isShowAppearAnimation || show_ani_disappearing) && show_ani_point != QPoint(0, 0) && !fixed_fore_pos)
        {
            //int w = width(), h = height();
            int pro = getSpringBackProgress(show_ani_progress, 50);

            // show_ani_point 是鼠标进入的点，那么起始方向应该是相反的
            double x = show_ani_point.x(), y = show_ani_point.y();
            int gen = quick_sqrt(long(x * x + y * y));
            x = m_waterRadius * x / gen; // 动画起始中心点横坐标 反向
            y = m_waterRadius * y / gen; // 动画起始中心点纵坐标 反向

            rect = QRectF(
                       rect.left() - x * (100 - pro) / 100 + rect.width() * (100 - pro) / 100,
                       rect.top() - y * (100 - pro) / 100 + rect.height() * (100 - pro) / 100,
                       rect.width() * pro / 100,
                       rect.height() * pro / 100);
        }
        else if (m_aligment == Qt::AlignCenter && m_paintModel != PaintModel::Text && !fixed_fore_size) // 默认的缩放动画
        {
            double delta_x = 0, delta_y = 0;
            if (click_ani_progress != 0) // 图标缩放
            {
                delta_x = rect.width() * click_ani_progress / 400;
                delta_y = rect.height() * click_ani_progress / 400;
            }
            else if (m_isShowAppearAnimation)
            {
                double pro = getNolinearProg(show_ani_progress, NolinearType::SpringBack50);
                delta_x = static_cast<int>(rect.width() * (1 - pro));
                delta_y = static_cast<int>(rect.height() * (1 - pro));
            }
            else if (show_ani_disappearing)
            {
                double pro = 1 - getNolinearProg(show_ani_progress, NolinearType::SlowFaster);
                delta_x = rect.width() * pro;  // (100-show_ani_progress) / 100;
                delta_y = rect.height() * pro; // (100-show_ani_progress) / 100;
            }
            if (int(delta_x + 1e-6) || int(delta_y + 1e-6))
                rect = QRectF(rect.left() + delta_x, rect.top() + delta_y,
                              rect.width() - delta_x * 2, rect.height() - delta_y * 2);
        }


        if (m_paintModel == PaintModel::Text)
        {
            painter.setPen(isEnabled() ? text_color : getOpacityColor(text_color));
            if (m_fontSize > 0)
            {
                QFont font = painter.font();
                font.setPointSize(m_fontSize);
                painter.setFont(font);
            }
            painter.drawText(rect, static_cast<int>(m_aligment), m_buttonText);
        }
        else if (m_paintModel == PaintModel::Icon) // 绘制图标
        {
            icon.paint(&painter, rect.toRect(), m_aligment, getIconMode());
        }
        else if (m_paintModel == PaintModel::PixmapMask)
        {
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true); // 可以让边缘看起来平滑一些
            painter.drawPixmap(rect.toRect(), pixmap);
        }
        else if (m_paintModel == PaintModel::IconText || m_paintModel == PaintModel::PixmapText) // 强制左对齐；左图标中文字
        {
            // 绘制图标
            int &sz = m_iconTextSize;
            QRectF icon_rect(rect.left(), rect.top() + rect.height() / 2 - sz / 2, sz, sz);
            icon_rect.moveTo(icon_rect.left() - quick_sqrt(long(offset_pos.x())), icon_rect.top() - quick_sqrt(long(offset_pos.y())));
            drawIconBeforeText(painter, icon_rect.toRect());
            rect.setLeft(rect.left() + sz + icon_text_padding);

            // 绘制文字
            // 扩展文字范围，确保文字可见
            painter.setPen(isEnabled() ? text_color : getOpacityColor(text_color));
            rect.setWidth(rect.width() + sz + icon_text_padding);
            if (m_fontSize > 0)
            {
                QFont font = painter.font();
                font.setPointSize(m_fontSize);
                painter.setFont(font);
            }
            painter.drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, m_buttonText);
        }
    }
}

void InteractiveButton::drawIconBeforeText(QPainter &painter, QRect icon_rect)
{
    if (m_paintModel == PaintModel::IconText)
        icon.paint(&painter, icon_rect, m_aligment, getIconMode());
    else if (m_paintModel == PaintModel::PixmapText)
        painter.drawPixmap(icon_rect, pixmap);
}


bool InteractiveButton::inArea(QPoint point)
{
    return !(point.x() < 0 || point.y() < 0 || point.x() > width() || point.y() > height());
}

bool InteractiveButton::inArea(QPointF point)
{
    return !(point.x() < 0 || point.y() < 0 || point.x() > width() || point.y() > height());
}

QPainterPath InteractiveButton::GetBackGroundPainterPath()
{
    QPainterPath path;
    if (m_radiusX || m_radiusY)
        path.addRoundedRect(QRect(0, 0, width(), height()), m_radiusX, m_radiusY);
    else
        path.addRect(QRect(0, 0, width(), height()));
    return path;
}

QPainterPath InteractiveButton::getWaterPainterPath(InteractiveButton::Water water)
{
    double prog = getNolinearProg(water.progress, NolinearType::FastSlower);
    double ra = m_waterRadius * prog;
    QRectF circle(water.point.x() - ra,
                  water.point.y() - ra,
                  ra * 2,
                  ra * 2);
    /*QRect circle(water.point.x() - water_radius*water.progress/100,
                water.point.y() - water_radius*water.progress/100,
                water_radius*water.progress/50,
                water_radius*water.progress/50);*/
    QPainterPath path;
    path.addEllipse(circle);
    if (m_radiusX || m_radiusY)
        return path & GetBackGroundPainterPath();
    return path;
}

QRectF InteractiveButton::getUnifiedGeometry()
{
    // 将动画进度转换为回弹动画进度
    int pro = m_isShowAppearAnimation ? getSpringBackProgress(show_ani_progress, 50) : show_ani_progress;
    double ul = 0, ut = 0, uw = width(), uh = height();

    // show_ani_point 是鼠标进入的点，那么起始方向应该是相反的
    double x = show_ani_point.x(), y = show_ani_point.y();
    int gen = quick_sqrt(long(x * x + y * y));
    x = -m_waterRadius * x / gen; // 动画起始中心点横坐标 反向
    y = -m_waterRadius * y / gen; // 动画起始中心点纵坐标 反向

    ul = ul + x * (100 - pro) / 100 + uw * (100 - pro) / 200;
    ut = ut + y * (100 - pro) / 100 + uh * (100 - pro) / 200;
    uw = uw * pro / 100;
    uh = uh * pro / 100;

    return QRectF(ul, ut, uw, uh);
}

/**
 * 更新统一绘制区域
 * 内部的 _l, _t, _w, _h 可直接使用
 */
void InteractiveButton::updateUnifiedGeometry()
{
    _l = 0;
    _t = 0;
    _w = width();
    _h = height();
    if ((m_isShowAppearAnimation || show_ani_disappearing) && show_ani_point != QPoint(0, 0))
    {
        int pro; // 将动画进度转换为回弹动画进度
        pro = m_isShowAppearAnimation ? getSpringBackProgress(show_ani_progress, 50) : show_ani_progress;

        // show_ani_point 是鼠标进入的点，那么起始方向应该是相反的
        double x = show_ani_point.x(), y = show_ani_point.y();
        int gen = quick_sqrt(long(x * x + y * y));
        x = -m_waterRadius * x / gen; // 动画起始中心点横坐标 反向
        y = -m_waterRadius * y / gen; // 动画起始中心点纵坐标 反向

        _l = _l + x * (100 - pro) / 100 + _w * (100 - pro) / 200;
        _t = _t + y * (100 - pro) / 100 + _h * (100 - pro) / 200;
        _w = _w * pro / 100;
        _h = _h * pro / 100;
    }
}

/**
 * 绘制一个水波纹动画
 * @param painter 绘制对象（即painter(this)对象）
 */
void InteractiveButton::paintWaterRipple(QPainter &painter)
{
    QColor water_finished_color(press_bg);

    for (int i = 0; i < waters.size(); i++)
    {
        Water water = waters.at(i);
        if (water.m_isFinished) // 渐变消失
        {
            water_finished_color.setAlpha(press_bg.alpha() * water.progress / 100);
            QPainterPath path_back = GetBackGroundPainterPath();
            //                painter.setPen(water_finished_color);
            painter.fillPath(path_back, QBrush(water_finished_color));
        }
        else // 圆形出现
        {
            QPainterPath path = getWaterPainterPath(water);
            painter.fillPath(path, QBrush(press_bg));
        }
    }
}

/**
 * 鼠标松开的时候，计算所有抖动效果的路径和事件
 * 在每次重绘界面的时候，依次遍历所有的路径
 */
void InteractiveButton::setJitter()
{
    jitters.clear();
    QPoint center_pos = geometry().center() - geometry().topLeft();
    double full_manh = (anchor_pos - center_pos).manhattanLength(); // 距离
    // 是否达到需要抖动的距离
    if (full_manh > (geometry().topLeft() - geometry().bottomRight()).manhattanLength()) // 距离超过外接圆半径，开启抖动
    {
        QPointF jitter_pos(effect_pos);
        full_manh = (jitter_pos - center_pos).manhattanLength();
        double manh = full_manh;
        int duration = jitter_duration;
        qint64 timestamp = release_timestamp;
        while (manh > elastic_coefficient)
        {
            jitters << Jitter(jitter_pos, timestamp);
            jitter_pos = center_pos - (jitter_pos - center_pos) / elastic_coefficient;
            duration = int(jitter_duration * manh / full_manh);
            timestamp += duration;
            manh = static_cast<int>(manh / elastic_coefficient);
        }
        jitters << Jitter(center_pos, timestamp);
        anchor_pos = mouse_pos = center_pos;
    }
    else if (!m_isHovering) // 悬浮的时候依旧有效
    {
        // 未达到抖动距离，直接恢复
        mouse_pos = center_pos;
    }
}

/**
 * 速度极快的开方算法，效率未知，原理未知
 * @param  X 待开方的数字
 * @return   平方根
 */
int InteractiveButton::quick_sqrt(long X) const
{
    bool fu = false;
    if (X < 0)
    {
        fu = true;
        X = -X;
    }
#if !defined(Q_OS_WIN)
    X = qSqrt(X);
    return fu ? -X : X;
#endif
    unsigned long M = static_cast<unsigned long>(X);
    unsigned int N, i;
    unsigned long tmp, ttp; // 结果、循环计数
    if (M == 0)             // 被开方数，开方结果也为0
        return 0;
    N = 0;
    tmp = (M >> 30); // 获取最高位：B[m-1]
    M <<= 2;
    if (tmp > 1) // 最高位为1
    {
        N++; // 结果当前位为1，否则为默认的0
        tmp -= N;
    }
    for (i = 15; i > 0; i--) // 求剩余的15位
    {
        N <<= 1; // 左移一位
        tmp <<= 2;
        tmp += (M >> 30); // 假设
        ttp = N;
        ttp = (ttp << 1) + 1;
        M <<= 2;
        if (tmp >= ttp) // 假设成立
        {
            tmp -= ttp;
            N++;
        }
    }
    return (fu ? -1 : 1) * static_cast<int>(N); // 不知道为什么计算出来的结果是反过来的
}

/**
 * 最大值
 */
int InteractiveButton::max(int a, int b) const
{
    return a > b ? a : b;
}

/**
 * 最小值
 */
int InteractiveButton::min(int a, int b) const
{
    return a < b ? a : b;
}

/**
 * 获取现行时间戳，13位，精确到毫秒
 * @return 时间戳
 */
qint64 InteractiveButton::getTimestamp() const
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

/**
 * 是否为亮色颜色
 * @param  color 颜色
 * @return       是否为亮色
 */
bool InteractiveButton::isLightColor(QColor color)
{
    return color.red() * 0.299 + color.green() * 0.578 + color.blue() * 0.114 >= 192;
}

/**
 * 获取非线性动画在某一时间比例的动画进度
 * 仅适用于弹过头效果的动画
 * @param  x   实际相对完整100%的动画进度
 * @param  max 前半部分动画进度上限
 * @return     应当显示的动画进度
 */
int InteractiveButton::getSpringBackProgress(int x, int max)
{
    if (x <= max)
        return x * 100 / max;
    if (x <= max + (100 - max) / 2)
        return (x - max) / 2 + 100;
    return 100 + (100 - x) / 2;
}

QColor InteractiveButton::getOpacityColor(QColor color, double level)
{
    color.setAlpha(static_cast<int>(color.alpha() * level));
    return color;
}

QPixmap InteractiveButton::getMaskPixmap(QPixmap p, QColor c)
{
    QBitmap mask = p.mask();
    p.fill(c);
    p.setMask(mask);
    return p;
}

double InteractiveButton::getNolinearProg(int p, InteractiveButton::NolinearType type)
{
    if (p <= 0)
        return 0.0;
    if (p >= 100)
        return 1.0;

    switch (type)
    {
    case NolinearType::Linear:
        return p / 100.0;
    case NolinearType::SlowFaster:
        return p * p / 10000.0;
    case NolinearType::FastSlower:
        return quick_sqrt(p * 100) / 100.0;
    case NolinearType::SlowFastSlower:
        if (p <= 50)
            return p * p / 50.0;
        else
            return 0.5 + quick_sqrt(50 * (p - 50)) / 100.0;
    case NolinearType::SpringBack20:
    case NolinearType::SpringBack50:
        if (p <= 50)
            return p / 50.0;
        else if (p < 75)
            return 1.0 + (p - 50) / 200.0;
        else
            return 1.0 + (100 - p) / 200.0;
    }
}

QIcon::Mode InteractiveButton::getIconMode()
{
    return isEnabled() ? (GetCheckedState() ? QIcon::Selected : (m_isHovering || m_isOnPressing ? QIcon::Active : QIcon::Normal)) : QIcon::Disabled;
}

/**
* Точка привязки становится синхронизирующими часами с положением мыши
  * Одновременно рассчитывайте все анимации и свойства, связанные со временем или частотой кадров
 */
void InteractiveButton::OnAnchorTimeOut()
{
    const qint64 timestamp = getTimestamp();
    if (m_isOnPressing) // 鼠标按下
    {
        if (m_pressProgress < 100) // 透明渐变，且没有完成
        {
            m_pressProgress += m_pressSpeed;
            if (m_pressProgress >= 100)
            {
                m_pressProgress = 100;
                if (m_mousePressEventPointer)
                {
                    Q_EMIT ToMousePressLater(m_mousePressEventPointer);
                    m_mousePressEventPointer = Q_NULLPTR;
                }
            }
        }
        if (m_isHovering && m_hoverProgress < 100)
        {
            m_hoverProgress += m_hoverSpeed;
            if (m_hoverProgress >= 100)
            {
                m_hoverProgress = 100;
                Q_EMIT ToMouseEnterLater();
            }
        }
    }
    else // наведение мыши
    {
        if (m_pressProgress > 0) // 如果按下的效果还在，变浅
        {
            m_pressProgress -= m_pressSpeed;
            if (m_pressProgress <= 0)
            {
                m_pressProgress = 0;
                if (m_mouseReleaseEventPointer)
                {
                    Q_EMIT ToMouseReleaseLater(m_mouseReleaseEventPointer);
                    m_mouseReleaseEventPointer = Q_NULLPTR;
                }
            }
        }

        if (m_isHovering) // 在框内：加深
        {
            if (m_hoverProgress < 100)
            {
                m_hoverProgress += m_hoverSpeed;
                if (m_hoverProgress >= 100)
                {
                    m_hoverProgress = 100;
                    Q_EMIT ToMouseEnterLater();
                }
            }
        }
        else // 在框外：变浅
        {
            if (m_hoverProgress > 0)
            {
                m_hoverProgress -= m_hoverSpeed;
                if (m_hoverProgress <= 0)
                {
                    m_hoverProgress = 0;
                    Q_EMIT ToMouseLeaveLater();
                }
            }
        }
    }

    // ==== Нажмите на фоновую анимацию водной ряби ====
    if (m_hasWaterAnimation)
    {
        for (int i = 0; i < waters.size(); i++)
        {
            Water &water = waters[i];
            if (water.m_isFinished) // конечное состояние
            {
                water.progress = static_cast<int>(100 - 100 * (timestamp - water.m_finishTimestamp) / m_waterFinishDuration);
                if (water.progress <= 0)
                {
                    waters.removeAt(i--);
                    if (m_mouseReleaseEventPointer) // Сигнал задержки нажатия не отправлен
                    {
                        Q_EMIT ToMouseReleaseLater(m_mouseReleaseEventPointer);
                        m_mouseReleaseEventPointer = Q_NULLPTR;
                    }
                }
            }
            else // 正在出现状态
            {
                if (water.progress >= 100) // 满了
                {
                    water.progress = 100;
                    if (water.m_releaseTimestamp) // 鼠标已经松开了
                    {
                        water.m_isFinished = true; // 准备结束
                        water.m_finishTimestamp = timestamp;
                    }
                }
                else // в анимации
                {
                    if (water.m_releaseTimestamp) // Мышь была отпущена
                    {
                        water.progress = static_cast<int>(100 * (water.m_releaseTimestamp - water.m_pressTimestamp) / m_waterPressDuration + 100 * (timestamp - water.m_releaseTimestamp) / m_waterReleaseDuration);
                    }
                    else // мышь нажата
                    {
                        water.progress = static_cast<int>(100 * (timestamp - water.m_pressTimestamp) / m_waterPressDuration);
                    }
                    if (water.progress >= 100)
                    {
                        water.progress = 100;
                        if (m_mousePressEventPointer) // Сигнал задержки нажатия не отправлен
                        {
                            Q_EMIT ToMousePressLater(m_mousePressEventPointer);
                            m_mousePressEventPointer = Q_NULLPTR;
                        }
                    }
                }
            }
        }
    }

    // ==== Появляется анимация ====
    if (isShowAnimation)
    {
        if (m_isShowAppearAnimation) // Появляться
        {
            const qint64 animationDurationDelta = getTimestamp() - show_timestamp;
            if (show_ani_progress >= 100) // конец
            {
                m_isShowAppearAnimation = false;
                Q_EMIT ToShowAnimationFinished();
            }
            else
            {
                show_ani_progress = static_cast<int>(100 * animationDurationDelta / show_duration);
                if (show_ani_progress > 100)
                {
                    show_ani_progress = 100;
                }
            }
        }
        if (show_ani_disappearing) //пропадать
        {
            const qint64 delta = getTimestamp() - hide_timestamp;
            if (show_ani_progress <= 0) // 消失结束
            {
                show_ani_disappearing = false;
                isShowForeground = false;
                show_ani_point = QPoint(0, 0);
                Q_EMIT ToHideAnimationFinished();
            }
            else
            {
                show_ani_progress = static_cast<int>(100 - 100 * delta / show_duration);
                if (show_ani_progress < 0)
                {
                    show_ani_progress = 0;
                }
            }
        }
    }

    // ==== Нажмите анимацию ====
    if (click_ani_disappearing) // Эффект анимации щелчка исчезает
    {
        const qint64 delta = getTimestamp() - release_timestamp - click_ani_duration;
        if (delta <= 0)
        {
            click_ani_progress = 100;
        }
        else
        {
            click_ani_progress = static_cast<int>(100 - delta * 100 / click_ani_duration);
        }
        if (click_ani_progress < 0)
        {
            click_ani_progress = 0;
            click_ani_disappearing = false;
            Q_EMIT ToPressAppearAnimationFinished();
        }
    }
    if (click_ani_appearing) // нажмите анимацию
    {
        const qint64 delta = getTimestamp() - release_timestamp;
        if (delta <= 0)
        {
            click_ani_progress = 0;
        }
        else
        {
            click_ani_progress = static_cast<int>(delta * 100 / click_ani_duration);
        }
        if (click_ani_progress > 100)
        {
            click_ani_progress = 100; // Сохраняйте состояние 100 и возвращайтесь к 0 при следующем щелчке
            click_ani_appearing = false;
            click_ani_disappearing = true;
            Q_EMIT ToPressDisappearAnimationFinished();
        }
    }

    //==== Перемещение якоря ====
    if (jitters.size() > 0) //Эффект дрожания при отпускании
    {
        // должно быть в последней точке
        const Jitter cur = jitters.first();
        const Jitter aim = jitters.at(1);
        const int del = static_cast<int>(getTimestamp() - cur.timestamp);
        const int dur = static_cast<int>(aim.timestamp - cur.timestamp);
        effect_pos = cur.point + (aim.point - cur.point) * del / dur;
        offset_pos = effect_pos - (geometry().center() - geometry().topLeft());

        if (del >= dur)
        {
            jitters.removeFirst();
        }

        // Джиттер заканчивается
        if (jitters.size() == 1)
        {
            jitters.clear();
            Q_EMIT ToJitterAnimationFinished();
        }
    }
    else if (anchor_pos != mouse_pos) // движущийся эффект
    {
        const double delta_x = anchor_pos.x() - mouse_pos.x(),
                     delta_y = anchor_pos.y() - mouse_pos.y();

        anchor_pos.setX(anchor_pos.x() - quick_sqrt(int(delta_x)));
        anchor_pos.setY(anchor_pos.y() - quick_sqrt(int(delta_y)));

        offset_pos.setX(quick_sqrt(long(anchor_pos.x() - (width() >> 1))));
        offset_pos.setY(quick_sqrt(long(anchor_pos.y() - (height() >> 1))));
        effect_pos.setX((width() >> 1) + offset_pos.x());
        effect_pos.setY((height() >> 1) + offset_pos.y());
    }
    else
    {
        if (!m_isOnPressing && !m_isHovering && !m_hoverProgress && !m_pressProgress && !click_ani_appearing && !click_ani_disappearing && !jitters.size() && !waters.size() && !m_isShowAppearAnimation && !show_ani_disappearing) // 没有需要加载的项，暂停（节约资源）
        {
            anchor_timer->stop();
        }
    }
    // ==== Анимация появления юниформ-координат ====
    if (unified_geometry)
    {
        updateUnifiedGeometry();
    }

    update();
}

void InteractiveButton::OnClicked()
{
    click_ani_appearing = true;
    click_ani_disappearing = false;
    click_ani_progress = 0;
    release_offset = offset_pos;

    jitters.clear(); // убрать дрожание
}

void InteractiveButton::OnCloseState()
{
    setState(false);
}
