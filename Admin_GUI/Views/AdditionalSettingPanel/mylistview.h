#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QListView>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>
#include <QStandardItemModel>

#include <Structs/programstruct.h>

class MyListView : public QListView
{
    Q_OBJECT
public:
    MyListView(QWidget *parent);
    ~MyListView();
protected:
    virtual void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
private:
    bool IsItemExsists(const DesktopEntity &newEntity) const;
};

#endif // MYLISTVIEW_H
