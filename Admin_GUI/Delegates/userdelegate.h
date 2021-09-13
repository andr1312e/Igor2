#ifndef ADMIN_GUI_DELEGATES_USERDELEGATE_H
#define ADMIN_GUI_DELEGATES_USERDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

#include "Structs/userstruct.h"

class UserDelegate : public QStyledItemDelegate
{
public:

    UserDelegate(QObject *parent);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // ADMIN_GUI_DELEGATES_USERDELEGATE_H
