#ifndef STRUCTS_USERSTRUCT_H
#define STRUCTS_USERSTRUCT_H

#include <QMetaType>
#include <QString>
#include <QStringList>

struct User{
    User(){}
    QString userId;
    QString name;
    QString FCS;
    QString rank;
    int role;
    QString m_image;

//    User& operator =(User &user)
//    {
//        return user;
//    }
};

Q_DECLARE_TYPEINFO(User, Q_MOVABLE_TYPE);

static const QStringList Roles={    QStringLiteral("Пользователь"),
                                    QStringLiteral("Оператор"),
                                    QStringLiteral("Специалист"),
                                    QStringLiteral("Администратор"), };

static const QVarLengthArray<QString,4> RolesToolTip={
                                                      QStringLiteral("Просмотр базы данных недоступен. Доступны только базовые возможности"),
                                                      QStringLiteral("Просмотр базы данных недоступен. В запускаемых программах будут включены возможности: QStringLiteral"),
                                                      QStringLiteral("Обладает возможностями по просмотру базы данных"),
                                                      QStringLiteral("Обладает неограниченными возможностями по редактированию базы данных, устанавливаете эту роль только по необходимости"), };


//Звание
static const QStringList Ranks={    QStringLiteral("Рядовой"),
                                    QStringLiteral("Ефрейтор"),
                                    QStringLiteral("Младший сержант"),
                                    QStringLiteral("Сержант"),
                                    QStringLiteral("Старший сержант"),
                                    QStringLiteral("Старшина"),
                                    QStringLiteral("Прапорщик"),
                                    QStringLiteral("Старший прапорщик"),
                                    QStringLiteral("Младший лейтенант"),
                                    QStringLiteral("Лейтенант"),
                                    QStringLiteral("Старший лейтенант"),
                                    QStringLiteral("Капитан"),
                                    QStringLiteral("Майор"),
                                    QStringLiteral("Подполковник"),
                                    QStringLiteral("Полковник"),
                                    QStringLiteral("Генерал-майор"),
                                    QStringLiteral("Генерал-лейтенант"),
                                    QStringLiteral("Генерал-полковник"), };


static const QVarLengthArray<QString,18>  RanksICO={QStringLiteral(":/images/Звания/Рядовой"),
                                                    QStringLiteral(":/images/Звания/Ефрейтор"),
                                                    QStringLiteral(":/images/Звания/Младший-сержант"),
                                                    QStringLiteral(":/images/Звания/Сержант"),
                                                    QStringLiteral(":/images/Звания/Старший-сержант"),
                                                    QStringLiteral(":/images/Звания/Старшина"),
                                                    QStringLiteral(":/images/Звания/Прапорщик"),
                                                    QStringLiteral(":/images/Звания/Старший-прапорщик"),
                                                    QStringLiteral(":/images/Звания/Младший-лейтенант"),
                                                    QStringLiteral(":/images/Звания/Лейтенант"),
                                                    QStringLiteral(":/images/Звания/Старший-лейтенант"),
                                                    QStringLiteral(":/images/Звания/Капитан"),
                                                    QStringLiteral(":/images/Звания/Майор"),
                                                    QStringLiteral(":/images/Звания/Подполковник"),
                                                    QStringLiteral(":/images/Звания/Полковник"),
                                                    QStringLiteral(":/images/Звания/Генерал-майор"),
                                                    QStringLiteral(":/images/Звания/Генерал-лейтенант"),
                                                    QStringLiteral(":/images/Звания/Генерал-полковник"), };

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
