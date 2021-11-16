#include "filedelegate.h"
#include <QDebug>

FileDelegate::FileDelegate(QObject *parent)
   : QStyledItemDelegate(parent)
   , m_globalRect(new QRect())
   , m_iconRect(new QRect())
   , m_programNameRect(new QRect())
   , m_pathRect(new QRect())
   , m_typeRect(new QRect())
   , m_userRoleRect(new QRect())
   , m_textPen(new QPen())
   , m_titlePen(new QPen())
   , m_mouseOverPen(new QPen())
   , m_mouseSelectedPen(new QPen())
   , m_size(new QSize(0, 70))
{

}

FileDelegate::~FileDelegate()
{
   delete m_globalRect;
   delete m_iconRect;
   delete m_programNameRect;
   delete m_pathRect;
   delete m_typeRect;
   delete m_userRoleRect;
   delete m_textPen;
   delete m_titlePen;
   delete m_mouseOverPen;
   delete m_mouseSelectedPen;
   delete m_size;
}

void FileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
   if (index.isValid()) {
      painter->save();

      QVariant data = index.data(Qt::UserRole + 1);
      DesktopEntity icon = data.value<DesktopEntity>();

      m_globalRect->setX(option.rect.x());
      m_globalRect->setY(option.rect.y());

      m_globalRect->setWidth(option.rect.width() + 1);
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

      if (option.state.testFlag(QStyle::State_MouseOver)) {
         m_mouseOverPen->setColor(option.palette.color(QPalette::ToolTipBase));
         painter->setPen(*m_mouseOverPen);
         painter->setBrush(m_mouseOverPen->color());
         painter->drawPath(path);
         m_titlePen->setColor(option.palette.color(QPalette::Link));
         m_textPen->setColor(option.palette.color(QPalette::Base));
      } else {
         if (option.state.testFlag(QStyle::State_Selected)) {
            m_mouseSelectedPen->setColor(option.palette.color(QPalette::Highlight));
            painter->setPen(*m_mouseSelectedPen);
            painter->setBrush(m_mouseSelectedPen->color());
            painter->drawPath(path);
         }

         m_titlePen->setColor(option.palette.color(QPalette::HighlightedText));
         m_textPen->setColor(option.palette.color(QPalette::WindowText));
      }

      m_iconRect->setRect(m_globalRect->left() + 5, m_globalRect->top() + 5, 45, 45);
      m_programNameRect->setRect(m_iconRect->right() + 5, m_iconRect->top(), m_globalRect->width() - 10 - m_iconRect->width(), 20);
      m_pathRect->setRect(m_programNameRect->left(), m_programNameRect->bottom() + 1, m_globalRect->width() - m_iconRect->width(), 20);
      m_typeRect->setRect(m_programNameRect->left(), m_pathRect->bottom() - 1, m_globalRect->width() - m_iconRect->width(), 20);

      painter->setPen(*m_titlePen);
      painter->setFont(QFont("Microsoft Yahei", 12));
      painter->drawText(*m_programNameRect, "Название: " + icon.name);

      painter->setPen(*m_textPen);
      painter->drawText(*m_pathRect, "Путь: " + icon.exec);
      painter->drawText(*m_typeRect, "Тип: " + icon.type);



      painter->restore();
   }
}

QSize FileDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
   Q_UNUSED(index)
   m_size->setWidth(option.rect.width());
   return *m_size;
}
