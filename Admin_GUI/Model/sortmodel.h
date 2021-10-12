#ifndef ADMIN_GUI_MODEL_SORTMODEL_H
#define ADMIN_GUI_MODEL_SORTMODEL_H

#include <QSortFilterProxyModel>

#include "Structs/userstruct.h"

class SortModel: public QSortFilterProxyModel
{
    Q_OBJECT
public:
    SortModel(QObject *parent);

public:

    void UpdateSeachWordAndSeachAttribute(const QString &text, const QString &attribute);

protected:

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:

    QString m_searchText;

    QString m_searchAttribute;
};

#endif // ADMIN_GUI_MODEL_SORTMODEL_H
