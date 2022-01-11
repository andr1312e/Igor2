#include "dependenciesserivce.h"

DependenciesService::DependenciesService()
    : m_terminal(Terminal::GetTerminal())
    , m_dependenciesList(new QStringList())
{

}

DependenciesService::~DependenciesService()
{
    delete m_dependenciesList;
}

void DependenciesService::GetDependenciesFromBackUp(const QDomElement &dependenciedsXmlElement)
{
    m_dependenciesList->clear();
    const QDomNodeList dependencies=dependenciedsXmlElement.childNodes();
    for (int i=0; i<dependencies.count(); ++i)
    {
        const QDomElement dependencyDomElement=dependencies.at(i).toElement();

        QString dependecyName=dependencyDomElement.text();
        if(ValidateDependency(dependecyName))
        {
            m_dependenciesList->push_back(dependecyName);
        }
    }
    m_dependenciesList->removeDuplicates();
    m_dependenciesList->removeAll("");
}

void DependenciesService::InstallDependencies()
{
    if(m_dependenciesList->isEmpty())
    {
        return;
    }
    else
    {
        CloseSynapticIfItRunned();
        const QStringList installedInSystemPackages=m_terminal->GetAllInstalledPackageNames(Q_FUNC_INFO);
        const QStringList notInstalledInSystemPackages=m_terminal->GetAllNotInstalledPackageNames(Q_FUNC_INFO);
        for (const QString &dependency: *m_dependenciesList)
        {
            if(notInstalledInSystemPackages.contains(dependency))
            {
                InstallPackage(dependency);
            }
            else
            {
                if(installedInSystemPackages.contains(dependency))
                {
                    continue;
                }
                else
                {
                    qDebug()<< QString("can't find package %1").arg(dependency);
                }
            }
        }
    }
}

const QStringList &DependenciesService::GetAllDependenciesList() const
{
    return  *m_dependenciesList;
}

bool DependenciesService::ValidateDependency(QString &dependency)
{
    if(dependency.startsWith(QChar(' ')) || dependency.endsWith(QChar(' ')))
    {
        dependency=dependency.trimmed();
    }
    return !dependency.contains(QChar(' '));
}

void DependenciesService::CloseSynapticIfItRunned()
{
    const QStringList allProcessesList=m_terminal->GetAllProcessList(Q_FUNC_INFO);
    if(allProcessesList.contains(m_astraPackageManagerName) || allProcessesList.contains(m_astraPackageManagerNameService))
    {
        m_terminal->KillProcess(m_astraPackageManagerName, Q_FUNC_INFO);
    }
}

void DependenciesService::InstallPackage(const QString &packageName)
{
    m_terminal->InstallPackageSudo(packageName, Q_FUNC_INFO);
}
