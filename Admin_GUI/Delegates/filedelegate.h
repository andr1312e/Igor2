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
   QRect *const m_globalRect;
   QRect *const m_iconRect;
   QRect *const m_programNameRect;
   QRect *const m_pathRect;
   QRect *const m_typeRect;
   QRect *const m_userRoleRect;
   QPen *const m_textPen;
   QPen *const m_titlePen;
   QPen *const m_mouseOverPen;
   QPen *const m_mouseSelectedPen;
   QSize * const m_size;
};

#endif // ADMIN_GUI_DELEGATES_FILEDELEGATE_H
