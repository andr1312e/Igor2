#include "userdelegate.h"
#include <QDebug>

UserDelegate::UserDelegate(QFont &font, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_globalRect(new QRect())
    , m_iconRect(new QRect())
    , m_userIdRect(new QRect())
    , m_userNameRect(new QRect())
    , m_userFCSRect(new QRect())
    , m_userRoleRect(new QRect())
    , m_textPen(new QPen())
    , m_titlePen(new QPen())
    , m_mouseOverPen(new QPen())
    , m_mouseSelectedPen(new QPen())
    , m_font(font)
    , m_size(new QSize(300, 0))
    , m_listItemsView(true)
{

}

UserDelegate::~UserDelegate()
{
    delete m_globalRect;
    delete m_iconRect;
    delete m_userIdRect;
    delete m_userNameRect;
    delete m_userFCSRect;
    delete m_userRoleRect;
    delete m_textPen;
    delete m_titlePen;
    delete m_mouseOverPen;
    delete m_mouseSelectedPen;
    delete m_size;
}

void UserDelegate::OnSetDelegateView(bool state)
{
    m_listItemsView=state;
}

void UserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //https://doc.qt.io/qt-5/qstyleditemdelegate.html#paint
    if (index.isValid()) {
        painter->save();

        QVariant data = index.data(Qt::UserRole + 1);
        User user = data.value<User>();
        m_globalRect->setX(option.rect.x());
        m_globalRect->setY(option.rect.y());
        m_globalRect->setWidth(option.rect.width());
        m_globalRect->setHeight(option.rect.height() - 1);

        QPainterPath path;
        path.moveTo(m_globalRect->topRight());
        path.lineTo(m_globalRect->topLeft());
        path.quadTo(m_globalRect->topLeft(), m_globalRect->topLeft());
        path.lineTo(m_globalRect->bottomLeft());
        path.quadTo(m_globalRect->bottomLeft(), m_globalRect->bottomLeft());
        path.lineTo(m_globalRect->bottomRight());
        path.quadTo(m_globalRect->bottomRight(), m_globalRect->bottomRight());
        path.lineTo(m_globalRect->topRight());
        path.quadTo(m_globalRect->topRight(), m_globalRect->topRight());


        if (option.state.testFlag(QStyle::State_MouseOver))
        {
            m_mouseOverPen->setColor(option.palette.color(QPalette::ToolTipBase));
            painter->setPen(*m_mouseOverPen);
            painter->setBrush(m_mouseOverPen->color());
            painter->drawPath(path);
            m_titlePen->setColor(option.palette.color(QPalette::Link));
            m_textPen->setColor(option.palette.color(QPalette::Base));
        }
        else
        {
            if (option.state.testFlag(QStyle::State_Selected)) {
                m_mouseSelectedPen->setColor(option.palette.color(QPalette::Highlight));
                painter->setPen(*m_mouseSelectedPen);
                painter->setBrush(m_mouseSelectedPen->color());
                painter->drawPath(path);
            }

            m_titlePen->setColor(option.palette.color(QPalette::HighlightedText));
            m_textPen->setColor(option.palette.color(QPalette::WindowText));
        }

        m_iconRect->setRect(m_globalRect->left() + 5, m_globalRect->top() + 5, m_font.pointSize() * 4 + 40, m_font.pointSize() * 4 + 40);
        m_userIdRect->setRect(m_iconRect->right() + 5, m_iconRect->top(), m_globalRect->width() - 10 - m_iconRect->width(), m_font.pointSize() + 10);
        m_userNameRect->setRect(m_userIdRect->left(), m_userIdRect->bottom() + 1, m_globalRect->width() - m_iconRect->width(), m_font.pointSize() + 10);
        m_userFCSRect->setRect(m_userIdRect->left(), m_userNameRect->bottom() - 1, m_globalRect->width() - m_iconRect->width(), m_font.pointSize() + 10);
        m_userRoleRect->setRect(m_userIdRect->left(), m_userFCSRect->bottom() - 1, m_globalRect->width() - m_iconRect->width(), m_font.pointSize() + 10);

        painter->drawImage(*m_iconRect, QImage(user.m_image));

        painter->setPen(*m_titlePen);
        painter->setFont(m_font);
        painter->drawText(*m_userIdRect, "Идентификатор: " + user.userId);

        painter->setPen(*m_textPen);
        painter->drawText(*m_userNameRect, "Имя пользователя: " + user.name);
        painter->drawText(*m_userFCSRect, "ФИО: " + user.FCS);
        QString role=(user.role>=0 && user.role < Roles.count()) ? Roles.at(user.role) : " ";
        painter->drawText(*m_userRoleRect, "Роль: " + role);
        painter->restore();

    }
}

QSize UserDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)
    //    if(m_listItemsView)
    //    {
    //        m_size->setWidth(300);
    //    }
    //    else
    //    {
    m_size->setWidth(300);
    //    }
    m_size->setHeight(m_font.pointSize() * 4 + 50);
    return *m_size;
}
