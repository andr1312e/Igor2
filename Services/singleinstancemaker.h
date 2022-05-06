#ifndef SERVICES_SINGLEINSTANCEMAKER_H
#define SERVICES_SINGLEINSTANCEMAKER_H

#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QCryptographicHash>

#include "Logging/logger.h"
//http://habrahabr.ru/post/173281/
class SingleInstanceMaker
{
public:
    explicit SingleInstanceMaker(const QString& sharedMemoryKey);
    ~SingleInstanceMaker();
public:
    bool TryToExecute();
    void ConnectToExsistsApp();
private:
    bool IsAnotherRunning();

    void Release();
    QString GenerateKeyHash( const QString& key, const QString& salt ) const;

    const QString m_strKey;
    const QString m_strMemLockKey;
    const QString m_strSharedMemKey;

    QSharedMemory    m_SharedMem;
    QSystemSemaphore m_lockSysSemph;

    Q_DISABLE_COPY( SingleInstanceMaker )
};

#endif // SERVICES_SINGLEINSTANCEMAKER_H
