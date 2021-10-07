#ifndef ADMIN_GUI_DELEGATES_USERDELEGATE_H
#define ADMIN_GUI_DELEGATES_USERDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

#include "Structs/userstruct.h"

class UserDelegate : public QStyledItemDelegate
{
public:

    UserDelegate(QFont *font, QObject *parent);

    ~UserDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
private:
    QRect *m_rect;
    QRect *m_iconRect;
    QRect *m_userIdRect;
    QRect *m_userNameRect;
    QRect *m_userFCSRect;
    QRect *m_userRoleRect;
    QPen *m_grayPen;
    QPen *m_whitePen;
    QColor *m_mouseOverColor;
    QPen *m_mouseOverPen;
    QColor *m_mouseSelectedColor;
    QPen *m_mouseSelectedPen;
    QFont *m_font;
    QSize *m_size;
};

#endif // ADMIN_GUI_DELEGATES_USERDELEGATE_H
