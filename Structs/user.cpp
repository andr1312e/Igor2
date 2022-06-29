#include "user.h"

User::User()
    : m_userImage(QLatin1Literal(":/images/users/noRole.png"))
{
    m_roleIndex = -1;
}

User::User(const QString &_userId, const QString &_name, const QString &_FCS, int _role)
    : m_userId(_userId)
    , m_name(_name)
    , m_FCS(_FCS)
    , m_roleIndex(_role)
    , m_userImage(QLatin1Literal(":/images/users/noRole.png"))
{

}

User::~User()
{

}

bool User::operator<(const User &user)
{
    if (user.m_name == this->m_name)
    {
        if (this->m_userId == user.m_userId)
        {
            return this->m_roleIndex > user.m_roleIndex;
        }
        else
        {
            return this->m_userId < user.m_userId;
        }
    }
    else
    {
        return this->m_name < user.m_name;
    }
}

bool User::operator==(const User &user)
{
    if (user.m_userId == this->m_userId && user.m_name == this->m_name)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void User::SetUserData(int roleIndex)
{
    m_roleIndex = roleIndex;
}

void User::SetUserData(const QString &FCS)
{
    m_FCS = FCS;
}

void User::SetUserData(const QString &FCS, int roleIndex)
{
    if (m_name.isEmpty() || m_userId.isEmpty())
    {
        qFatal("fd");
    }
    else
    {
        SetUserData(FCS);
        SetUserData(roleIndex);
    }
}

void User::SetUserData(const QString &userId, const QString &name, const QString &FCS, int roleIndex)
{
    m_userId = userId;
    m_name = name;
    SetUserData(FCS, roleIndex);
}

void User::SetUserData(const QString &userId, const QString &name, const QString &FCS, int roleIndex, const QString &userImage)
{
    SetUserData(userId, name, FCS, roleIndex);
    m_userImage = userImage;
}

void User::SetUserImage(const QString &_userImage)
{
    m_userImage = _userImage;
}

void User::ClearUserData()
{
    m_FCS.clear();
    m_roleIndex = -1;
}

QString User::GetUserId() const noexcept
{
    return m_userId;
}

QString User::GetUserName() const noexcept
{
    return m_name;
}

QString User::GetUserFCS() const noexcept
{
    return m_FCS;
}

int User::GetUserRole() const noexcept
{
    return m_roleIndex;
}

QString User::GetUserImage() const noexcept
{
    return m_userImage;
}

bool User::RoleIsValid() const noexcept
{
    return m_roleIndex >= 0 && m_roleIndex < Roles.count();
}

bool User::RoleIsValid(int roleId) noexcept
{
    return roleId >= 0 && roleId < Roles.count();
}
