#ifndef DEPENDENCIESSERIVCE_H
#define DEPENDENCIESSERIVCE_H
#include <QDomElement>
#include <QObject>
#include <QMessageBox>
#include <QApplication>

#include "Logging/logger.h"
#include "Services/Terminals/terminal.h"
#include "Services/mountchecker.h"

class DependenciesService : public QObject
{
    Q_OBJECT
public:
    explicit DependenciesService(QObject *parent);
    ~DependenciesService();
Q_SIGNALS:
    void ToPackageInstalling(const QString &packageName);
public:
    void GetDependenciesFromBackUp(const QDomElement &dependenciedsXmlElement);
    void ClearDependencies();
    void InstallDependencies();
    const QStringList &GetAllDependenciesList() const noexcept;
    int GetDependeciesCount() const noexcept;
    void SetNeedInstallState(bool state);
private:
    bool IsFixable(QString &dependency);
    void CloseSynapticIfItRunned();
    void CheckAndMountRepository();
    void InstallPackage(const QString &packageName);
private:
    Terminal *const m_terminal;
    MountChecker *const m_mountChecker;
    QStringList m_dependenciesList;
    bool m_needToInstall;
    const QString m_astraPackageManagerName;
    const QString m_astraPackageManagerNameService;

};

#endif // DEPENDENCIESSERIVCE_H
