#include "sqldatabaseserivce.h"

SqlDatabaseSerivce::SqlDatabaseSerivce(QObject *parent)
    : QObject(parent)
    , m_postgeSqlDatabaseDriverStringKey(QStringLiteral("QPSQL"))
    , m_currentRoleModel(new QSqlQueryModel(this))
    , m_usersTablePrefix(QStringLiteral("rlstiusers"))
    , m_startupTablePrefix(QStringLiteral("rlstistartups"))
    , m_desktopTablePrefix(QStringLiteral("rlstidesktops"))

{
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO+ QStringLiteral(" Создаем сервис для работы с бд. Драйвера баз данных в системе: ")+QSqlDatabase::drivers().join(' '));
}

SqlDatabaseSerivce::~SqlDatabaseSerivce()
{
    if(m_db.isOpen())
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Закрываем бд: "));
        m_db.close();
    }
    else
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Бд не была открыта "));
    }
    delete m_currentRoleModel;
}

bool SqlDatabaseSerivce::ConnectToDataBase(const QString &hostName, const quint16 &port, const QString &databaseName, const QString &userName, const QString &dbPassword)
{
    m_db=QSqlDatabase::addDatabase(m_postgeSqlDatabaseDriverStringKey);
    m_db.setHostName(hostName);
    m_db.setPort(port);
    m_db.setDatabaseName(databaseName);
    m_db.setUserName(userName);
    m_db.setPassword(dbPassword);
    if (m_db.open(userName, dbPassword))
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO+ QStringLiteral(" Подключились к базе данных"));
        return true;
    }
    else
    {
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO+ QStringLiteral(" Ошибка в подключении к базе данных. Ошибка: ") + m_db.lastError().text());
        return false;
    }
}

void SqlDatabaseSerivce::CreateUsersTableIfNotExists()
{
    QSqlQuery query;
    if (query.exec("CREATE TABLE IF NOT EXISTS " + m_usersTablePrefix +
                   " (id        SERIAL    PRIMARY KEY, "+
                   userIdCN  +" VARCHAR(100) NOT NULL, "+
                   userNameCN+" VARCHAR(100) NOT NULL, "+
                   fcsCN + "    VARCHAR(100) NOT NULL, "+
                   rankCN+ "    VARCHAR(100) NOT NULL, "+
                   roleCN+ "    INT NOT NULL)"))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось создать бд с пользователями. Ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::CreateStartupsTableIfNotExists(int roleId)
{
    QSqlQuery query;
    const QString request="CREATE TABLE IF NOT EXISTS " + m_startupTablePrefix +QString::number(roleId)+
            " (id             SERIAL   PRIMARY KEY, "+
            startupPathCN + " VARCHAR(100) NOT NULL)";
    if (query.exec(request))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось создать бд с файлами перезапуска стола. Роль: ") + QString::number(roleId)+ QStringLiteral(" Ошибка: ")+ query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::CreateDesktopRolesIfNotExists(int roleId)
{
    QSqlQuery query;
    if(query.exec("CREATE TABLE IF NOT EXISTS " + m_desktopTablePrefix +QString::number(roleId)+
                  " (id             SERIAL    PRIMARY KEY, "+
                  desktopNameCN + " VARCHAR(100) NOT NULL, "+
                  desktopTypeCN +"  VARCHAR(100) NOT NULL, "+
                  execPathCN +"     VARCHAR(100) NOT NULL, "+
                  iconPathCN +"     VARCHAR(100));"))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось создать бд с файлами рабочего стола. Роль: ") + QString::number(roleId)+ QStringLiteral(" Ошибка: ")+ query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QStringList SqlDatabaseSerivce::GetAdminsRoleUserName()
{
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получаем список имен администратора: "));
    QStringList adminsUserNameList;
    QSqlQuery query;
    query.prepare("SELECT "+ userNameCN + " FROM " + m_usersTablePrefix+
                  " WHERE " + rankCN + " = '" + Roles.at(Roles.count()-1) + "'"
                  );
    if(query.exec())
    {
        while(query.next())
        {
            if(QVariant::String==query.value(0).type())
            {
                adminsUserNameList.append(query.value(0).toString());
            }
            else
            {
                qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не преобразовать имя пользователя в строку. Настоящий тип данных ") + QString::number(query.value(0).type())).toUtf8().constData());
            }
        }
        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Администраторы : ") + adminsUserNameList.join(' '));
        return  adminsUserNameList;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем получить список имен администратора, запрос не выполнен. Ошибка: ") + query.lastError().text().toUtf8().constData()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QString SqlDatabaseSerivce::GetUserFCS(const QString &currentUserName)
{
    Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Получаем ФИО пользователя: имя пользователя ") + currentUserName);
    QSqlQuery query;
    query.prepare("SELECT " + fcsCN+ " FROM "+ m_usersTablePrefix+
                  " WHERE "+ userNameCN +"=?");
    query.bindValue(0, currentUserName.simplified());
    if(query.exec())
    {
        if(query.next())
        {
            if(QVariant::String==query.value(0).type())
            {
                const QString fcs=query.value(0).toString();
                return fcs;
            }
            else
            {
                qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не преобразовать ФИО пользователя в строку. Настоящий тип данных ") + QString::number(query.value(0).type()) + QStringLiteral(" имя пользователя ") + currentUserName ).toUtf8().constData());
            }
        }
        else
        {
            return QString();
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Не можем получить ФИО пользователя, запрос не выполнен. Ошибка: ")+ query.lastError().text().toUtf8().constData() + QStringLiteral(" имя пользователя ") + currentUserName).toUtf8().constData());
    }
}

QString SqlDatabaseSerivce::GetUserRank(const QString &currentUserName)
{
    Log4Qt::Logger::rootLogger()->info(QStringLiteral(" Получаем ранг пользователя: ")+Q_FUNC_INFO + QStringLiteral(" имя пользователя ") + currentUserName);
    QSqlQuery query;
    query.prepare("SELECT " + rankCN+ " FROM "+ m_usersTablePrefix+
                  " WHERE "+ userNameCN +"=?");
    query.bindValue(0, currentUserName);
    if(query.exec())
    {
        if(query.next())
        {
            if(query.value(0).type()==QVariant::String)
            {
                QString rank=query.value(0).toString();
                return rank;
            }
            else
            {
                qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не преобразовать ранг пользователя в строку. Настоящий тип данных: ") + QString::number(query.value(0).type()) + QStringLiteral(" Имя пользователя: ") + currentUserName ).toUtf8().constData());
            }
        }
        else
        {
            return QString();
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Не можем получить ранг пользователя, запрос не выполнен. Ошибка: ")+ query.lastError().text().toUtf8().constData() + QStringLiteral(" Имя пользователя: ") + currentUserName).toUtf8().constData());
    }
}

int SqlDatabaseSerivce::GetUserRole(const QString &currentUserName)
{
    QSqlQuery query;
    query.prepare("SELECT " + roleCN+ " FROM "+ m_usersTablePrefix+
                  " WHERE "+ userNameCN +"=?");
    query.bindValue(0, currentUserName);
    if(query.exec())
    {
        if(query.next())
        {
            if(query.value(0).type()==QVariant::Int)
            {
                bool ok;
                int role=query.value(0).toInt(&ok);
                if(ok)
                {
                    return role;
                }
                else
                {
                    qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Не можем конвертировать в число. Число ") + QString::number(query.value(0).type()) + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
                }
            }
            else
            {
                qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем преобразовать роль пользователя в число. Hастоящий тип данных: ") + QString::number(query.value(0).type()) + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем получить роль пользователя, запрос не выполнен. Ошибка: ") + query.lastError().text().toUtf8().constData() + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QString SqlDatabaseSerivce::GetUserFCS(QStringView currentUserName)
{
    QSqlQuery query;
    const QString request="SELECT " + fcsCN+ " FROM "+ m_usersTablePrefix+
            " WHERE "+ userNameCN +"=\'" + currentUserName+ "\'";
    query.prepare(request);
    if(query.exec())
    {
        if(query.next())
        {
            if(QVariant::String==query.value(0).type())
            {
                return query.value(0).toString();
            }
            else
            {
                qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не преобразовать фио пользователя в строку : ") +  QStringLiteral( " настоящий тип данных ") + QString::number(query.value(0).type()) + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
            }
        }
        else
        {
            return QString();
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Не можем получить фио пользователя, запрос не выполнен. Ошибка: ")+ query.lastError().text().toUtf8().constData() + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QString SqlDatabaseSerivce::GetUserRank(QStringView currentUserName)
{
    QSqlQuery query;
    const QString request="SELECT " + rankCN+ " FROM "+ m_usersTablePrefix+
            " WHERE "+ userNameCN +"=\'" + currentUserName + "\'";
    query.prepare(request);
    if(query.exec())
    {
        if(query.next())
        {
            if(QVariant::String ==query.value(0).type())
            {
                QString rank=query.value(0).toString();
                return rank;
            }
            else
            {
                qFatal("%s", QString(Q_FUNC_INFO+QStringLiteral(" Не преобразовать ранг пользователя в строку. Настоящий тип данных: ") + QString::number(query.value(0).type()) + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
            }
        }
        else
        {
            return QString();
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Не можем получить ранг пользователя, запрос не выполнен. Ошибка: ")+ query.lastError().text().toUtf8().constData() + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

int SqlDatabaseSerivce::GetUserRole(QStringView currentUserName)
{
    QSqlQuery query;
    const QString request="SELECT " + roleCN+ " FROM "+ m_usersTablePrefix+
            " WHERE "+ userNameCN +"=\'" + currentUserName +"\'";
    query.prepare(request);
    if(query.exec())
    {
        if(query.next())
        {
            if(QVariant::Int==query.value(0).type())
            {
                bool ok;
                int role=query.value(0).toInt(&ok);
                if(ok)
                {
                    return role;
                }
                else
                {
                    qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем конвертировать в число : ") +  QStringLiteral( " тип ") + QString::number(query.value(0).type()) + QStringLiteral(" имя пользователя ") + currentUserName ).toUtf8().constData());
                }
            }
            else
            {
                qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем преобразовать роль пользователя в число : ")+  QStringLiteral( " настоящий тип данных ") + QString::number(query.value(0).type()) + QStringLiteral(" имя пользователя ") + currentUserName ).toUtf8().constData());
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем получить роль пользователя, запрос не выполнен. Ошибка: ")+ query.lastError().text().toUtf8().constData() + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData() );
    }
}

void SqlDatabaseSerivce::ClearTable(QString tableName)
{
    QSqlQuery query(m_db);
    const QString request=QStringLiteral(" DELETE FROM ")+tableName;
    query.prepare(request);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral("Не получается отчистить таблицу. Ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::ClearUsersTable()
{
    ClearTable(m_usersTablePrefix);
}

void SqlDatabaseSerivce::ClearStartupsTable(int roleId)
{
    QSqlQuery query(m_db);
    ClearTable(m_startupTablePrefix+QString::number(roleId));
}

void SqlDatabaseSerivce::ClearDesktopTable(int roleId)
{
    QSqlQuery query(m_db);
    ClearTable(m_desktopTablePrefix+QString::number(roleId));
}

bool SqlDatabaseSerivce::CheckUsersTable()
{
    QSqlQuery query(m_db);
    query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_usersTablePrefix+ "')");
    if(query.exec())
    {
        return  GetBoolFromMessage(query);
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Не получается проверить наличилие таблицы с пользователями. Ошибка: ")+ query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

bool SqlDatabaseSerivce::CheckStartupTables()
{
    for (int i=0; i<Roles.count(); ++i)
    {
        QSqlQuery query(m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_startupTablePrefix +QString::number(i)+ "')");
        if(query.exec())
        {
            if(false==GetBoolFromMessage(query))
            {
                return false;
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается проверить наличилие таблицы с путями на автозагрузку программ. Ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    return  true;
}

bool SqlDatabaseSerivce::CheckStartupTables(int roleId)
{
    if(roleId>=0 && roleId<Roles.count())
    {
        QSqlQuery query(m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_startupTablePrefix +QString::number(roleId)+ "')");
        if(query.exec())
        {
            return GetBoolFromMessage(query);
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается проверить наличилие таблицы с путями на автозагрузку программ. Роль ") +QString::number(roleId)+ QStringLiteral( " ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        return false;
    }
}

bool SqlDatabaseSerivce::CheckDesktopTables()
{
    for (int i=0; i<Roles.count(); ++i)
    {
        QSqlQuery query(m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_desktopTablePrefix +QString::number(i)+ "')");
        if(query.exec())
        {
            if(false==GetBoolFromMessage(query))
            {
                return false;
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается проверить наличилие таблицы с ярлыками программ. Ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    return  true;
}

bool SqlDatabaseSerivce::CheckDesktopTables(int roleId)
{
    if(roleId>=0 && roleId<Roles.count())
    {
        QSqlQuery query(m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_desktopTablePrefix +QString::number(roleId)+ "')");
        if(query.exec())
        {
            return GetBoolFromMessage(query);
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается проверить наличилие таблицы с ярлыками программ. Роль: ") +QString::number(roleId)+ QStringLiteral( " ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        return false;
    }
}

void SqlDatabaseSerivce::AppendUserIntoTable(const User &user)
{
    QSqlQuery query(m_db);
    const QString request=
            "DO $$ BEGIN IF EXISTS(SELECT * FROM " + m_usersTablePrefix+ " WHERE " +userIdCN + "= '" +user.userId +"' AND " +userNameCN + " = '" + user.name + "') THEN " +
            "UPDATE " + m_usersTablePrefix+ " SET "+ fcsCN + " = '"+user.FCS+"' , "+ rankCN + " = '"+ user.rank+ "', "+ roleCN + " = "+ QString::number(user.role)+ " WHERE "+ userIdCN + " = '" +user.userId +"' AND "+ userNameCN + " = '" +user.name +"'; "+
            "ELSE "
            "INSERT INTO " + m_usersTablePrefix+ " (" +userIdCN + ", " +userNameCN + ", "+ fcsCN + ", "+ rankCN + ", "+ roleCN + ") "+
            "VALUES ('"+ user.userId+ "',  '"+ user.name+ "', '"+ user.FCS+ "', '"+ user.rank+ "',  "+ QString::number(user.role)+ ");  "
                                                                                                                                   "END IF; "
                                                                                                                                   "END $$; ";
    if(query.exec(request))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO +  QStringLiteral(" Не получается добавить пользователя в таблицу. Запрос ")+query.lastQuery()+ QStringLiteral( " ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}


void SqlDatabaseSerivce::RemoveUserIntoTable(int roleId,const User &user)
{
    //БРЕД????
    QSqlQuery query(m_db);
    const QString request="DELETE FROM " + m_startupTablePrefix + QString::number(roleId) +
            " WHERE" + userIdCN +"=\'"+ user.userId +"\'";
    query.prepare(request);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается удалить из в таблицы пользователя. Запрос ") +query.lastQuery()+ QStringLiteral( " ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QList<User> SqlDatabaseSerivce::GetAllUsers()
{
    QSqlQueryModel sqlQueryModel(this);
    QList<User> userList;
    QSqlQuery query(m_db);
    const QString request="SELECT "+ userIdCN +" , "+ userNameCN +" , "+ fcsCN +" , "+ rankCN +" , "+ roleCN +" FROM "+m_usersTablePrefix;
    query.prepare(request);

    if(query.exec()){
        sqlQueryModel.setQuery(query);
        if(5==sqlQueryModel.columnCount())
        {
            for(int i = 0; i < sqlQueryModel.rowCount(); i++)
            {
                User user;
                GetStringFromMessage(user.userId, sqlQueryModel.record(i), 0);
                GetStringFromMessage(user.name, sqlQueryModel.record(i), 1);
                GetStringFromMessage(user.FCS, sqlQueryModel.record(i), 2);
                GetStringFromMessage(user.rank, sqlQueryModel.record(i), 3);
                user.role=sqlQueryModel.record(i).value(4).toInt();
                userList.append(user);
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается получить из в таблицы всех пользователей, в таблице должно быть 5 колонок ") +  QStringLiteral( " а в реальности их ") +QString::number(sqlQueryModel.columnCount())+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается получить из в таблицы всех пользователей ") +  QStringLiteral( " запрос ") +query.lastQuery()+ QStringLiteral( " ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
    return userList;
}

void SqlDatabaseSerivce::AppendStartupIntoRole(int roleId, const QString &exec)
{
    QSqlQuery query(m_db);
    const QString request="INSERT INTO "+ m_startupTablePrefix+QString::number(roleId)+
            " ( "+ startupPathCN + " ) VALUES (?)";
    query.prepare(request);
    query.bindValue(0, exec);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось записать  путь к программе для роли - ")  +  QStringLiteral( " программа: ") +exec +  QStringLiteral( " роль: ") + QString::number(roleId)  + QStringLiteral( " ошибка: ") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QStringList SqlDatabaseSerivce::GetAllRoleStartups(int roleId)
{

    QSqlQueryModel sqlQueryModel(this);
    QStringList listOfStartups;
    QSqlQuery query(m_db);
    query.prepare("SELECT " + startupPathCN + " FROM "+ m_startupTablePrefix +QString::number(roleId));
    if(query.exec())
    {
        sqlQueryModel.setQuery(query);
        qDebug()<<sqlQueryModel.columnCount();
        qDebug()<<sqlQueryModel.rowCount();
        if(1==sqlQueryModel.columnCount())
        {
            for(int i = 0; i < sqlQueryModel.rowCount(); i++)
            {
                QString startup;
                GetStringFromMessage(startup, sqlQueryModel.record(i), 0);
                listOfStartups.append(startup);
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество колонок не равно одному : ")  +  QStringLiteral( " колличество записей: ") + QString::number(sqlQueryModel.columnCount())+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось получить  все исполняемые файлы : ")  +  QStringLiteral( " роль: ") + QString::number(roleId)  +  QStringLiteral(" ошибка") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
    return listOfStartups;
}

void SqlDatabaseSerivce::GetAllRoleStartupsIntoModel(int roleId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT " + startupPathCN + " FROM "+ m_startupTablePrefix +QString::number(roleId));
    if(query.exec())
    {
        m_currentRoleModel->setQuery(query);
        qDebug()<<m_currentRoleModel->columnCount();
        qDebug()<<m_currentRoleModel->rowCount();
        if(1!=m_currentRoleModel->columnCount())
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество колонок не равно одному : ")  +  QStringLiteral( " колличество записей: ") + QString::number(m_currentRoleModel->columnCount())).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось получить  все пути для перезапускаемых программ: ")  +  QStringLiteral( " роль: ") + QString::number(roleId)  +  QStringLiteral(" ошибка") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QSqlQueryModel *SqlDatabaseSerivce::GetRoleStartupsModel()
{
    return m_currentRoleModel;
}

void SqlDatabaseSerivce::RemoveStartupIntoRole(int roleId, const QString &startupPath)
{
    QSqlQuery query;
    const QString request="DELETE FROM "+ m_startupTablePrefix + QString::number(roleId) +
            " WHERE "+ startupPathCN +"=\'"+ startupPath+ "\'";
    query.prepare(request);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось записать путь для перезапускаемой программы: ")  +  QStringLiteral( " роль: ") + QString::number(roleId) +  QStringLiteral( " путь : ") + startupPath  +  QStringLiteral(" ошибка") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::AppendDesktopIntoRole(int roleId,const DesktopEntity &entity)
{
    QSqlQuery query(m_db);
    const QString request="INSERT INTO "+ m_desktopTablePrefix+QString::number(roleId)+
            " ( " + desktopNameCN + " , "+ desktopTypeCN + " , "+ execPathCN + " , "+ iconPathCN+" ) "+
            "VALUES ( ? , ? , ? , ? )";
    query.prepare(request);
    query.bindValue(0, entity.name);
    query.bindValue(1, entity.type);
    query.bindValue(2, entity.exec);
    query.bindValue(3, entity.icon);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось записать файлы рабочего стола для роли: ")  +  QStringLiteral( " роль: ") + QString::number(roleId) +  QStringLiteral( " имя : ") + entity.name  +  QStringLiteral(" ошибка") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::RemoveDesktopIntoRole(int roleId, const QString &entityName)
{
    QSqlQuery query(m_db);

    const QString request="DELETE FROM "+m_desktopTablePrefix +QString::number(roleId) +
            " WHERE "+ desktopNameCN +"=\'" + entityName + "\'";
    query.prepare(request);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось удалить  файлы рабочего стола для роли: ")  +  QStringLiteral( " роль: ") + QString::number(roleId) +  QStringLiteral( " имя : ") + entityName  +  QStringLiteral(" ошибка") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QList<DesktopEntity> SqlDatabaseSerivce::GetAllRoleDesktops(int roleId)
{
    QList<DesktopEntity> listOfExecs;
    QSqlQueryModel sqlQueryModel(this);
    QSqlQuery query(m_db);
    query.prepare("SELECT " + desktopNameCN + " , "+ desktopTypeCN + " , "+ execPathCN + " , "+ iconPathCN + " FROM "+ m_desktopTablePrefix +QString::number(roleId));
    if(query.exec())
    {
        sqlQueryModel.setQuery(query);
        if(4==sqlQueryModel.columnCount())
        {
            for(int i = 0; i < sqlQueryModel.rowCount(); ++i)
            {
                DesktopEntity entity;
                GetStringFromMessage(entity.name, sqlQueryModel.record(i), 0);
                GetStringFromMessage(entity.type, sqlQueryModel.record(i), 1);
                GetStringFromMessage(entity.exec, sqlQueryModel.record(i), 2);
                GetStringFromMessage(entity.icon, sqlQueryModel.record(i), 3);
                listOfExecs.append(entity);
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество колонок не равно четырем : ")  +  QStringLiteral( " колличество записей: ") + QString::number(sqlQueryModel.columnCount())+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось получить  все файлы рабочего стола для роли: ")  +  QStringLiteral( " роль: ") + QString::number(roleId) +  QStringLiteral(" ошибка") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
    return listOfExecs;
}

QStringList SqlDatabaseSerivce::GetAllUsersWithRoleId(int roleId)
{
    Q_ASSERT(roleId>=0 && roleId<Roles.count());
    QSqlQueryModel sqlQueryModel(this);
    QStringList usersNamesWithRoleId;
    QSqlQuery query(m_db);
    query.prepare("SELECT " + userNameCN + " FROM "+ m_usersTablePrefix + " WHERE " + roleCN + " = " +QString::number(roleId));
    if(query.exec())
    {
        sqlQueryModel.setQuery(query);
        qDebug()<<sqlQueryModel.columnCount();
        qDebug()<<sqlQueryModel.rowCount();
        if(1==sqlQueryModel.columnCount())
        {
            for(int i = 0; i < sqlQueryModel.rowCount(); ++i)
            {
                QString userName;
                GetStringFromMessage(userName, sqlQueryModel.record(i), 0);
                usersNamesWithRoleId.append(userName);
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество колонок не равно одному : ")  +  QStringLiteral( " колличество записей: ") + QString::number(sqlQueryModel.columnCount())+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось получить  всех пользователей для роли: ")  +  QStringLiteral( " роль: ") + QString::number(roleId) +  QStringLiteral(" ошибка") + query.lastError().text()+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
    return usersNamesWithRoleId;
}

bool SqlDatabaseSerivce::GetBoolFromMessage(QSqlQuery &query)
{
    if (1==query.size())
    {
        query.next();
        if(QVariant::Bool==query.value(0).type())
        {
            return query.value(0).toBool();
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Тип текущей записи не булевый, а должен быть булевый")  +  QStringLiteral( " тип записи: ") + QString::number(query.value(0).type())+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество записей в запросе не равно одному")  +  QStringLiteral( " колличество записей: ") + QString::number(query.size())+ QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::GetStringFromMessage(QString &inputString, const QSqlRecord &record, const int &rowPos)
{
    if(record.count()>rowPos)
    {
        if(QVariant::String==record.value(rowPos).type())
        {
            inputString= record.value(rowPos).toString();
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Тип текущей записи не строка, а должен быть строкой")  +  QStringLiteral( " тип записи: ") + QString::number(record.value(rowPos).type())).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral("Колличество записей в запросе больше чем колличество строк")  +  QStringLiteral( " колличество строк: ") + QString::number(rowPos) +  QStringLiteral( " колличество записей в запросе: ") + QString::number(record.count())).toUtf8().constData());
    }
}

