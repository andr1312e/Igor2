#include "globalshortcut.h"

ShortCut::GlobalShortcut::GlobalShortcut(QObject *parent)
    : QObject(parent)
    , m_privateGlobarShortcut(new ShortCut::GlobalShortcutPrivate())
{
    m_privateGlobarShortcut->m_display = QX11Info::display();
    m_privateGlobarShortcut->m_win = DefaultRootWindow(m_privateGlobarShortcut->m_display);
    m_privateGlobarShortcut->enabled = true;
    m_privateGlobarShortcut->InitHash();
    qApp->installNativeEventFilter(this);
}

ShortCut::GlobalShortcut::~GlobalShortcut()
{
    UnsetShortcut();
    qApp->removeNativeEventFilter(this);
    delete m_privateGlobarShortcut;
}
/**
 * ShortCut::GlobalShortcut::OnUnsetShortcut()
 *nativeEventFilter устанавливается на всё приложение.
 * При этом число фильтров в приложении не ограничено
 * Фильтр установленный последним, будет срабатывать первым.
 * Использовать данный класс можно в любом месте приложения.
 * Может влиять на производительность при наличии других nativeEventFilter
 */
bool ShortCut::GlobalShortcut::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    if (!m_privateGlobarShortcut->keys.isEmpty() && m_privateGlobarShortcut->enabled)
    {
        if (QLatin1Literal("xcb_generic_event_t") == eventType)
        {
            const xcb_generic_event_t *const event = static_cast<xcb_generic_event_t *>(message);
            if ((event->response_type & 127) == XCB_KEY_PRESS)
            {
                const xcb_key_press_event_t *const keyEvent = static_cast<xcb_key_press_event_t *>(message);
                for (const GlobalData *data : qAsConst(m_privateGlobarShortcut->listKeys))
                {
                    for (const quint32 maskMods :  m_privateGlobarShortcut->MaskModifiers())
                    {
                        if ((keyEvent->state == (data->GetModifier() | maskMods )) &&  keyEvent->detail == data->GetKey())
                        {
                            Q_EMIT ToActivate();
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
/**
 * QKeySequence ShortCut::GlobalShortcut::GetShortcut()
 * Метод возвращает установленную последовательность сочетаний горячих клавиш QKeySequence.
 * Если последовательность не установлена, то возвращается пустая последовательность QKeySequence("").
 */
QKeySequence ShortCut::GlobalShortcut::GetShortcut() const noexcept
{
    if (!m_privateGlobarShortcut->keys.isEmpty())
    {
        return m_privateGlobarShortcut->keys;
    }
    else
    {
        return QKeySequence("");
    }
}
/**
 * bool ShortCut::GlobalShortcut::IsEmpty() const noexcept
 * Метод проверяет, установлена ли последовательность горячих клавиш или нет.
 */
bool ShortCut::GlobalShortcut::IsEmpty() const noexcept
{
    return m_privateGlobarShortcut->keys.isEmpty();
}
/**
 * bool ShortCut::GlobalShortcut::IsEnabled() const noexcept
 * Метод проверяет, включён ли хоткей или нет.
 * Сочетание горячих клавиш может быть установлено, тогда isEmpty() будет возвращать true
 * Их срабатывание может быть выключено. То есть сигнал активации хоткея не будет генерироваться.
 */
bool ShortCut::GlobalShortcut::IsEnabled() const noexcept
{
    return m_privateGlobarShortcut->enabled;
}
/**
 * ShortCut::GlobalShortcut::OnUnsetShortcut()
 * Метод является слотом и устанавливает последовательность горячих клавиш.
 * Если QKeySequence содержит несколько сочетаний клавиш, то они все будут зарегистрированы.
 */
void ShortCut::GlobalShortcut::SetShortcut(const QKeySequence &keySequence)
{
    UnsetShortcut();
    m_privateGlobarShortcut->keys = keySequence;
    const QStringList list = m_privateGlobarShortcut->keys.toString().split(QLatin1Literal(", "));
    for (const QString str :  list)
    {
        GlobalData *data = new GlobalData();
        data->Setkey(m_privateGlobarShortcut->X11HotKey(m_privateGlobarShortcut->m_display, QKeySequence(str)));
        data->SetModifier(m_privateGlobarShortcut->X11KeyModificator(QKeySequence(str)));
        m_privateGlobarShortcut->listKeys.append(data);
        for (quint32 maskMods : m_privateGlobarShortcut->MaskModifiers())
        {
            XGrabKey(m_privateGlobarShortcut->m_display, data->GetKey(), data->GetModifier() | maskMods, m_privateGlobarShortcut->m_win, True, GrabModeAsync, GrabModeAsync);
        }
    }
}
/**
 * ShortCut::GlobalShortcut::OnUnsetShortcut()
 * Метод является слотом и удаляет сочетание горячих клавиш, если оно было установлено.
 */
void ShortCut::GlobalShortcut::UnsetShortcut()
{
    if (!m_privateGlobarShortcut->keys.isEmpty())
    {
        for (GlobalData *data : m_privateGlobarShortcut->listKeys)
        {
            foreach (quint32 maskMods, m_privateGlobarShortcut->MaskModifiers())
            {
                XUngrabKey(m_privateGlobarShortcut->m_display, data->GetKey(), data->GetModifier() | maskMods, m_privateGlobarShortcut->m_win);
            }
        }
        m_privateGlobarShortcut->listKeys.clear();
    }
}
/**
 * ShortCut::GlobalShortcut::OnUnsetShortcut()
 * Метод является слотом и служит для включения и выключения активации сочетания горячих клавиш.
 */
void ShortCut::GlobalShortcut::SetEnabled(bool enable)
{
    m_privateGlobarShortcut->enabled = enable;
}
