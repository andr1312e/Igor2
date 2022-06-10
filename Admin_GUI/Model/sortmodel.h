#ifndef ADMIN_GUI_MODEL_SORTMODEL_H
#define ADMIN_GUI_MODEL_SORTMODEL_H

#include <QSortFilterProxyModel>

#include "Logging/logger.h"
#include "Structs/user.h"

class SortModel: public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SortModel(QObject *parent);
    ~SortModel();
public:
    void UpdateSeachTextAndSeachAttribute(const QString &text, const QString &attribute) noexcept;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const Q_DECL_OVERRIDE;

private:
    QString m_searchText;
    QString m_searchAttribute;
};

#endif // ADMIN_GUI_MODEL_SORTMODEL_H
