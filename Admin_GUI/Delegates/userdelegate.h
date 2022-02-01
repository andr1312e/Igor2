#ifndef ADMIN_GUI_DELEGATES_USERDELEGATE_H
#define ADMIN_GUI_DELEGATES_USERDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

#include "Structs/userstruct.h"

class UserDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    UserDelegate(QFont &font, QObject *parent);
    ~UserDelegate();

public:
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const Q_DECL_OVERRIDE;
private:
    QRect * const m_globalRect;
    QRect * const m_iconRect;
    QRect * const m_userIdRect;
    QRect *const m_userNameRect;
    QRect *const m_userFCSRect;
    QRect *const m_userRoleRect;
    QPen *const m_textPen;
    QPen *const m_titlePen;
    QPen *const m_mouseOverPen;
    QPen *const m_mouseSelectedPen;
    const QFont m_font;
    QSize *const  m_size;
};

#endif // ADMIN_GUI_DELEGATES_USERDELEGATE_H
