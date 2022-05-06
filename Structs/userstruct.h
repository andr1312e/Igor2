#ifndef STRUCTS_USERSTRUCT_H
#define STRUCTS_USERSTRUCT_H

#include <QMetaType>
#include <QString>
#include <QStringList>

struct User
{
    User(){}
    QString userId;
    QString name;
    QString FCS;
    int role;
    QString userImage;
};

Q_DECLARE_TYPEINFO(User, Q_MOVABLE_TYPE);

static const QStringList Roles={    QStringLiteral("Пользователь"),
                                    QStringLiteral("Оператор"),
                                    QStringLiteral("Специалист"),
                                    QStringLiteral("Администратор"), };

static const QStringList RolesToolTip={  QStringLiteral("Просмотр базы данных недоступен. Доступны только базовые возможности"),
                                         QStringLiteral("Просмотр базы данных недоступен. В запускаемых программах будут включены возможности: QStringLiteral"),
                                         QStringLiteral("Обладает возможностями по просмотру базы данных"),
                                         QStringLiteral("Обладает неограниченными возможностями по редактированию базы данных, устанавливаете эту роль только по необходимости"), };


static const QStringList comboBoxSearchAttributes={
    QStringLiteral("ФИО"),
    QStringLiteral("ID"),
    QStringLiteral("Имя в ОС Astra"),
};

static const QStringList userEditFields={
    QStringLiteral("Добавить пользователя в базу:"),
    QStringLiteral("Изменить данные пользователя:")
};

Q_DECLARE_METATYPE(User)

#endif // STRUCTS_USERSTRUCT_H
