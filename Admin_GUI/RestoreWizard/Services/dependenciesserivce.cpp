#include "dependenciesserivce.h"

DependenciesService::DependenciesService(QObject *parent)
    : QObject(parent)
    , m_terminal(Terminal::GetTerminal())
    , m_needToInstall(true)
    , m_astraPackageManagerName(QStringLiteral("synaptic"))
    , m_astraPackageManagerNameService(QStringLiteral("synaptic-pkexec"))
{

}

DependenciesService::~DependenciesService()
{

}

void DependenciesService::GetDependenciesFromBackUp(const QDomElement &dependenciedsXmlElement)
{
    m_dependenciesList.clear();
    const QDomNodeList dependencies = dependenciedsXmlElement.childNodes();
    for (int i = 0; i < dependencies.count(); ++i)
    {
        const QDomElement dependencyDomElement = dependencies.at(i).toElement();
        QString dependecyName(dependencyDomElement.text());
        if (IsFixable(dependecyName))
        {
            m_dependenciesList.push_back(dependecyName);
        }
    }
    m_dependenciesList.removeDuplicates();
    m_dependenciesList.removeAll(QString());
}

void DependenciesService::ClearDependencies()
{
    m_dependenciesList.clear();
}

void DependenciesService::InstallDependencies()
{
    if (m_dependenciesList.isEmpty())
    {
        return;
    }
    else
    {
        CloseSynapticIfItRunned();
        const QStringList installedInSystemPackages = m_terminal->GetAllInstalledPackageNames(Q_FUNC_INFO);
        const QStringList notInstalledInSystemPackages = m_terminal->GetAllNotInstalledPackageNames(Q_FUNC_INFO);
        for (const QString &dependency : qAsConst(m_dependenciesList))
        {
            if (notInstalledInSystemPackages.contains(dependency))
            {
                InstallPackage(dependency);
            }
            else
            {
                if (installedInSystemPackages.contains(dependency))
                {
                    continue;
                }
                else
                {
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не смогли  установить пакет ") + dependency + QStringLiteral(" так как его нет в списке пакетов доступных для установки"));
                }
            }
        }
    }
}

const QStringList &DependenciesService::GetAllDependenciesList() const noexcept
{
    return m_dependenciesList;
}

int DependenciesService::GetDependeciesCount() const noexcept
{
    return m_dependenciesList.count();
}

void DependenciesService::SetNeedInstallState(bool state)
{
    m_needToInstall = state;
}

bool DependenciesService::IsFixable(QString &dependency)
{
    if (dependency.front().isSpace() || dependency.back().isSpace())
    {
        dependency = dependency.trimmed();
    }
    return !dependency.contains(QChar(' '));
}

void DependenciesService::CloseSynapticIfItRunned()
{
    const QStringList allProcessesList(m_terminal->GetAllProcessList(Q_FUNC_INFO));
    if (allProcessesList.contains(m_astraPackageManagerName) || allProcessesList.contains(m_astraPackageManagerNameService))
    {
        m_terminal->KillProcess(m_astraPackageManagerName, Q_FUNC_INFO);
    }
}

void DependenciesService::InstallPackage(const QString &packageName)
{
    Q_EMIT ToPackageInstalling(QStringLiteral("установка пакета: ") + packageName);
    QString outputInfo, errorInfo;
    m_terminal->InstallPackageSudo(packageName, outputInfo, errorInfo);

}
