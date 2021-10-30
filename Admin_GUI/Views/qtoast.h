#ifndef ADMIN_GUI_VIEWS_QTOAST_H
#define ADMIN_GUI_VIEWS_QTOAST_H

#include <QWidget>
#include <QString>
#include <QSize>
#include <QFont>

class QToast : public QWidget
{
   Q_OBJECT

public:
   static int LENGTH_LONG;
   static int LENGTH_SHORT;
   enum TOAST_POS {TOP, CENTER, BOTTOM};
   static QToast *CreateToast(QString strContent, int nToastInterval, QWidget *parent);
   void SetTextFont(const QFont &font)
   {
      m_drawFont = font;
   }
   const QFont &GetTextFont()
   {
      return m_drawFont;
   }
   void SetToastPos();
Q_SIGNALS:
   void signals_finished();
protected:
   void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
   void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
   void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
   void InitUI();
private:
   QString m_strContent;
   int m_nToastInterval;
   int m_nCurrentWindowOpacity;
   int m_nCurrentStayTime;
   int m_nStatus;
   bool m_bAutoDelete;
   QFont m_drawFont;

   QToast(QString strContent, int nToastInterval, QWidget *parent = nullptr);
};

#endif // ADMIN_GUI_VIEWS_QTOAST_H
