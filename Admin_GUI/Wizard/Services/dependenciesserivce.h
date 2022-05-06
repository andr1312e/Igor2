#ifndef DEPENDENCIESSERIVCE_H
#define DEPENDENCIESSERIVCE_H
#include <QDomElement>
#include <QObject>

#include "Services/Terminals/terminal.h"

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
    void InstallDependencies();
    const QStringList &GetAllDependenciesList() const;
    int GetDependeciesCount() const;
    void SetNeedInstallState(bool state);
private:
    bool FixUp(QString &dependency);
    void CloseSynapticIfItRunned();
    void InstallPackage(const QString &packageName);
private:
    Terminal* const m_terminal;
    QStringList m_dependenciesList;
    bool m_needToInstall;
    const QString m_astraPackageManagerName;
    const QString m_astraPackageManagerNameService;

};

#endif // DEPENDENCIESSERIVCE_H
