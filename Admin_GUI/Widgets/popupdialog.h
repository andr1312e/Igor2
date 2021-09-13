#ifndef POPUPDIALOG_H
#define POPUPDIALOG_H

#include <QWidget>

class PopupDialog : public QWidget
{
    Q_OBJECT
public:

    PopupDialog(QWidget *parent);

protected:
    virtual void paintEvent(QPaintEvent *e);

};

#endif // POPUPDIALOG_H
