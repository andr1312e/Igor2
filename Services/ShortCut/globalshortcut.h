#ifndef SERVICES_SHORTCUT_GLOBALSHORTCUT_H
#define SERVICES_SHORTCUT_GLOBALSHORTCUT_H

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QKeySequence>
#include <QApplication>

#include "Services/ShortCut/globalshortcut_p.h"

namespace ShortCut
{

class GlobalShortcut : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit GlobalShortcut(QObject *parent);
    ~GlobalShortcut();
public:
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;
public:
    void SetShortcut(const QKeySequence &keySequence);
    void UnsetShortcut();
    QKeySequence GetShortcut() const noexcept;
    bool IsEmpty() const noexcept;
    bool IsEnabled() const noexcept;
    void SetEnabled(bool enable);

Q_SIGNALS:
    void ToActivate();
private:
    ShortCut::GlobalShortcutPrivate *m_privateGlobarShortcut;
};
}



#endif // SERVICES_SHORTCUT_GLOBALSHORTCUT_H
