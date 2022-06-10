#include "mylistview.h"


MyListView::MyListView(QWidget *parent)
    :QListView(parent)
{

}

MyListView::~MyListView()
{

}

void MyListView::dropEvent(QDropEvent *event)
{
    const MyListView *const source =qobject_cast<MyListView*>(event->source());
    if (Q_NULLPTR!=source)
    {
        const QModelIndex index=source->currentIndex();
        if(index.isValid())
        {
            const QVariant indexData=index.data(Qt::UserRole+1);
            const DesktopEntity entity=indexData.value<DesktopEntity>();
            if(!IsItemExsists(entity))
            {

            }
        }
        else
        {
            qFatal("fd");
        }
    }
    qInfo()<< "dropEvent()";
}

bool MyListView::IsItemExsists(const DesktopEntity &newEntity) const
{
    const QStandardItemModel * const myModel=qobject_cast<QStandardItemModel*>(model());
    if(Q_NULLPTR==myModel)
    {
         qFatal("");
    }
    else
    {
        for (int i=0; i<myModel->rowCount(); ++i)
        {
            const QModelIndex index=model()->index(i, 0);
            if(index.isValid())
            {
                const QVariant indexData=index.data(Qt::UserRole+1);
                const DesktopEntity entity=indexData.value<DesktopEntity>();
                if(newEntity==entity)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
