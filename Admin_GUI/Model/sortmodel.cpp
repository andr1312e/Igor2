#include "sortmodel.h"

SortModel::SortModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{

}

SortModel::~SortModel()
{

}

void SortModel::UpdateSeachTextAndSeachAttribute(const QString &text, const QString &attribute) noexcept
{
    m_searchText = text;
    m_searchAttribute = attribute;
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
        const QVariant data = index.data(Qt::UserRole + 1);
        const User user = data.value<User>();
        if (comboBoxSearchAttributes.at(0) == m_searchAttribute)
        {
            return user.GetUserFCS().contains(m_searchText);
        }
        else
        {
            if (comboBoxSearchAttributes.at(1) == m_searchAttribute)
            {
                return user.GetUserId().contains(m_searchText);
            }
            else
            {
                return user.GetUserName().contains(m_searchText);
            }
        }
    }
}
