#include "Services/ShortCut/globaldata.h"

ShortCut::GlobalData::GlobalData()
{

}

ShortCut::GlobalData::GlobalData(const ShortCut::GlobalData &other)
    : m_key(other.m_key)
    , m_modifier(other.m_modifier)
{

}

ShortCut::GlobalData::~GlobalData()
{

}

quint32 ShortCut::GlobalData::GetKey() const noexcept
{
    return m_key;
}

quint32 ShortCut::GlobalData::GetModifier() const noexcept
{
    return m_modifier;
}

void ShortCut::GlobalData::Setkey(quint32 key)
{
    m_key = key;
}

void ShortCut::GlobalData::SetModifier(quint32 modifier)
{
    m_modifier = modifier;
}


