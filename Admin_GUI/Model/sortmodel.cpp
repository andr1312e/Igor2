#include "sortmodel.h"

SortModel::SortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

SortModel::~SortModel()
{

}

void SortModel::UpdateSeachTextAndSeachAttribute(const QString &text, const QString &attribute)
{
    m_searchText=text;
    m_searchAttribute=attribute;
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Обновили фильтр поиска по списку юзеров. Текст: ") + text + QStringLiteral( " аттрибут поиска ") + attribute);
    invalidateFilter();//https://evileg.com/en/forum/topic/1421/
}

bool SortModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_searchText.isEmpty())
    {
        return  true;
    }
    else
    {
        const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
        const QVariant data=index.data(Qt::UserRole+1);
        const User user=data.value<User>();
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
}
