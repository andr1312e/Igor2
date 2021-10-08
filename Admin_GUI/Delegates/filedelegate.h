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

    FileDelegate(QObject *parent);

    ~FileDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
private:
    QRect *m_rect;
    QRect *m_iconRect;
    QRect *m_programNameRect;
    QRect *m_pathRect;
    QRect *m_typeRect;
    QRect *m_userRoleRect;
    QPen *m_textPen;
    QPen *m_titlePen;
    QPen *m_mouseOverPen;
    QPen *m_mouseSelectedPen;
    QSize *m_size;
};

#endif // ADMIN_GUI_DELEGATES_FILEDELEGATE_H
