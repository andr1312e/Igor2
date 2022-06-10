#include "programstruct.h"

DesktopEntity::DesktopEntity()
{

}

DesktopEntity::DesktopEntity(const QString &name, const QString &exec, const QString &icon)
    : m_name(name)
    , m_exec(exec)
    , m_icon(icon)
{

}

DesktopEntity::DesktopEntity(const DesktopEntity &entity)
    : m_name(entity.GetName())
    , m_exec(entity.GetExec())
    , m_icon(entity.GetIcon())
{

}

DesktopEntity::~DesktopEntity()
{

}

bool DesktopEntity::operator ==(const DesktopEntity &entity) const noexcept
{
    return m_name==entity.m_name && m_exec==entity.m_exec;
}

DesktopEntity& DesktopEntity::operator =(const DesktopEntity &entity) noexcept
{
    if(this!=&entity)
    {
        m_name=entity.m_name;
        m_exec=entity.m_exec;
        m_icon=entity.m_icon;
    }
    return *this;
}

void DesktopEntity::SetParams(const QString &name, const QString &exec, const QString &icon) noexcept
{
    SetParams(name, exec);
    SetIcon(icon);
}

void DesktopEntity::SetParams(const QString &name, const QString &exec) noexcept
{
    SetName(name);
    SetExec(exec);
}

void DesktopEntity::SetName(const QString &name) noexcept
{
    m_name=name;
}

void DesktopEntity::SetExec(const QString &exec) noexcept
{
    m_exec=exec;
}

void DesktopEntity::SetIcon(const QString &icon) noexcept
{
    m_icon=icon;
}

const QString &DesktopEntity::GetName() const noexcept
{
    return m_name;
}

const QString &DesktopEntity::GetExec() const noexcept
{
    return m_exec;
}

const QString &DesktopEntity::GetIcon() const noexcept
{
    return m_icon;
}
