#include "sortmodel.h"

SortModel::SortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

SortModel::~SortModel()
{

}

void SortModel::UpdateSeachWordAndSeachAttribute(const QString &text, const QString &attribute)
{
    m_searchText=text;
    m_searchAttribute=attribute;
    invalidateFilter();//https://evileg.com/en/forum/topic/1421/
}

bool SortModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_searchText!="")
    {
        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        QVariant data=index.data(Qt::UserRole+1);
        User user=data.value<User>();
        if (comboBoxSearchAttributes.at(0)==m_searchAttribute)
        {
            if (user.FCS.contains(m_searchText))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (comboBoxSearchAttributes.at(1)==m_searchAttribute)
            {
                if (user.userId.contains(m_searchText))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (user.name.contains(m_searchText))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return true;
}
