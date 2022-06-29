#include "Services/ShortCut/globalshortcut_p.h"

ShortCut::GlobalShortcutPrivate::GlobalShortcutPrivate()
{

}

ShortCut::GlobalShortcutPrivate::~GlobalShortcutPrivate()
{

}

void ShortCut::GlobalShortcutPrivate::InitHash()
{
    Q_ASSERT(strShortcuts.count() == codeShortcuts.count());
    int index = 0;
    for (const QString &strShortKey : strShortcuts)
    {
        hash.insert(strShortKey, codeShortcuts.at(index));
        ++index;
    }
}

QVector<quint32> ShortCut::GlobalShortcutPrivate::MaskModifiers()
{
    return QVector<quint32>() << 0 << Mod2Mask << LockMask << (Mod2Mask | LockMask);
}

quint32 ShortCut::GlobalShortcutPrivate::X11KeyModificator(const QKeySequence &sequence)
{
    const QStringList list = sequence.toString().split('+');
    quint32 keyModificator = 0;

    for (const QString &str : list)
    {
        if (str == "Ctrl")
        {
            keyModificator |= XCB_MOD_MASK_CONTROL;
            continue;
        }
        else
        {
            if (str == "Alt")
            {
                keyModificator |= XCB_MOD_MASK_1;
                continue;
            }
            else
            {
                if (str == "Shift")
                {
                    keyModificator |= XCB_MOD_MASK_SHIFT;
                    continue;
                }
                else
                {
                    if (str == "Meta")
                    {
                        keyModificator |= XCB_MOD_MASK_4;
                        continue;
                    }
                }
            }
        }
    }
    return keyModificator;
}

quint32 ShortCut::GlobalShortcutPrivate::X11HotKey(Display *display, const QKeySequence &sequence)
{
    const QStringList list = sequence.toString().split('+');
    const QString lastString = list.constLast();
    if (lastString.count() == 0)
    {
        return XKeysymToKeycode(display, XK_plus);
    }
    else
    {
        if (1 == lastString.count())
        {
            return XKeysymToKeycode(display, lastString.at(0).unicode()); // return Key Letters and Numbers
        }
        else
        {
            return XKeysymToKeycode(display, hash.value(lastString));
        }
    }
    return 0;
}
