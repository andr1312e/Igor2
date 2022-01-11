#include "sqldatabaseserivce.h"

SqlDatabaseSerivce::SqlDatabaseSerivce(QObject *parent)
    : QObject(parent)
{
    qDebug()<< "DbDrivers" << QSqlDatabase::drivers();
}

SqlDatabaseSerivce::~SqlDatabaseSerivce()
{
    if(m_db->isOpen())
    {
        m_db->close();
    }
    delete m_db;
}

bool SqlDatabaseSerivce::ConnectToDataBase(const QString &hostName, const quint16 &port, const QString &databaseName, const QString &userName, const QString &dbPassword)
{
    m_db=new QSqlDatabase(QSqlDatabase::addDatabase(postgeSqlDatabaseDriverStringKey));
    m_db->setHostName(hostName);
    m_db->setPort(port);
    m_db->setDatabaseName(databaseName);
    m_db->setUserName(userName);
    m_db->setPassword(dbPassword);
    bool isDbCreated= m_db->open(userName, dbPassword);
    if (isDbCreated)
    {
        return true;
    }
    else
    {
        qDebug()<< "Error" << "Database connection error" + m_db->lastError().text();
        return false;
    }
}

void SqlDatabaseSerivce::CreateUsersTableIfNotExists()
{
    QSqlQuery query;
    if (query.exec("CREATE TABLE IF NOT EXISTS " + usersTablePrefix +
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
        qDebug()<< QStringLiteral("Database error") + m_db->lastError().text();
        qDebug()<< QStringLiteral("Query error") << query.lastError().text();
        qFatal("Не удалось создать бд с исполняемыми файлами");
    }
}

void SqlDatabaseSerivce::CreateStartupsTableInNotExists(quint8 roleId)
{
    QSqlQuery query;
    QString request="CREATE TABLE IF NOT EXISTS " + startupTablePrefix +QString::number(roleId)+
            " (id             SERIAL   PRIMARY KEY, "+
            startupPathCN + " VARCHAR(100) NOT NULL)";
    if (query.exec(request.toLatin1()))
    {
        return;
    }
    else
    {
        qDebug()<< QString("Не удалось создать бд с исполняемыми файлами" + query.lastError().text());
        qFatal("msg");
    }
}

void SqlDatabaseSerivce::CreateDesktopRolesIfNotExists(quint8 roleId)
{
    QSqlQuery query;
    if(query.exec("CREATE TABLE IF NOT EXISTS " + desktopTablePrefix +QString::number(roleId)+
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
        qFatal("Не удалось создать бд с ярлыками");
    }
}

QStringList SqlDatabaseSerivce::GetAdminsRoleUserName()
{
    QStringList adminsUserNameList;
    QSqlQuery query;
    query.prepare("SELECT "+ userNameCN + " FROM " + usersTablePrefix+
                  " WHERE " + rankCN + " = '" + Roles.at(Roles.count()-1) + "'"
                  );
    if(query.exec())
    {
        while(query.next())
        {
            if(query.value(0).type()==QVariant::String)
            {
                adminsUserNameList.append(query.value(0).toString());
            }
            else
            {
                qFatal("Unvalid type");
            }
        }
        return  adminsUserNameList;
    }
    else
    {
        qFatal("Cant get list of admins in db");
    }
}

QString SqlDatabaseSerivce::GetUserFCS(const QString &currentUserName)
{
    QSqlQuery query;
    query.prepare("SELECT " + fcsCN+ " FROM "+ usersTablePrefix+
                  " WHERE "+ userNameCN +"=?");
    query.bindValue(0, currentUserName.simplified());
    if(query.exec())
    {
        if(query.next())
        {
            if(query.value(0).type()==QVariant::String)
            {
                QString fcs=query.value(0).toString();
                return fcs;
            }
            else
            {
                qFatal("Name type is incorrect");
            }
        }
        else
        {
            return "";
        }
    }
    else
    {
        qFatal("Cant delete from database exec");
    }
}

QString SqlDatabaseSerivce::GetUserRank(const QString &currentUserName)
{
    QSqlQuery query;
    query.prepare("SELECT " + rankCN+ " FROM "+ usersTablePrefix+
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
                qFatal("Name type is incorrect");
            }
        }
        else
        {
            return "";
        }
    }
    else
    {
        qFatal("Cant delete from database exec");
    }
}

int SqlDatabaseSerivce::GetUserRole(const QString &currentUserName)
{
    QSqlQuery query;
    query.prepare("SELECT " + roleCN+ " FROM "+ usersTablePrefix+
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
                    qFatal("Can't convert to int string value");
                }
            }
            else
            {
                qFatal("Name type is incorrect");
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        qFatal("Can't delete from database exec");
    }
}

void SqlDatabaseSerivce::ClearTable(QString tableName)
{
    QSqlQuery query(*m_db);
    QString request=QStringLiteral(" DELETE FROM ")+tableName;
    query.prepare(request);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("Can't clear database");
    }
}

void SqlDatabaseSerivce::ClearUsersTable()
{
    ClearTable(usersTablePrefix);
}

void SqlDatabaseSerivce::ClearStartupsTable(quint8 roleId)
{
    QSqlQuery query(*m_db);
    ClearTable(startupTablePrefix+QString::number(roleId));
}

void SqlDatabaseSerivce::ClearDesktopTable(quint8 roleId)
{
    QSqlQuery query(*m_db);
    ClearTable(desktopTablePrefix+QString::number(roleId));
}

bool SqlDatabaseSerivce::CheckUsersTable()
{
    QSqlQuery query(*m_db);
    query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + usersTablePrefix+ "')");
    if(query.exec())
    {
        return  GetBoolFromMessage(query);
    }
    else
    {
        qFatal("Cant execute check user table query");
    }
}

bool SqlDatabaseSerivce::CheckStartupTables()
{
    for (int i=0; i<Roles.count(); ++i)
    {
        QSqlQuery query(*m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + startupTablePrefix +QString::number(i)+ "')");
        if(query.exec())
        {
            if(false==GetBoolFromMessage(query))
            {
                return false;
            }
        }
        else
        {
            qFatal("Cant execute check user table query");
        }
    }
    return  true;
}

bool SqlDatabaseSerivce::CheckStartupTables(quint8 roleId)
{
    if(roleId>=0 && roleId<Roles.count())
    {
        QSqlQuery query(*m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + startupTablePrefix +QString::number(roleId)+ "')");
        if(query.exec())
        {
            return GetBoolFromMessage(query);
        }
        else
        {
            qFatal("Cant execute check user table query");
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
        QSqlQuery query(*m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + desktopTablePrefix +QString::number(i)+ "')");
        if(query.exec())
        {
            if(false==GetBoolFromMessage(query))
            {
                return false;
            }
        }
        else
        {
            qFatal("Cant execute check user table query");
        }
    }
    return  true;
}

bool SqlDatabaseSerivce::CheckDesktopTables(quint8 roleId)
{
    if(roleId>=0 && roleId<Roles.count())
    {
        QSqlQuery query(*m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + desktopTablePrefix +QString::number(roleId)+ "')");
        if(query.exec())
        {
            return GetBoolFromMessage(query);
        }
        else
        {
            qFatal("Cant execute check user table query");
        }
    }
    else
    {
        return false;
    }
}

void SqlDatabaseSerivce::AppendUserIntoTable(const User &user)
{
    QSqlQuery query(*m_db);
    QString request=
            "DO $$ BEGIN IF EXISTS(SELECT * FROM " + usersTablePrefix+ " WHERE " +userIdCN + "= '" +user.userId +"' AND " +userNameCN + " = '" + user.name + "') THEN " +
            "UPDATE " + usersTablePrefix+ " SET "+ fcsCN + " = '"+user.FCS+"' , "+ rankCN + " = '"+ user.rank+ "', "+ roleCN + " = "+ QString::number(user.role)+ " WHERE "+ userIdCN + " = '" +user.userId +"' AND "+ userNameCN + " = '" +user.name +"'; "+
            "ELSE "
            "INSERT INTO " + usersTablePrefix+ " (" +userIdCN + ", " +userNameCN + ", "+ fcsCN + ", "+ rankCN + ", "+ roleCN + ") "+
            "VALUES ('"+ user.userId+ "',  '"+ user.name+ "', '"+ user.FCS+ "', '"+ user.rank+ "',  "+ QString::number(user.role)+ ");  "
                                                                                                                                   "END IF; "
                                                                                                                                   "END $$; ";
    if(query.exec(request))
    {
        return;
    }
    else
    {
        qDebug()<< query.executedQuery();
        qDebug()<< query.lastQuery();
        qDebug()<< query.lastError().text();
        qFatal("Cant write user to database");
    }
}


void SqlDatabaseSerivce::RemoveUserIntoTable(quint8 roleId,const User &user)
{
    QSqlQuery query(*m_db);
    query.prepare("DELETE FROM " + startupTablePrefix + QString::number(roleId) +
                  " WHERE" + userIdCN +"='?'");
    query.bindValue(0, user.userId);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("Can't delete user from dataBase");
    }
}

QList<User> SqlDatabaseSerivce::GetAllUsers()
{
    QSqlQueryModel sqlQueryModel(this);
    QList<User> userList;
    QSqlQuery query(*m_db);
    QString request="SELECT "+ userIdCN +" , "+ userNameCN +" , "+ fcsCN +" , "+ rankCN +" , "+ roleCN +" FROM "+usersTablePrefix;
    query.prepare(request);

    if(query.exec()){
        sqlQueryModel.setQuery(query);
        if(sqlQueryModel.columnCount()==5)
        {
            for(int i = 0; i < sqlQueryModel.rowCount(); i++)
            {
                User user;
                GetStringFromMessage(user.userId, sqlQueryModel.record(i), 0);
                GetStringFromMessage(user.name, sqlQueryModel.record(i), 1);
                GetStringFromMessage(user.FCS, sqlQueryModel.record(i), 2);
                GetStringFromMessage(user.rank, sqlQueryModel.record(i), 3);
                user.role=sqlQueryModel.record(i).value(4).toInt();
                userList.push_back(user);
            }
        }
        else
        {
            qFatal("Column count != 5");
        }
    }
    else
    {
        qFatal("can't execute");
    }
    return userList;
}

void SqlDatabaseSerivce::AppendStartupIntoRole(quint8 roleId, const QString &exec)
{
    QSqlQuery query(*m_db);
    QString request="INSERT INTO "+ startupTablePrefix+QString::number(roleId)+
            " ( "+ startupPathCN + " ) VALUES (?)";
    query.prepare(request);
    query.bindValue(0, exec);
    if(query.exec())
    {
        return;
    }
    else
    {
        const QString errorMessage=QString("Не удалось записать  путь к программе: %1 для роли: %2").arg(exec).arg(roleId);
        qFatal("%s", errorMessage.toUtf8().data());
    }
}

QStringList SqlDatabaseSerivce::GetAllRoleStartups(quint8 roleId)
{
    QSqlQueryModel sqlQueryModel(this);
    QStringList listOfStartups;
    QSqlQuery query(*m_db);
    query.prepare("SELECT " + startupPathCN + " FROM "+ startupTablePrefix +QString::number(roleId));
    if(query.exec())
    {
        sqlQueryModel.setQuery(query);
        qDebug()<<sqlQueryModel.columnCount();
        qDebug()<<sqlQueryModel.rowCount();
        if(sqlQueryModel.columnCount()==1)
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
            qFatal("Wrong column count");
        }
    }
    else
    {
        const QString errorMessage=QString("Не удалось получить  все исполняемые файлы  для роли: " ).arg(roleId);
        qFatal("%s", errorMessage.toUtf8().constData());
    }
    return listOfStartups;
}

void SqlDatabaseSerivce::RemoveStartupIntoRole(quint8 roleId, const QString &startupPath)
{

    QSqlQuery query;
    query.prepare("DELETE FROM "+ startupTablePrefix + QString::number(roleId) +
                  " WHERE "+ startupPathCN +"='?'");
    query.bindValue(0, startupPath);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("Cant delete from database exec");
    }
}

void SqlDatabaseSerivce::AppendDesktopIntoRole(quint8 roleId,const DesktopEntity &entity)
{
    QSqlQuery query(*m_db);
    QString request="INSERT INTO "+ desktopTablePrefix+QString::number(roleId)+
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
        qDebug()<< query.lastError();
        const QString stringError=QString("Не удалось записать  ярлык: %1 для роли: %2").arg(entity.name).arg(roleId);
        qFatal("%s", stringError.toUtf8().constData());
    }
}

void SqlDatabaseSerivce::RemoveDesktopIntoRole(quint8 roleId, const QString &entityName)
{
    QSqlQuery query(*m_db);
    query.prepare("DELETE FROM "+desktopTablePrefix +QString::number(roleId) +
                  "WHERE "+ desktopNameCN +"='?'");
    query.bindValue(0, entityName);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("Cant delete desktop from table");
    }
}

QList<DesktopEntity> SqlDatabaseSerivce::GetAllRoleDesktops(quint8 roleId)
{
    QList<DesktopEntity> listOfExecs;
    QSqlQueryModel sqlQueryModel(this);
    QSqlQuery query(*m_db);
    query.prepare("SELECT " + desktopNameCN + " , "+ desktopTypeCN + " , "+ execPathCN + " , "+ iconPathCN + " FROM "+ desktopTablePrefix +QString::number(roleId));
    if(query.exec())
    {
        sqlQueryModel.setQuery(query);
        qDebug()<<sqlQueryModel.columnCount();
        qDebug()<<sqlQueryModel.rowCount();
        if(sqlQueryModel.columnCount()==4)
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
            qFatal("Wrong column count");
        }
    }
    else
    {
        const QString errorMessage=QString("Не удалось получить  все ярлыки файлы  для роли: %1").arg(roleId);
        qFatal("%s", errorMessage.toUtf8().constData());
    }
    return listOfExecs;
}

QStringList SqlDatabaseSerivce::GetAllUsersWithRoleId(quint8 roleId)
{
    Q_ASSERT(roleId>=0 && roleId<Roles.count());
    QSqlQueryModel sqlQueryModel(this);
    QStringList usersNamesWithRoleId;
    QSqlQuery query(*m_db);
    query.prepare("SELECT " + userNameCN + " FROM "+ usersTablePrefix + " WHERE " + roleCN + " = " +QString::number(roleId));
    if(query.exec())
    {
        sqlQueryModel.setQuery(query);
        qDebug()<<sqlQueryModel.columnCount();
        qDebug()<<sqlQueryModel.rowCount();
        if(sqlQueryModel.columnCount()==1)
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
            qFatal("Wrong column count");
        }
    }
    else
    {
        const QString errorMessage=QString("Не удалось получить  всех пользователей для роли: " ).arg(roleId);
        qFatal("%s", errorMessage.toUtf8().constData());
    }
    return usersNamesWithRoleId;
}

bool SqlDatabaseSerivce::GetBoolFromMessage(QSqlQuery &query)
{
    if (1==query.size())
    {
        query.next();
        if(query.value(0).type()==QVariant::Bool)
        {
            return query.value(0).toBool();
        }
        else
        {
            qFatal("Value must be bool, but it is not bool");
        }
    }
    else
    {
        qFatal("Wrong message size");
    }
}

void SqlDatabaseSerivce::GetStringFromMessage(QString &inputString, const QSqlRecord &record, const int &rowPos)
{
    if(record.count()>rowPos)
    {
        if(record.value(rowPos).type()==QVariant::String)
        {
            inputString= record.value(rowPos).toString();
        }
        else
        {
            qFatal("Wrong message type");
        }
    }
    else
    {
        qFatal("Wrong message size");
    }
}

