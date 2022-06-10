#ifndef STRUCTS_USERSTRUCT_H
#define STRUCTS_USERSTRUCT_H

#include <QMetaType>
#include <QString>
#include <QStringList>

class User
{
public:
    explicit User();
    explicit User(const QString &_userId, const QString &_name, const QString &_FCS = "", int _role = -1);
    ~User();
public:
    bool operator<(const User &user);
    bool operator==(const User &user);
public:
    void SetUserData(int roleIndex);
    void SetUserData(const QString &FCS);
    void SetUserData(const QString &FCS, int role);
    void SetUserData(const QString &userId, const QString &name, const QString &FCS, int roleIndex);
    void SetUserData(const QString &userId, const QString &name, const QString &FCS, int roleIndex, const QString &userImage);
    void SetUserImage(const QString &userImage);
    void ClearUserData();
public:
    QString GetUserId() const noexcept;
    QString GetUserName() const noexcept;
    QString GetUserFCS() const noexcept;
    int GetUserRole() const noexcept;
    QString GetUserImage() const noexcept;
public:
    bool RoleIsValid() const noexcept;
    static bool RoleIsValid(int roleId) noexcept;
private:
    QString m_userId;
    QString m_name;
    QString m_FCS;
    int m_roleIndex;
    QString m_userImage;
};

Q_DECLARE_METATYPE(User)

static const QStringList Roles = {    QStringLiteral("Пользователь"),
                                      QStringLiteral("Оператор"),
                                      QStringLiteral("Специалист"),
                                      QStringLiteral("Администратор"),
                                 };


static const QStringList comboBoxSearchAttributes =
{
    QStringLiteral("ФИО"),
    QStringLiteral("Идендификатор"),
    QStringLiteral("Имя в ОС Astra"),
};

static const QStringList userEditFields =
{
    QStringLiteral("Добавить пользователя в базу:"),
    QStringLiteral("Изменить данные пользователя:")
};

#endif // STRUCTS_USERSTRUCT_H
