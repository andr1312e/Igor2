#ifndef SERVICES_SHORTCUT_GLOBALDATA_H
#define SERVICES_SHORTCUT_GLOBALDATA_H

#include <QtGlobal>
#include <QVarLengthArray>
#include <QStringList>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <xcb/xcb.h>

namespace ShortCut
{
static const QStringList strShortcuts = {"Esc", "Tab", "BackTab", "Backspace", "Return", "Enter", "Ins", "Del",
                                         "Pause", "Print", "SysReq", "Clear", "Home", "End", "Left", "Up", "Right",
                                         "Down", "PgUp", "PgDown", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
                                         "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24",
                                         "Space", "*", ",", "-", "/"
                                        };

static const QVarLengthArray<quint32, 49> codeShortcuts = {XK_Escape, XK_Tab, XK_Tab, XK_BackSpace, XK_Return, XK_Return, XK_Insert, XK_Delete,
                                                           XK_Pause, XK_Print, XK_Sys_Req, XK_Clear, XK_Home, XK_End, XK_Left, XK_Up, XK_Right,
                                                           XK_Down, XK_Page_Up, XK_Page_Down, XK_F1, XK_F2, XK_F3, XK_F4, XK_F5, XK_F6, XK_F7, XK_F8, XK_F9, XK_F10, XK_F11, XK_F12,
                                                           XK_F13, XK_F14, XK_F15, XK_F16, XK_F17, XK_F18, XK_F19, XK_F20, XK_F21, XK_F22, XK_F23, XK_F24,
                                                           XK_space, XK_asterisk, XK_comma, XK_minus, XK_slash
                                                          };

class GlobalData
{
public:
    GlobalData();
    GlobalData(const GlobalData &other);
    ~GlobalData();
public:
    quint32 GetKey() const noexcept;
    quint32 GetModifier() const noexcept;
    void Setkey(quint32 key);
    void SetModifier(quint32 modifier);
public:

private:
    quint64 m_key;
    quint64 m_modifier;
};
}
#endif // SERVICES_SHORTCUT_GLOBALDATA_H
