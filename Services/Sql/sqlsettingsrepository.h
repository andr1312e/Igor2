#ifndef SQLSETTINGSREPOSITORY_H
#define SQLSETTINGSREPOSITORY_H

#include <QString>
#include <QDir>
#include <QSettings>

class SqlSettingsRepository
{
public:
    explicit SqlSettingsRepository();
    ~SqlSettingsRepository();
public:
    void LoadSettingsFile();
    void SaveSettingsFile();
public:
    const QString &GetHost() const noexcept;
    quint16 GetPort() const noexcept;
    const QString &GetDbName() const noexcept;
    const QString &GetUserName() const noexcept;
    const QString &GetUserPassword() const noexcept;
private:
    void CheckFields() noexcept;
private:
    const QStringList m_sqlKeywords;
    const QString m_iniFilePath;
    QString m_host;
    quint16 m_port;
    QString m_dbName;
    QString m_userName;
    QString m_userPassword;
};

#endif // SQLSETTINGSREPOSITORY_H
