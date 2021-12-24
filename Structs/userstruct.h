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
    QString role;
    QString m_image;
    bool hasData=false;

//    User& operator =(User &user)
//    {
//        return user;
//    }
};

static const QStringList Roles={    "Пользователь",
                                    "Оператор",
                                    "Специалист",
                                    "Администратор", };

static const QVarLengthArray<QString,4> RolesToolTip={"Просмотр базы данных недоступен. Доступны только базовые возможности",
                                                      "Просмотр базы данных недоступен. В запускаемых программах будут включены возможности: ",
                                                      "Обладает возможностями по просмотру базы данных",
                                                      "Обладает неограниченными возможностями по редактированию базы данных, устанавливаете эту роль только по необходимости", };


//Звание
static const QStringList Ranks={    "Рядовой",
                                    "Ефрейтор",
                                    "Младший сержант",
                                    "Сержант",
                                    "Старший сержант",
                                    "Старшина",
                                    "Прапорщик",
                                    "Старший прапорщик",
                                    "Младший лейтенант",
                                    "Лейтенант",
                                    "Старший лейтенант",
                                    "Капитан",
                                    "Майор",
                                    "Подполковник",
                                    "Полковник",
                                    "Генерал-майор",
                                    "Генерал-лейтенант",
                                    "Генерал-полковник", };


static const QVarLengthArray<QString,18>  RanksICO={":/images/Звания/Рядовой",
                                                    ":/images/Звания/Ефрейтор",
                                                    ":/images/Звания/Младший-сержант",
                                                    ":/images/Звания/Сержант",
                                                    ":/images/Звания/Старший-сержант",
                                                    ":/images/Звания/Старшина",
                                                    ":/images/Звания/Прапорщик",
                                                    ":/images/Звания/Старший-прапорщик",
                                                    ":/images/Звания/Младший-лейтенант",
                                                    ":/images/Звания/Лейтенант",
                                                    ":/images/Звания/Старший-лейтенант",
                                                    ":/images/Звания/Капитан",
                                                    ":/images/Звания/Майор",
                                                    ":/images/Звания/Подполковник",
                                                    ":/images/Звания/Полковник",
                                                    ":/images/Звания/Генерал-майор",
                                                    ":/images/Звания/Генерал-лейтенант",
                                                    ":/images/Звания/Генерал-полковник", };



static const QStringList UserKeys={ "1-",
                                    "1+",
                                    "2-",
                                    "2+", };

static const QStringList comboBoxSearchAttributes={
    "ФИО",
    "ID",
    "Имя в ОС Astra",
};

static const QStringList userEditFields={
    "Добавить пользователя в базу:",
    "Изменить данные пользователя:"
};

Q_DECLARE_METATYPE(User)

#endif // STRUCTS_USERSTRUCT_H
