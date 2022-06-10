#ifndef ADMIN_GUI_DELEGATES_FILEDELEGATE_H
#define ADMIN_GUI_DELEGATES_FILEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QRect>
#include <QPainterPath>

#include "Structs/programstruct.h"

class FileDelegate : public QStyledItemDelegate
{
public:

    explicit FileDelegate(QObject *parent);

    ~FileDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // ADMIN_GUI_DELEGATES_FILEDELEGATE_H
