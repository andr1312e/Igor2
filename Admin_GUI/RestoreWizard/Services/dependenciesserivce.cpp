#include "dependenciesserivce.h"

DependenciesService::DependenciesService(QObject *parent)
    : QObject(parent)
    , m_terminal(Terminal::GetTerminal())
    , m_mountChecker(new MountChecker())
    , m_needToInstall(true)
    , m_astraPackageManagerName(QStringLiteral("synaptic"))
    , m_astraPackageManagerNameService(QStringLiteral("synaptic-pkexec"))
{

}

DependenciesService::~DependenciesService()
{
    delete m_mountChecker;
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
    if (m_dependenciesList.isEmpty() || !m_needToInstall)
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Список пакетов для установки пуст"));
        return;
    }
    else
    {
        CloseSynapticIfItRunned();
        const QStringList installedInSystemPackages = m_terminal->GetAllInstalledPackageNames(Q_FUNC_INFO);
        const QStringList notInstalledInSystemPackages = m_terminal->GetAllNotInstalledPackageNames(Q_FUNC_INFO);
        CheckAndMountRepository();
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
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Пакет уже установлен: ") + dependency);
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
    if (!m_needToInstall)
    {
        return 0;
    }
    else
    {
        return m_dependenciesList.count();
    }
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
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Убиваем синаптик"));
        m_terminal->KillProcess(m_astraPackageManagerName, Q_FUNC_INFO);
    }
}

void DependenciesService::CheckAndMountRepository()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Проверка наличие исо образов"));
    if (m_mountChecker->HasMountFiles())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral("Образы в наличии. Проверка монтировки репозиториев"));
        if (!m_mountChecker->IsReposiotoryMounted(Q_FUNC_INFO))
        {
            m_mountChecker->MountRepository();
        }
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral("Образы отсутвуют. Необходимо поместить их в папку. Имена /opt/devel-smolensk-1.6.iso и /opt/smolensk-1.6.iso или /opt/boot/boot.iso и /opt/dev/dev.iso"));
        QMessageBox::critical(Q_NULLPTR, QStringLiteral("Ошибка поиска образов"), QStringLiteral("Образы отсутвуют. Необходимо поместить их в папку. Имена /opt/devel-smolensk-1.6.iso и /opt/smolensk-1.6.iso или /opt/boot/boot.iso и /opt/dev/dev.iso"));
        QApplication::exit();
    }
}

void DependenciesService::InstallPackage(const QString &packageName)
{
    Q_EMIT ToPackageInstalling(QStringLiteral("установка пакета: ") + packageName);
    QString outputInfo, errorInfo;
    m_terminal->InstallPackageSudo(packageName, outputInfo, errorInfo);

}
