#include "sqldatabaseserivce.h"

SqlDatabaseSerivce::SqlDatabaseSerivce(QObject *parent)
    : QObject(parent)
{
    qDebug()<< "DbDrivers" << QSqlDatabase::drivers();
}

SqlDatabaseSerivce::~SqlDatabaseSerivce()
{
    db->close();
    delete db;
}

bool SqlDatabaseSerivce::ConnectToDataBase(const QString &hostName, const quint16 &port, const QString &databaseName, const QString &userName, const QString &dbPassword)
{
    db=new QSqlDatabase(QSqlDatabase::addDatabase(postgeSqlDatabaseDriverStringKey));
    db->setHostName(hostName);
    db->setPort(port);
    db->setDatabaseName(databaseName);
    db->setUserName(userName);
    db->setPassword(dbPassword);
    bool isDbCreated= db->open(userName, dbPassword);
    if (isDbCreated)
    {
        return true;
    }
    else
    {
        qDebug()<< "Error" << "Database connection error" + db->lastError().text();
        return false;
    }
}

void SqlDatabaseSerivce::CreateUsersTableIfNotExists()
{
    QSqlQuery query;
    if (query.exec("CREATE TABLE IF NOT EXISTS " + accountsTableName +
                   " (id        SERIAL    PRIMARY KEY, "+
                   userIdCN  +" VARCHAR(100) NOT NULL, "+
                   userNameCN+" VARCHAR(100) NOT NULL, "+
                   fcsCN + "    VARCHAR(100) NOT NULL, "+
                   rankCN+ "    VARCHAR(100) NOT NULL, "+
                   roleCN+"     VARCHAR(100) NOT NULL)"))
    {
        return;
    }
    else
    {
        qDebug()<< "Error" << "Database error" + db->lastError().text();
        qDebug()<< "Error" << "query error" + query.lastError().text();
        qFatal("Не удалось создать бд с исполняемыми файлами");
    }
}

void SqlDatabaseSerivce::CreateExecsTableInNotExists(quint8 roleId)
{
    QSqlQuery query;
    QString request="CREATE TABLE IF NOT EXISTS " + execTablePrefix +QString::number(roleId)+
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
    query.prepare("SELECT "+ userNameCN + " FROM " + accountsTableName+
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

QString SqlDatabaseSerivce::GetUserFCS(QString &currentUserName)
{
    QSqlQuery query;
    query.prepare("SELECT " + fcsCN+ " FROM "+ accountsTableName+
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

QString SqlDatabaseSerivce::GetUserRank(QString &currentUserName)
{
    QSqlQuery query;
    query.prepare("SELECT " + rankCN+ " FROM "+ accountsTableName+
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

QString SqlDatabaseSerivce::GetUserRole(const QString &currentUserName)
{
    QSqlQuery query;
    query.prepare("SELECT " + roleCN+ " FROM "+ accountsTableName+
                  " WHERE "+ userNameCN +"=?");
    query.bindValue(0, currentUserName);
    if(query.exec())
    {
        if(query.next())
        {
            if(query.value(0).type()==QVariant::String)
            {
                QString role=query.value(0).toString();
                return role;
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

void SqlDatabaseSerivce::ClearTable(QString tableName)
{
    QSqlQuery query(*db);
    QString request=" DELETE FROM "+tableName;
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

void SqlDatabaseSerivce::ClearUserTable()
{
    ClearTable(accountsTableName);
}

void SqlDatabaseSerivce::ClearExecsTable(quint8 roleId)
{
    QSqlQuery query;
    ClearTable(execTablePrefix+QString::number(roleId));
}

void SqlDatabaseSerivce::ClearDesktopTable(quint8 roleId)
{
    QSqlQuery query;
    ClearTable(desktopTablePrefix+QString::number(roleId));
}

bool SqlDatabaseSerivce::CheckUserTable()
{
    QSqlQuery query;
    query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + accountsTableName+ "')");
    if(query.exec())
    {
        qDebug()<< query.size();
        return  GetBoolFromMessage(query);
    }
    else
    {
        qFatal("Cant execute check user table query");
    }
}

bool SqlDatabaseSerivce::CheckExecTables()
{
    bool result=true;
    for (int i=0; i<Roles.count(); ++i)
    {
        QSqlQuery query;
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + execTablePrefix +QString::number(i)+ "')");
        if(query.exec())
        {
            result=result&GetBoolFromMessage(query);
        }
        else
        {
            qFatal("Cant execute check user table query");
        }
    }
    return  result;
}

bool SqlDatabaseSerivce::ChekcDesktopTables()
{
    bool result=true;
    for (int i=0; i<Roles.count(); ++i)
    {
        QSqlQuery query;
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + desktopTablePrefix +QString::number(i)+ "')");
        if(query.exec())
        {
            result=result&GetBoolFromMessage(query);
        }
        else
        {
            qFatal("Cant execute check user table query");
        }
    }
    return  result;
}

void SqlDatabaseSerivce::AppendUserIntoTable(User &user)
{
    QSqlQuery query;
    QString request="INSERT INTO " + accountsTableName +
            " ( "+ userIdCN + " , " +userNameCN +" , " + fcsCN +" , " + rankCN +" , " + roleCN + ")"+
            " VALUES ( ?,           ?,                      ?,              ?,               ? )";
    query.prepare(request);
    query.bindValue(0, user.userId);
    query.bindValue(1, user.name);
    query.bindValue(2, user.FCS);
    query.bindValue(3, user.rank);
    query.bindValue(4, user.role);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal("Cant write user to database");
    }
}

void SqlDatabaseSerivce::RemoveUserIntoTable(quint8 roleId, User &user)
{
    QSqlQuery query;
    query.prepare("DELETE FROM " + execTablePrefix + QString::number(roleId) +
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
    QSqlQuery query;
    QString request="SELECT "+ userIdCN +" , "+ userNameCN +" , "+ fcsCN +" , "+ rankCN +" , "+ roleCN +" FROM "+accountsTableName;
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
                GetStringFromMessage(user.role, sqlQueryModel.record(i), 4);
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

void SqlDatabaseSerivce::AppendExecIntoRole(quint8 roleId, const QString &exec)
{
    QSqlQuery query;
    QString request="INSERT INTO "+ execTablePrefix+QString::number(roleId)+
            " ( "+ startupPathCN + " ) VALUES (?)";
    query.prepare(request);
    query.bindValue(0, exec);
    if(query.exec())
    {
        return;
    }
    else
    {
        qFatal(QString("Не удалось записать  путь к программе: " + exec  + " для роли: " + QString::number(roleId)).toLocal8Bit());
    }
}

QStringList SqlDatabaseSerivce::GetAllRoleExecs(quint8 roleId)
{
    QSqlQueryModel sqlQueryModel(this);
    QStringList listOfExecs;
    QSqlQuery query;
    query.prepare("SELECT " + startupPathCN + " FROM "+ execTablePrefix +QString::number(roleId));
    if(query.exec())
    {
        sqlQueryModel.setQuery(query);
        qDebug()<<sqlQueryModel.columnCount();
        qDebug()<<sqlQueryModel.rowCount();
        if(sqlQueryModel.columnCount()==1)
        {
            for(int i = 0; i < sqlQueryModel.rowCount(); i++)
            {
                QString exec;
                GetStringFromMessage(exec, sqlQueryModel.record(i), 0);
                listOfExecs.append(exec);
            }
        }
        else
        {
            qFatal("Wrong column count");
        }
    }
    else
    {
        qFatal(QString("Не удалось получить  все исполняемые файлы  для роли: " +QString::number(roleId)).toLocal8Bit());
    }
    return listOfExecs;
}

void SqlDatabaseSerivce::RemoveExecIntoRole(quint8 roleId, const QString &startupPath)
{

    QSqlQuery query;
    query.prepare("DELETE FROM "+ execTablePrefix + QString::number(roleId) +
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

void SqlDatabaseSerivce::AppendDesktopIntoRole(quint8 roleId, DesktopEntity &entity)
{
    QSqlQuery query;
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
        qFatal(QString("Не удалось записать  ярлык: " + entity.name  + " для роли: " +QString::number(roleId)).toLocal8Bit());
    }
}

void SqlDatabaseSerivce::RemoveDesktopIntoRole(quint8 roleId, DesktopEntity &entity)
{
    QSqlQuery query;
    query.prepare("DELETE FROM "+desktopTablePrefix +QString::number(roleId) +
                  "WHERE "+ desktopNameCN +"='?'");
    query.bindValue(0, entity.name);
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
    QSqlQuery query;
    query.prepare("SELECT " + desktopNameCN + " , "+ desktopTypeCN + " , "+ execPathCN + " , "+ iconPathCN + " FROM "+ desktopTablePrefix +QString::number(roleId));
    if(query.exec())
    {
        sqlQueryModel.setQuery(query);
        qDebug()<<sqlQueryModel.columnCount();
        qDebug()<<sqlQueryModel.rowCount();
        if(sqlQueryModel.columnCount()==4)
        {
            for(int i = 0; i < sqlQueryModel.rowCount(); i++)
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
        qFatal(QString("Не удалось получить  все ярлыки файлы  для роли: " +QString::number(roleId)).toLocal8Bit());
    }
    return listOfExecs;
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

void SqlDatabaseSerivce::GetStringFromMessage(QString &inputString,const QSqlRecord &record, int rowPos)
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

