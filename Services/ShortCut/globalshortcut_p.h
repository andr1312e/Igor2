#ifndef SERVICES_SHORTCUT_GLOBALSHORTCUT_P_H
#define SERVICES_SHORTCUT_GLOBALSHORTCUT_P_H

#include <QHash>
#include <QKeySequence>
#include <QX11Info>
#include <QVector>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <xcb/xcb.h>

#include "Services/ShortCut/globaldata.h"
namespace ShortCut
{
class GlobalShortcutPrivate
{
public:
    GlobalShortcutPrivate();
    ~GlobalShortcutPrivate();
public:
    void InitHash();
    QVector<quint32> MaskModifiers();
    quint32 X11KeyModificator(const QKeySequence &sequence);
    quint32 X11HotKey(Display *display, const QKeySequence &sequence);
public:
    QKeySequence keys;
    QList<GlobalData *>listKeys;
    QHash <QString, unsigned int>hash;
    bool enabled;
    Display *m_display;
    Window m_win;
};
}


#endif // SERVICES_SHORTCUT_GLOBALSHORTCUT_P_H
