#ifndef DEPENDENCIESSERIVCE_H
#define DEPENDENCIESSERIVCE_H
#include <QDomElement>

#include "Services/Terminals/terminal.h"

class DependenciesService
{
public:
    DependenciesService(Terminal* const terminal);
    ~DependenciesService();
public:
    void GetDependenciesFromBackUp(const QDomElement &dependenciedsXmlElement);
    void InstallDependencies();
private:
    bool ValidateDependency(QString &dependency);
    void CloseSynapticIfItRunned();
    void InstallPackage(const QString &packageName);
private:
    Terminal* const m_terminal;
    QStringList m_dependenciesList;

    const QString m_astraPackageManagerName="synaptic";
    const QString m_astraPackageManagerNameService="synaptic-pkexec";

};

#endif // DEPENDENCIESSERIVCE_H
