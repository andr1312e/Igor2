#ifndef ADMIN_GUI_DELEGATES_USERDELEGATE_H
#define ADMIN_GUI_DELEGATES_USERDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

#include "Structs/user.h"

class UserDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit UserDelegate(QFont &font, QObject *parent);
    ~UserDelegate();
public:
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const Q_DECL_OVERRIDE;
private:
    QFont m_font;
};

#endif // ADMIN_GUI_DELEGATES_USERDELEGATE_H
