#include "sqldatabaseserivce.h"

SqlDatabaseSerivce::SqlDatabaseSerivce(QObject *parent)
    : QObject(parent)
    , m_postgeSqlDatabaseDriverStringKey(QStringLiteral("QPSQL"))
    , m_currentRoleModel(new QSqlQueryModel(this))
    , m_usersTablePrefix(QStringLiteral("rlstiusers"))
    , m_startupTablePrefix(QStringLiteral("rlstistartups"))
    , m_desktopTablePrefix(QStringLiteral("rlstidesktops"))
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Создаем сервис для работы с бд. Драйвера баз данных в системе: ") + QSqlDatabase::drivers().join(' '));
}

SqlDatabaseSerivce::~SqlDatabaseSerivce()
{
    if (m_db.isOpen())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Закрываем бд: "));
        m_db.close();
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Бд не была открыта "));
    }
    delete m_currentRoleModel;
}

void SqlDatabaseSerivce::OnDropDatabase()
{
    if (CheckUsersTable())
    {
        DropUsersTable();
    }
    for (int roleIndex = 0; roleIndex < Roles.count(); ++roleIndex)
    {
        DropDesktopsTable(roleIndex);
        DropStartupTable(roleIndex);
    }
}

DbConnectionState SqlDatabaseSerivce::ConnectToDataBase() noexcept
{
    SqlSettingsRepository settingsRepository;
    settingsRepository.LoadSettingsFile();
    const QString hostName = settingsRepository.GetHost();
    const quint16 port = settingsRepository.GetPort();
    const QString databaseName = settingsRepository.GetDbName();
    const QString userName = settingsRepository.GetUserName();
    const QString userPassword = settingsRepository.GetUserPassword();
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Подключаемся к бд hostName") + hostName + QStringLiteral(" port=") + QString::number(port) + QStringLiteral(" dbName=") + databaseName + QStringLiteral(" userName=") + userName + QStringLiteral(" password=") + userPassword);
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral("Доступные драйверы") + QSqlDatabase::drivers().join(','));
    m_db = QSqlDatabase(QSqlDatabase::addDatabase(m_postgeSqlDatabaseDriverStringKey));
    m_db.setHostName(hostName);
    m_db.setPort(port);
    m_db.setDatabaseName(databaseName);
    m_db.setUserName(userName);
    m_db.setPassword(userPassword);
    if (m_db.open())
    {
        settingsRepository.SaveSettingsFile();
        return DbConnectionState::Connected;
    }
    else
    {
        const QString errorString = m_db.lastError().text();
        const DbErrorState error = AnalyzeError(errorString, userName);
        return FixError(error, errorString, userName, userPassword);
    }
}

DbErrorState SqlDatabaseSerivce::AnalyzeError(const QString &errorString, const QString userName) const noexcept
{
    if (QStringLiteral("Driver not loaded Driver not loaded") == errorString)
    {
        return DbErrorState::NoDriver;
    }
    else
    {
        if (errorString.startsWith(QStringLiteral("не удалось подключится к серверу")))
        {
            return DbErrorState::NoPostgre;
        }
        else
        {
            if (errorString.startsWith(QStringLiteral("СБОЙ:  пользователь \"") + userName + QStringLiteral("\" не прошёл проверку")))
            {
                return DbErrorState::WrongPassword;
            }
            else
            {
                return DbErrorState::UnknownError;
            }
        }
    }
}

DbConnectionState SqlDatabaseSerivce::FixError(DbErrorState newError, const QString &errorString, const QString &userName, const QString &password)
{
    SqlProblemsFixer problemFixer;
    QString newErrorString;
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Ошибка подключения (код): ") + QString::number(static_cast<int>(newError)) + QStringLiteral(" ошибка: ") + errorString);
    switch (newError)
    {
    case DbErrorState::NoPostgre:
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не можем подключится к бд нет пакета постгре установленого"));
        Q_EMIT ToTrayMessage(QStringLiteral("Устанавливаем базу данных Postgre SQL. Пожалуйста подождите..."));
        if (problemFixer.InstallPostgreSqlAndDriver())
        {
            return DbConnectionState::NeedRestart;
        }
        break;

    }
    case DbErrorState::NoDriver:
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не можем подключится к бд нет драйвера установленного"));
        Q_EMIT ToTrayMessage(QStringLiteral("Устанавливаем драйвер базы данных Postgre SQL. Пожалуйста подождите... "));
        if (problemFixer.InstallSqlDriverForQt5())
        {
            return DbConnectionState::NeedRestart;
        }
        break;
    }
    case DbErrorState::WrongPassword:
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не можем подключится к бд неверный пароль и/или имя пользователя"));
        problemFixer.ResetPostgreUserPassword(userName, password);
        if (m_db.open())
        {
            return DbConnectionState::Connected;
        }
        else
        {
            newErrorString = m_db.lastError().text();
            problemFixer.SetLastError(AnalyzeError(newErrorString, userName));
        }
        break;
    }
    case DbErrorState::UnknownError:
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Невозможно запустить приложение, ошибка в процессе подключения к бд. Ошибка не известная. Проверьте пользоватлей, пакет драйвера к бд, и установлен ли пакет постгре. Переустановите их если ошибка не убирается"));
        QMessageBox::warning(Q_NULLPTR, QStringLiteral("Неизвестная ошибка"), QStringLiteral("смотрите ошибку в файле логов"));
        return DbConnectionState::FailedConnection;
    }
    default:
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Невозможно обработать ошибку так как обработчик не написан")).toUtf8().constData());
        return DbConnectionState::FailedConnection;
    }
    }

    if (problemFixer.GetLastError() == newError)//Ошибка снова
    {
        QString errorReason;
        switch (newError)
        {
        case DbErrorState::NoPostgre:
            errorReason = QStringLiteral(" не смогли установить постгрес: ") + newErrorString;
            break;
        case DbErrorState::NoDriver:
            errorReason = QStringLiteral(" не смогли установить драйвер постгре для кути:") + newErrorString;
            break;
        case DbErrorState::WrongPassword:
            errorReason = QStringLiteral(" не смогли поменять пароль для бд (нет пользователя postgres или смена пароля запрещена)") + newErrorString;
            break;
        case DbErrorState::UnknownError:
            errorReason = QStringLiteral(" неизвестная ошибка") + newErrorString;
            break;
        default:
            errorReason = QStringLiteral(" нереализованное поведение ") + newErrorString + ' ' + Q_FUNC_INFO;
            break;
        }
        QMessageBox::critical(Q_NULLPTR, QStringLiteral("Подключение к бд не удалось"), QStringLiteral("Причина: ") + errorReason);
    }
    return  DbConnectionState::FailedConnection;
}

void SqlDatabaseSerivce::CreateUsersTableIfNotExists()
{
    QSqlQuery query;
    if (query.exec("CREATE TABLE IF NOT EXISTS " + m_usersTablePrefix +
                   " (id        SERIAL    PRIMARY KEY, " +
                   CN::userId  + " VARCHAR(100) NOT NULL, " +
                   CN::userName + " VARCHAR(100) NOT NULL, " +
                   CN::fcs + "    VARCHAR(100) NOT NULL, " +
                   CN::role + "    INT NOT NULL)"))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось создать бд с пользователями. Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::CreateStartupsTableIfNotExists(int roleIndex)
{
    QSqlQuery query;
    const QString request = "CREATE TABLE IF NOT EXISTS " + m_startupTablePrefix + QString::number(roleIndex) +
                            " (id             SERIAL   PRIMARY KEY, " +
                            CN::startupPath + " VARCHAR(100) NOT NULL)";
    if (query.exec(request.toUtf8()))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось создать бд с файлами перезапуска стола. Роль: ") + QString::number(roleIndex) + QStringLiteral(" Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::CreateDesktopRolesIfNotExists(int roleIndex)
{
    QSqlQuery query;
    if (query.exec("CREATE TABLE IF NOT EXISTS " + m_desktopTablePrefix + QString::number(roleIndex) +
                   " (id             SERIAL    PRIMARY KEY, " +
                   CN::desktopName + " VARCHAR(100) NOT NULL, " +
                   CN::execPath + "  VARCHAR(100) NOT NULL, " +
                   CN::iconPath + "     VARCHAR(100));"))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось создать бд с файлами рабочего стола. Роль: ") + QString::number(roleIndex) + QStringLiteral(" Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::DropUsersTable()
{
    QSqlQuery query;
    if (query.exec("DROP TABLE " + m_usersTablePrefix))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось удалить бд с пользователями. Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::DropStartupTable(int roleIndex)
{
    QSqlQuery query;
    if (query.exec("DROP TABLE " + m_startupTablePrefix + QString::number(roleIndex)))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось удалить бд с файлами перезапуска. Роль: ") + QString::number(roleIndex) + QStringLiteral(" Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::DropDesktopsTable(int roleIndex)
{
    QSqlQuery query;
    if (query.exec("DROP TABLE " + m_desktopTablePrefix + QString::number(roleIndex)))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось удалить бд с файлами рабочего стола. Роль: ") + QString::number(roleIndex) + QStringLiteral(" Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QStringList SqlDatabaseSerivce::GetAdminsRoleUserName()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Получаем список имен администратора: "));
    QStringList adminsUserNameList;
    QSqlQuery query;
    query.prepare("SELECT " + CN::userName + " FROM " + m_usersTablePrefix +
                  " WHERE " + CN::role + " = '" + QString::number(Roles.count() - 1) + "'"
                 );
    if (query.exec())
    {
        while (query.next())
        {
            if (QVariant::String == query.value(0).type())
            {
                adminsUserNameList.append(query.value(0).toString());
            }
            else
            {
                qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не преобразовать имя пользователя в строку. Настоящий тип данных ") + QString::number(query.value(0).type())).toUtf8().constData());
            }
        }
        return  adminsUserNameList;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем получить список имен администратора, запрос не выполнен. Ошибка: ") + query.lastError().text().toUtf8().constData() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QString SqlDatabaseSerivce::GetUserFCS(const QString &currentUserName)
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Получаем ФИО пользователя: имя пользователя ") + currentUserName);
    QSqlQuery query;
    query.prepare("SELECT " + CN::fcs + " FROM " + m_usersTablePrefix +
                  " WHERE " + CN::userName + "=?");
    query.bindValue(0, currentUserName.simplified());
    if (query.exec())
    {
        if (query.next())
        {
            if (QVariant::String == query.value(0).type())
            {
                const QString fcs(query.value(0).toString());
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
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем получить ФИО пользователя, запрос не выполнен. Ошибка: ") + query.lastError().text().toUtf8().constData() + QStringLiteral(" имя пользователя ") + currentUserName).toUtf8().constData());
    }
}

int SqlDatabaseSerivce::GetUserRole(const QString &currentUserName)
{
    QSqlQuery query;
    query.prepare("SELECT " + CN::role + " FROM " + m_usersTablePrefix +
                  " WHERE " + CN::userName + "=?");
    query.bindValue(0, currentUserName);
    if (query.exec())
    {
        if (query.next())
        {
            if (QVariant::Int == query.value(0).type())
            {
                bool ok;
                const int role = query.value(0).toInt(&ok);
                if (ok)
                {
                    return role;
                }
                else
                {
                    qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем конвертировать в число. Число ") + QString::number(query.value(0).type()) + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
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
    const QString request = "SELECT " + CN::fcs + " FROM " + m_usersTablePrefix +
                            " WHERE " + CN::userName + "=\'" + currentUserName + "\'";
    query.prepare(request);
    if (query.exec())
    {
        if (query.next())
        {
            if (QVariant::String == query.value(0).type())
            {
                const QString fcs = query.value(0).toString();
                return fcs;
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
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем получить фио пользователя, запрос не выполнен. Ошибка: ") + query.lastError().text().toUtf8().constData() + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

int SqlDatabaseSerivce::GetUserRole(QStringView currentUserName)
{
    QSqlQuery query;
    const QString request = "SELECT " + CN::role + " FROM " + m_usersTablePrefix +
                            " WHERE " + CN::userName + "=\'" + currentUserName + "\'";
    query.prepare(request);
    if (query.exec())
    {
        if (query.next())
        {
            if (QVariant::Int == query.value(0).type())
            {
                bool ok;
                const int role = query.value(0).toInt(&ok);
                if (ok)
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
                qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем преобразовать роль пользователя в число : ") +  QStringLiteral( " настоящий тип данных ") + QString::number(query.value(0).type()) + QStringLiteral(" имя пользователя ") + currentUserName ).toUtf8().constData());
            }
        }
        else
        {
            return -1;
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не можем получить роль пользователя, запрос не выполнен. Ошибка: ") + query.lastError().text().toUtf8().constData() + QStringLiteral(" имя пользователя ") + currentUserName + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData() );
    }
}

void SqlDatabaseSerivce::ClearTable(const QString &tableName)
{
    QSqlQuery query(m_db);
    const QString request = QStringLiteral(" DELETE FROM ") + tableName;
    query.prepare(request);
    if (query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral("Не получается отчистить таблицу. Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::ClearUsersTable()
{
    ClearTable(m_usersTablePrefix);
}

void SqlDatabaseSerivce::ClearStartupsTable(int roleIndex)
{
    QSqlQuery query(m_db);
    ClearTable(m_startupTablePrefix + QString::number(roleIndex));
}

void SqlDatabaseSerivce::ClearDesktopTable(int roleIndex)
{
    QSqlQuery query(m_db);
    ClearTable(m_desktopTablePrefix + QString::number(roleIndex));
}

bool SqlDatabaseSerivce::CheckUsersTable()
{
    QSqlQuery query(m_db);
    query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_usersTablePrefix + "')");
    if (query.exec())
    {
        return  GetBoolFromMessage(query);
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается проверить наличилие таблицы с пользователями. Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

bool SqlDatabaseSerivce::CheckStartupTables()
{
    for (int i = 0; i < Roles.count(); ++i)
    {
        QSqlQuery query(m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_startupTablePrefix + QString::number(i) + "')");
        if (query.exec())
        {
            if (false == GetBoolFromMessage(query))
            {
                return false;
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается проверить наличилие таблицы с путями на автозагрузку программ. Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    return  true;
}

bool SqlDatabaseSerivce::CheckStartupTables(int roleIndex)
{
    if (0 <= roleIndex && Roles.count() > roleIndex)
    {
        QSqlQuery query(m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_startupTablePrefix + QString::number(roleIndex) + "')");
        if (query.exec())
        {
            return GetBoolFromMessage(query);
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается проверить наличилие таблицы с путями на автозагрузку программ. Роль ") + QString::number(roleIndex) + QStringLiteral( " ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        return false;
    }
}

bool SqlDatabaseSerivce::CheckDesktopTables()
{
    for (int i = 0; i < Roles.count(); ++i)
    {
        QSqlQuery query(m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_desktopTablePrefix + QString::number(i) + "')");
        if (query.exec())
        {
            if (false == GetBoolFromMessage(query))
            {
                return false;
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается проверить наличилие таблицы с ярлыками программ. Ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    return  true;
}

bool SqlDatabaseSerivce::CheckDesktopTables(int roleIndex)
{
    if (roleIndex >= 0 && roleIndex < Roles.count())
    {
        QSqlQuery query(m_db);
        query.prepare(" SELECT EXISTS (SELECT FROM pg_tables WHERE schemaname = 'public' AND tablename = '" + m_desktopTablePrefix + QString::number(roleIndex) + "')");
        if (query.exec())
        {
            return GetBoolFromMessage(query);
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается проверить наличилие таблицы с ярлыками программ. Роль: ") + QString::number(roleIndex) + QStringLiteral( " ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
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
    const QString request =
        "DO $$ BEGIN IF EXISTS(SELECT * FROM " + m_usersTablePrefix + " WHERE " + CN::userId + "= '" + user.GetUserId() + "' AND " + CN::userName + " = '" + user.GetUserName() + "') THEN " +
        "UPDATE " + m_usersTablePrefix + " SET " + CN::fcs + " = '" + user.GetUserFCS() + "', " + CN::role + " = " + QString::number(user.GetUserRole()) + " WHERE " + CN::userId + " = '" + user.GetUserId() + "' AND " + CN::userName + " = '" + user.GetUserName() + "'; " +
        "ELSE "
        "INSERT INTO " + m_usersTablePrefix + " (" + CN::userId + ", " + CN::userName + ", " + CN::fcs + ", " + CN::role + ") " +
        "VALUES ('" + user.GetUserId() + "',  '" + user.GetUserName() + "', '" + user.GetUserFCS() + "', '" + QString::number(user.GetUserRole()) + "');  "
        "END IF; "
        "END $$; ";
    if (query.exec(request))
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO +  QStringLiteral(" Не получается добавить пользователя в таблицу. Запрос ") + query.lastQuery() + QStringLiteral( " ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}


void SqlDatabaseSerivce::RemoveUserIntoTable(int roleIndex, const User &user)
{
    QSqlQuery query(m_db);
    const QString request = "DELETE FROM " + m_startupTablePrefix + QString::number(roleIndex) +
                            " WHERE" + CN::userId + "=\'" + user.GetUserId() + "\'";
    query.prepare(request);
    if (query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается удалить из в таблицы пользователя. Запрос ") + query.lastQuery() + QStringLiteral( " ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QList<User> SqlDatabaseSerivce::GetAllUsers()
{
    QSqlQueryModel sqlQueryModel(this);
    QList<User> userList;
    QSqlQuery query(m_db);
    const QString request = "SELECT " + CN::userId + " , " + CN::userName + " , " + CN::fcs + " , " + CN::role + " FROM " + m_usersTablePrefix;
    query.prepare(request);

    if (query.exec())
    {
        sqlQueryModel.setQuery(query);
        if (4 == sqlQueryModel.columnCount())
        {
            for (int i = 0; i < sqlQueryModel.rowCount(); i++)
            {
                const User user(GetStringFromMessage(sqlQueryModel.record(i), 0), GetStringFromMessage(sqlQueryModel.record(i), 1), GetStringFromMessage(sqlQueryModel.record(i), 2), sqlQueryModel.record(i).value(3).toInt());
                userList.append(user);
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается получить из в таблицы всех пользователей, в таблице должно быть четыре колоноки ") +  QStringLiteral( " а в реальности их ") + QString::number(sqlQueryModel.columnCount()) + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не получается получить из в таблицы всех пользователей ") +  QStringLiteral( " запрос ") + query.lastQuery() + QStringLiteral( " ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
    return userList;
}

void SqlDatabaseSerivce::AppendStartupIntoRole(int roleIndex, const QString &exec)
{
    QSqlQuery query(m_db);
    const QString request = "INSERT INTO " + m_startupTablePrefix + QString::number(roleIndex) +
                            " ( " + CN::startupPath + " ) VALUES (?)";
    query.prepare(request);
    query.bindValue(0, exec);
    if (query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось записать  путь к программе для роли - ")  +  QStringLiteral( " программа: ") + exec +  QStringLiteral( " роль: ") + QString::number(roleIndex)  + QStringLiteral( " ошибка: ") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QStringList SqlDatabaseSerivce::GetAllRoleStartups(int roleIndex)
{

    QSqlQueryModel sqlQueryModel(this);
    QStringList listOfStartups;
    QSqlQuery query(m_db);
    query.prepare("SELECT " + CN::startupPath + " FROM " + m_startupTablePrefix + QString::number(roleIndex));
    if (query.exec())
    {
        sqlQueryModel.setQuery(query);
        qDebug() << sqlQueryModel.columnCount();
        qDebug() << sqlQueryModel.rowCount();
        if (1 == sqlQueryModel.columnCount())
        {
            for (int i = 0; i < sqlQueryModel.rowCount(); ++i)
            {
                const QString startup = GetStringFromMessage(sqlQueryModel.record(i), 0);
                listOfStartups.append(startup);
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество колонок не равно одному : ")  +  QStringLiteral( " колличество записей: ") + QString::number(sqlQueryModel.columnCount()) + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось получить  все исполняемые файлы : ")  +  QStringLiteral( " роль: ") + QString::number(roleIndex)  +  QStringLiteral(" ошибка") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
    return listOfStartups;
}

void SqlDatabaseSerivce::GetAllRoleStartupsIntoModel(int roleIndex)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT " + CN::startupPath + " FROM " + m_startupTablePrefix + QString::number(roleIndex));
    if (query.exec())
    {
        m_currentRoleModel->setQuery(query);
        if (1 != m_currentRoleModel->columnCount())
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество колонок не равно одному : ")  +  QStringLiteral( " колличество записей: ") + QString::number(m_currentRoleModel->columnCount())).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось получить  все пути для перезапускаемых программ: ")  +  QStringLiteral( " роль: ") + QString::number(roleIndex)  +  QStringLiteral(" ошибка") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QSqlQueryModel *SqlDatabaseSerivce::GetRoleStartupsModel()
{
    return m_currentRoleModel;
}

void SqlDatabaseSerivce::RemoveStartupIntoRole(int roleIndex, const QString &startupPath)
{
    QSqlQuery query;
    const QString request = "DELETE FROM " + m_startupTablePrefix + QString::number(roleIndex) +
                            " WHERE " + CN::startupPath + "=\'" + startupPath + "\'";
    query.prepare(request);
    if (query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось записать путь для перезапускаемой программы: ")  +  QStringLiteral( " роль: ") + QString::number(roleIndex) +  QStringLiteral( " путь : ") + startupPath  +  QStringLiteral(" ошибка") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::AppendDesktopIntoRole(int roleIndex, const DesktopEntity &entity)
{
    QSqlQuery query(m_db);
    const QString request = "INSERT INTO " + m_desktopTablePrefix + QString::number(roleIndex) +
                            " ( " + CN::desktopName + " , " + CN::execPath + " , " + CN::iconPath + " ) " +
                            "VALUES ( '" + entity.GetName() + "' , '" + entity.GetExec() + "', '" + entity.GetIcon() + "' )";
    query.prepare(request);
    if (query.exec())
    {
        return;
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось записать файлы рабочего стола для роли: ")  +  QStringLiteral( " роль: ") + QString::number(roleIndex) +  QStringLiteral( " имя : ") + entity.GetName()  +  QStringLiteral(" ошибка") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

void SqlDatabaseSerivce::RemoveDesktopIntoRole(int roleIndex, const QString &entityName)
{
    QSqlQuery query(m_db);

    const QString request = "DELETE FROM " + m_desktopTablePrefix + QString::number(roleIndex) +
                            " WHERE " + CN::desktopName + "=\'" + entityName + "\'";
    query.prepare(request);
    if (!query.exec())
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось удалить  файлы рабочего стола для роли: ")  +  QStringLiteral( " роль: ") + QString::number(roleIndex) +  QStringLiteral( " имя : ") + entityName  +  QStringLiteral(" ошибка") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QList<DesktopEntity> SqlDatabaseSerivce::GetAllRoleDesktops(int roleIndex)
{
    QList<DesktopEntity> listOfExecs;
    QSqlQueryModel sqlQueryModel(this);
    QSqlQuery query(m_db);
    query.prepare("SELECT " + CN::desktopName + " , " + CN::execPath + " , " + CN::iconPath + " FROM " + m_desktopTablePrefix + QString::number(roleIndex));
    if (query.exec())
    {
        sqlQueryModel.setQuery(query);
        if (3 == sqlQueryModel.columnCount())
        {
            for (int i = 0; i < sqlQueryModel.rowCount(); ++i)
            {
                const DesktopEntity entity(GetStringFromMessage(sqlQueryModel.record(i), 0), GetStringFromMessage(sqlQueryModel.record(i), 1), GetStringFromMessage(sqlQueryModel.record(i), 2));
                listOfExecs.append(entity);
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество колонок не равно четырем : ")  +  QStringLiteral( " колличество записей: ") + QString::number(sqlQueryModel.columnCount()) + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось получить  все файлы рабочего стола для роли: ")  +  QStringLiteral( " роль: ") + QString::number(roleIndex) +  QStringLiteral(" ошибка") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
    return listOfExecs;
}

QStringList SqlDatabaseSerivce::GetAllUsersWithRoleIndex(int roleIndex)
{
    Q_ASSERT(User::RoleIsValid(roleIndex));
    QSqlQueryModel sqlQueryModel(this);
    QStringList usersNamesWithroleIndex;
    QSqlQuery query(m_db);
    query.prepare("SELECT " + CN::userName + " FROM " + m_usersTablePrefix + " WHERE " + CN::role + " = " + QString::number(roleIndex));
    if (query.exec())
    {
        sqlQueryModel.setQuery(query);
        if (1 == sqlQueryModel.columnCount())
        {
            for (int i = 0; i < sqlQueryModel.rowCount(); ++i)
            {
                const QString userName = GetStringFromMessage(sqlQueryModel.record(i), 0);
                usersNamesWithroleIndex.append(userName);
            }
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество колонок не равно одному : ")  +  QStringLiteral( " колличество записей: ") + QString::number(sqlQueryModel.columnCount()) + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Не удалось получить  всех пользователей для роли: ")  +  QStringLiteral( " роль: ") + QString::number(roleIndex) +  QStringLiteral(" ошибка") + query.lastError().text() + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
    return usersNamesWithroleIndex;
}

bool SqlDatabaseSerivce::GetBoolFromMessage(QSqlQuery &query) noexcept
{
    if (1 == query.size())
    {
        query.next();
        if (QVariant::Bool == query.value(0).type())
        {
            return query.value(0).toBool();
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Тип текущей записи не булевый, а должен быть булевый")  +  QStringLiteral( " тип записи: ") + QString::number(query.value(0).type()) + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Колличество записей в запросе не равно одному")  +  QStringLiteral( " колличество записей: ") + QString::number(query.size()) + QStringLiteral( " запрос: ") + query.lastQuery()).toUtf8().constData());
    }
}

QString SqlDatabaseSerivce::GetStringFromMessage(const QSqlRecord &record, int rowPos)
{
    if (record.count() > rowPos)
    {
        if (QVariant::String == record.value(rowPos).type())
        {
            return record.value(rowPos).toString();
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

