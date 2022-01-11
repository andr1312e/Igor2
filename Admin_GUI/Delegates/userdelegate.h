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

public Q_SLOTS:
    void OnSetDelegateView(bool state);

public:
    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const Q_DECL_OVERRIDE;
private:
    QRect *m_globalRect;
    QRect *m_iconRect;
    QRect *m_userIdRect;
    QRect *m_userNameRect;
    QRect *m_userFCSRect;
    QRect *m_userRoleRect;
    QPen *m_textPen;
    QPen *m_titlePen;
    QPen *m_mouseOverPen;
    QPen *m_mouseSelectedPen;
    QFont m_font;
    QSize *m_size;
    bool m_listItemsView;
};

#endif // ADMIN_GUI_DELEGATES_USERDELEGATE_H
