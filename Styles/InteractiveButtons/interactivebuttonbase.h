#ifndef STYLES_INTERACTIVEBUTTONS_INTERACTIVEBUTTONBASE_H
#define STYLES_INTERACTIVEBUTTONS_INTERACTIVEBUTTONBASE_H

#include <QObject>
#include <QApplication>
#include <QPushButton>
#include <QPoint>
#include <QTimer>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include <QList>
#include <QBitmap>
#include <QtMath>

#define GOLDEN_RATIO 0.618

#define DOUBLE_PRESS_INTERVAL 300
#define SINGLE_PRESS_INTERVAL 200

class InteractiveButton : public QPushButton
{
    Q_OBJECT
public:
    InteractiveButton(QWidget *parent);
    InteractiveButton(const QString &m_buttonText, QWidget *parent);
    InteractiveButton(const QIcon &icon, QWidget *parent);
    InteractiveButton(const QPixmap &pixmap, QWidget *parent);
    InteractiveButton(const QIcon &icon, const  QString &m_buttonText, QWidget *parent);
    InteractiveButton(const QPixmap &pixmap, const  QString &m_buttonText, QWidget *parent);
    virtual ~InteractiveButton();
    /**
     * объект переднего плана
     */
    enum class PaintModel
    {
        None,       // нет переднего плана, только фон
        Text,       // обычный текст (заменяет родительский класс)
        Icon,
        PixmapMask, // Иконки, меняющие цвет (реализовано через pixmap+mask), с явным алиасингом
        IconText,   // Значок + текст (принудительное выравнивание по левому краю)
        PixmapText  // Меняющий цвет значок + текст (принудительное выравнивание по левому краю)
    };

    /**
     * Дополнительные значки переднего плана (можно несколько)
     * * может быть угловым маркером (например, стрелкой расширения)
     * * может быть префиксом
     */
    struct PaintAddin
    {
        PaintAddin() : enable(false) {}
        PaintAddin(QPixmap p, Qt::Alignment a, QSize s) : enable(true), pixmap(p), align(a), size(s) {}
        bool enable;         // 是否启用
        QPixmap pixmap;      // 可变色图标
        Qt::Alignment align; // 对齐方式
        QSize size;          // 固定大小
    };

    /**
     * 鼠标松开时抖动动画
     * 松开的时候计算每一次抖动距离+时间，放入队列中
     * 定时调整抖动的队列实体索引
     */
    struct Jitter
    {
        Jitter(QPointF p, qint64 t) : point(p), timestamp(t) {}
        QPointF point;     // целевая координата для перемещения
        qint64 timestamp; // Перейдите к отметке времени целевой координаты, удалите объект пути дрожания после окончания
    };

    /**
     * 鼠标按下/弹起水波纹动画
     * 鼠标按下时动画速度慢（压住），松开后动画速度骤然加快
     * 同样用队列记录所有的水波纹动画实体
     */
    struct Water
    {
        Water(QPointF p, qint64 t)
            : point(p)
            , progress(0)
            , m_pressTimestamp(t)
            , m_releaseTimestamp(0)
            , m_finishTimestamp(0)
            , m_isFinished(false)
        {

        }
        ~Water()
        {

        }

        QPointF point;
        int progress;             // 水波纹进度100%（已弃用，当前使用时间戳）
        qint64 m_pressTimestamp;   // отметка времени наведения мыши
        qint64 m_releaseTimestamp; // Отметка времени выпуска мыши.
        //Вместе с нажатой временной меткой и текущей временной меткой он становится параметром расчета прогресса водной ряби.
        qint64 m_finishTimestamp;  // Отметка времени окончания. Из текущей метки времени вычитается метка времени после исчезновения градиента.
        bool m_isFinished;            // кончится ли. После окончания градиент исчезает
    };

    /**
     * 四周边界的padding
     * 调整按钮大小时：宽度+左右、高度+上下
     */
    struct EdgeVal
    {
        EdgeVal() {}
        EdgeVal(int l, int t, int r, int b)
            : left(l)
            , top(t)
            , right(r)
            , bottom(b)
        {

        }
        ~EdgeVal()
        {

        }
        int left, top, right, bottom; // расстояние для четырех границ
    };

    enum class NolinearType
    {
        Linear,
        SlowFaster,
        FastSlower,
        SlowFastSlower,
        SpringBack20,
        SpringBack50
    };

    virtual void SetText(const QString &buttonText);
    virtual void SetIcon(const QString &path);
    virtual void SetIcon(const QIcon &icon);
    virtual void setPixmapPath(const QString &path);
    virtual void SetPixmap(const QPixmap &pixmap);
    virtual void setPaintAddin(QPixmap pixmap, Qt::Alignment m_aligment = Qt::AlignRight, QSize size = QSize(0, 0));

    void setSelfEnabled(bool e = true);
    void setParentEnabled(bool e = false);
    void setForeEnabled(bool e = true);

    void setHoverAniDuration(int d);
    void setPressAniDuration(int d);
    void setClickAniDuration(int d);
    void setWaterAniDuration(int press, int release, int finish);
    void SetWaterRipple(bool enable = true);
    void setJitterAni(bool enable = true);
    void setUnifyGeomerey(bool enable = true);
    void SetBackgroundColor(const QColor &color);
    void SetBackgroundColor(const QColor &hover, const QColor &press);
    void SetNormalColor(const QColor &color);
    void SetBorderColor(const QColor &color);
    void SetHoverColor(const QColor &color);
    void SetPressColor(const QColor &color);
    void SetIconColor(const QColor &color = QColor(0, 0, 0));
    void SetTextColor(QColor color = QColor(0, 0, 0));
    void setFocusBg(QColor color);
    void setFocusBorder(QColor color);
    void SetFontSize(int newFontSize);
    void SetHover();
    void SetAlign(Qt::Alignment aligment);
    void SetRadius(int radius);
    void SetRadius(int radiusX, int radiusY);
    void setBorderWidth(int borderWidth);
    void SetDisabled(bool newState);
    void SetPaddings(int l, int r, int t, int b);
    void SetPaddings(int h, int v);
    void SetPaddings(int x);
    void SetIconPaddingProper(double x);
    void setFixedForePos(bool f = true);
    void setFixedForeSize(bool f = true, int addin = 0);
    void setSquareSize();
    void setFixedTextPos(bool f);
    void setTextDynamicSize(bool d = true);
    void setLeaveAfterClick(bool l = true);
    void setDoubleClicked(bool e = true);
    void setAutoTextColor(bool a = true);
    void setPretendFocus(bool f = true);
    void setBlockHover(bool b = true);

    void setShowAni(bool enable = true);
    void showForeground();
    void showForeground2(QPoint point = QPoint(0, 0));
    void hideForeground();
    void delayShowed(int time, QPoint point = QPoint(0, 0));

    const QString &GetText() const noexcept;
    void setMenu(QMenu *menu);
    void RegulateMinimumSize();
    void setState(bool s = true);
    bool GetCheckedState() const noexcept;
    virtual void simulateStatePress(bool s = true, bool a = false);
    bool HasIcon() const noexcept
    {
        return !m_iconPath.isEmpty();
    }
    bool isHovering()
    {
        return m_isHovering;
    }
    bool isPressing()
    {
        return m_isOnPressing;
    }
    void simulateHover();
    void discardHoverPress(bool force = false);

    bool getSelfEnabled()
    {
        return self_enabled;
    }
    bool getParentEnabled()
    {
        return parent_enabled;
    }
    bool getForeEnabled()
    {
        return fore_enabled;
    }
    QColor getIconColor()
    {
        return m_iconColor;
    }
    QColor getTextColor()
    {
        return text_color;
    }
    QColor getNormalColor()
    {
        return normal_bg;
    }
    QColor getBorderColor()
    {
        return border_bg;
    }
    QColor getHoverColor()
    {
        return hover_bg;
    }
    QColor getPressColor()
    {
        return press_bg;
    }
    const QString &GetIconPath() const noexcept
    {
        return m_iconPath;
    }
    int getHoverAniDuration()
    {
        return hover_bg_duration;
    }
    int getPressAniDuration()
    {
        return press_bg_duration;
    }
    int getClickAniDuration()
    {
        return click_ani_duration;
    }
    double getIconPaddingProper()
    {
        return icon_padding_proper;
    }
    int getRadius()
    {
        return qMax(m_radiusX, m_radiusY);
    }
    int getBorderWidth()
    {
        return m_borderWidth;
    }
    bool getFixedTextPos()
    {
        return fixed_fore_pos;
    }
    bool getTextDynamicSize()
    {
        return isTextDynamicSize;
    }
    bool getLeaveAfterClick()
    {
        return m_leaveAfterClick;
    }
    bool getShowAni()
    {
        return isShowAnimation;
    }
    bool getWaterRipple()
    {
        return m_hasWaterAnimation;
    }

    virtual bool inArea(QPoint point);
    virtual bool inArea(QPointF point);

protected:
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    virtual void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    virtual void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    virtual void changeEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    virtual QPainterPath GetBackGroundPainterPath();
    virtual QPainterPath getWaterPainterPath(Water water);
    virtual void drawIconBeforeText(QPainter &painter, QRect icon_rect);

    QRectF getUnifiedGeometry();
    void updateUnifiedGeometry();
    void paintWaterRipple(QPainter &painter);
    void setJitter();

    int GetFontSizeT();
    void SetFontSizeT(int m_fontSize);

    int max(int a, int b) const;
    int min(int a, int b) const;
    int quick_sqrt(long X) const;
    qint64 getTimestamp() const;
    bool isLightColor(QColor color);
    int getSpringBackProgress(int x, int max);
    QColor getOpacityColor(QColor color, double level = 0.5);
    QPixmap getMaskPixmap(QPixmap p, QColor c);

    double getNolinearProg(int p, NolinearType type);
    QIcon::Mode getIconMode();

Q_SIGNALS:
    void ToShowAnimationFinished();
    void ToHideAnimationFinished();
    void ToPressAppearAnimationFinished();
    void ToPressDisappearAnimationFinished();
    void ToJitterAnimationFinished();
    void ToDoubleClicked();
    void ToRightClicked();
    void ToFocusIn();
    void ToFocusOut();

    void ToMouseEnter();
    void ToMouseEnterLater(); // Задержка сигнала после входа (при условии завершения анимации градиента, что эквивалентно ручной настройке)
    void ToMouseLeave();
    void ToMouseLeaveLater(); // Задержка сигнала после ухода (до завершения анимации затухания (если сразу смахнуть, то это тоже будет быстрее))
    void ToMousePress(QMouseEvent *event);
    void ToMousePressLater(QMouseEvent *event);
    void ToMouseRelease(QMouseEvent *event);
    void ToMouseReleaseLater(QMouseEvent *event);

public Q_SLOTS:
    virtual void OnAnchorTimeOut();
    virtual void OnClicked();
    void OnCloseState();

protected:
    PaintModel m_paintModel;
    QIcon icon;
    QString m_iconPath;
    QString m_buttonText;
    QPixmap pixmap;
    PaintAddin paint_addin;
    EdgeVal fore_paddings;

protected:

    bool self_enabled, parent_enabled, fore_enabled; // 是否启用子类、启动父类、绘制子类前景

    // анимация с передним планом
    bool isShowAnimation, isShowForeground;
    bool m_isShowAppearAnimation, show_ani_disappearing;
    int show_duration;
    qint64 show_timestamp, hide_timestamp;
    int show_ani_progress;
    QPointF show_ani_point;
    QRectF paint_rect;

    // Координаты и метка времени, когда мышь начинает зависать, нажимать, отпускать и уходить.
    // Точка привязки мыши, целевая точка привязки, координаты текущей точки привязки, текущее смещение XY
    QPointF enter_pos, press_pos, release_pos, mouse_pos, anchor_pos /*Целевая точка привязки приближается к мыши*/;
    QPointF offset_pos /* текущее смещение */, effect_pos, release_offset;                // 相对中心、相对左上角、弹起时的平方根偏移
    bool m_isHovering, m_isOnPressing;                                                     // 是否悬浮和按下的状态机
    qint64 hover_timestamp, leave_timestamp, press_timestamp, release_timestamp; // 各种事件的时间戳
    int hover_bg_duration, press_bg_duration, click_ani_duration;                // 各种动画时长

    // Регулярно обновлять интерфейс (чтобы анимация продолжалась)
    QTimer *const anchor_timer;
    int move_speed;

    // 背景与前景
    QColor m_iconColor, text_color;                   // 前景颜色
    QColor normal_bg, hover_bg, press_bg, border_bg; // 各种背景颜色
    QColor focus_bg, focus_border;                   // 有焦点的颜色
    int m_hoverSpeed, press_start, m_pressSpeed;       // 颜色渐变速度
    int m_hoverProgress, m_pressProgress;              // 颜色渐变进度
    double icon_padding_proper;                      // 图标的大小比例
    int icon_text_padding, m_iconTextSize;           // 图标+文字模式共存时，两者间隔、图标大小
    int m_borderWidth;
    int m_radiusX, m_radiusY;
    int m_fontSize;
    bool fixed_fore_pos;    // 鼠标进入时是否固定文字位置
    bool fixed_fore_size;   // 鼠标进入/点击时是否固定前景大小
    bool isTextDynamicSize; // 设置字体时自动调整最小宽高
    bool auto_text_color;   // 动画时是否自动调整文字颜色
    bool focusing;          // 是否获得了焦点

    // анимация щелчка мышью
    bool click_ani_appearing, click_ani_disappearing; // 是否正在按下的动画效果中
    int click_ani_progress;                           // 按下的进度（使用时间差计算）
    QMouseEvent *m_mousePressEventPointer, *m_mouseReleaseEventPointer;

    // Область, в которой иконка отрисовывается равномерно (от всей кнопки до центра на две трети и рассчитывается по смещению)
    bool unified_geometry; // Если вышеперечисленное не работает, то и это тоже не работает...
    double _l, _t, _w, _h;

    // Эффект перетаскивания мышью и обратного дрожания
    bool jitter_animation;      // 是否开启鼠标松开时的抖动效果
    double elastic_coefficient; // 弹性系数
    QList<Jitter> jitters;
    int jitter_duration; // 抖动一次，多次效果叠加

    // Эффект анимации пульсации воды при нажатии мыши
    bool m_hasWaterAnimation; // Включить ли анимацию ряби на воде
    QList<Water> waters;
    int m_waterPressDuration, m_waterReleaseDuration, m_waterFinishDuration;
    int m_waterRadius;

    // другие эффекты
    Qt::Alignment m_aligment;      // текст/значок: ориентации
    bool _state;              // Переменная, которая записывает состояние, например, сохраняется ли оно
    bool m_leaveAfterClick; // После отпускания щелчка мыши эффект приостановки (для меню и всплывающих окон) отменяется, и кнопка должна терять фокус
    bool _block_hover;        // Если есть анимация, временно заблокируйте эффект зависания

    // Двойной клик
    bool m_hasDoubleClicked;  // Включить двойной щелчок
    QTimer *m_doubleClickTimer;
    bool m_doubleIsPrevented;  // запрещающий флаг
};

#endif // STYLES_INTERACTIVEBUTTONS_INTERACTIVEBUTTONBASE_H
