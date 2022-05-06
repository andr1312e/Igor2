#include "wizardservice.h"

WizardService::WizardService(const QString &rlsTiFolder, const LoadingState &state, LinuxUserService *service, ISqlDatabaseService *iSqlDataBaseService, QObject *parent)
    : QObject(parent)
    , m_terminal(Terminal::GetTerminal())
    , m_dependenciesService(new DependenciesService(this))
    , m_filesAndFoldersSerivce(new FilesAndFoldersSerivce(rlsTiFolder))
    , m_rlsTiFolder(rlsTiFolder)
    , m_indexActionWithUserRepository(0)
    , m_actionWithRolesRepository({0, 0, 0, 0})
    , m_oldDataCurrentUserWizardRepositry(new UsersDataWizardRepository(service))
    , m_backupDataUserWizardRepositry(new UsersDataWizardRepository(service))
    , m_oldDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(m_terminal))
    , m_backupDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(m_terminal))
    , m_iSqlDatabaseService(iSqlDataBaseService)
    , m_globalTagName(QStringLiteral("settings"))
{
    ConnectObjects();
    GetExsistsRepositoriesData(state);
}

WizardService::~WizardService()
{
    delete m_dependenciesService;
    delete m_oldDataCurrentUserWizardRepositry;
    delete m_backupDataUserWizardRepositry;
    delete m_oldDataRolesAndStartupsWizardRepository;
    delete m_backupDataRolesAndStartupsWizardRepository;
}

void WizardService::ConnectObjects()
{
    connect(m_dependenciesService, &DependenciesService::ToPackageInstalling, this, &WizardService::ToCurrentActionDoing);
}

void WizardService::OnChangeInstallState(bool state)
{
    m_dependenciesService->SetNeedInstallState(state);
}

void WizardService::GetExsistsRepositoriesData(const LoadingState &state)
{
    switch (state) {
    case NoFiles:
        break;
    case NoRoleData:
        GetExsistsUsersListFromDb();
        GetExsistsExecsAndDesktopFilesFromDb();//мб что то из базы имеется
        break;

    case NoUserDb:
        GetExsistsExecsAndDesktopFilesFromDb();
        break;

    case Fine:
        Q_UNREACHABLE();
    }
}

void WizardService::GetExsistsUsersListFromDb()
{
    m_oldDataCurrentUserWizardRepositry->GetUsersFromDb(m_iSqlDatabaseService);
}

void WizardService::GetExsistsExecsAndDesktopFilesFromDb()
{
    m_oldDataRolesAndStartupsWizardRepository->GetRoleDesktopsAndStartupsFromDb(m_iSqlDatabaseService);
}

bool WizardService::CheckAndParseBackupFile(const QString &backupPath)
{
    const QByteArray backupByteArray = m_terminal->GetFileText(backupPath, Q_FUNC_INFO, true).toUtf8();
    QDomDocument backupXmlDocument;
    QString errorMessage;
    int errorLine=0, errorColumn=0;
    if (backupXmlDocument.setContent(backupByteArray, false, &errorMessage, &errorLine, &errorColumn)) {
        const QDomElement settings = backupXmlDocument.firstChildElement();

        if (settings.tagName() == m_globalTagName) {
            const QDomNodeList list(settings.childNodes());
            QStringList tagList;

            for (int i = 0; i < settings.childNodes().count(); ++i) {
                tagList.append(list.at(i).toElement().tagName());
            }

            if (m_backupCorrectTagsList==tagList) {
                m_backupFilePath=backupPath.left(backupPath.lastIndexOf('/')+1);
                ParseBackupFile(m_backupFilePath, list);
                return true;
            }
            else
            {
                Q_EMIT ToSendBackupParceError(QStringLiteral("Ошибка: Теги файла не соответствуют, должны быть: ") +m_backupCorrectTagsList.join(',') + QStringLiteral(" а в файле: ") + tagList.join(','));
                m_backupFilePath.clear();
            }
        }
        else
        {
            Q_EMIT ToSendBackupParceError(QStringLiteral("Ошибка: Глобальный тэг не соответствует, должен быть: ") +m_globalTagName + QStringLiteral(" а в файле: ") + settings.tagName());
        }
    }
    else
    {
        Q_EMIT ToSendBackupParceError(QStringLiteral("Ошибка: обработка файла неуспешно произведена: ") + errorMessage + QStringLiteral(" в строке: ") + QString::number(errorLine) + QStringLiteral(" на позиции: ") + QString::number(errorColumn));
    }
    return false;
}

const QString &WizardService::GetBackupFilePath() const
{
    return m_backupFilePath;
}

void WizardService::ClearBackup()
{
    m_backupDataUserWizardRepositry->Clear();
    m_backupDataRolesAndStartupsWizardRepository->Clear();
}

bool WizardService::HasUserData(DbWizardDataType dataType) const
{
    const UsersDataWizardRepository * const currentRepository=GetUserReposiotry(dataType);
    return currentRepository->HasData();
}

bool WizardService::HasAnyRolesData(DbWizardDataType dataType) const
{
    const RolesAndStartupsWizardRepository * const currentRepository=GetRolesRepositoryType(dataType);
    bool hasOldData=false;
    for (int roleId=0; roleId<Roles.count(); ++roleId)
    {
        hasOldData=hasOldData||currentRepository->HasData(roleId);
    }
    return hasOldData;
}

bool WizardService::HasRoleIdAnyData(DbWizardDataType dataType, int roleId) const
{
    const RolesAndStartupsWizardRepository *const currentRepository=GetRolesRepositoryType(dataType);
    return currentRepository->HasData(roleId);
}

void WizardService::ParseBackupFile(const QString &backupPath, const QDomNodeList &backupNodeLists)
{
    const QDomElement usersDomElement = backupNodeLists.at(0).toElement();
    m_backupDataUserWizardRepositry->GetUsersFromXmL(usersDomElement);

    for (int i = 0; i < Roles.count(); ++i)
    {
        const QDomElement roleDomElement = backupNodeLists.at(i+1).toElement();
        m_backupDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromBackup(i, roleDomElement, backupPath);
    }
    const QDomElement dependeciesDomElement=backupNodeLists.at(Roles.count()+1).toElement();
    m_dependenciesService->GetDependenciesFromBackUp(dependeciesDomElement);
    const QDomElement filesDomElement=backupNodeLists.at(backupNodeLists.length()-1).toElement();

    const QDomElement foldersDomElement=backupNodeLists.at(backupNodeLists.length()-1).toElement();

}

void WizardService::ApplyWizardActions()
{
    const int actionsCount=GetAllActionsCount();
    Q_EMIT ToSetActionsCount(actionsCount);
    ApplySettingsWithUserRepository(m_indexActionWithUserRepository, m_backupDataUserWizardRepositry, m_oldDataCurrentUserWizardRepositry);
    ApplySettingsWithRolesRepository(m_actionWithRolesRepository, m_backupDataRolesAndStartupsWizardRepository);
    ApplySettingsWithDependencies();
}

int WizardService::GetAllActionsCount() const
{
    int actionsCount=m_dependenciesService->GetDependeciesCount();
    QStringList programsNamesToCopy;
    for (int i=0; i<m_actionWithRolesRepository.count(); ++i)
    {
        if (1 == m_actionWithRolesRepository.at(i))
        {
            programsNamesToCopy.append(m_backupDataRolesAndStartupsWizardRepository->GetAllUniqueDesktopExecsAndStarups(i));
        }

    }
    if(!programsNamesToCopy.isEmpty())
    {
        programsNamesToCopy.removeDuplicates();
    }
    actionsCount+=programsNamesToCopy.count();
    return actionsCount;
}

void WizardService::ApplySettingsWithUserRepository(int actionIndexWithUserRepository, UsersDataWizardRepository *backupRepository, UsersDataWizardRepository *oldRepository)
{
    if (0 == actionIndexWithUserRepository) {
        oldRepository->SaveUsers(m_iSqlDatabaseService, true);
    } else {
        if (1 == actionIndexWithUserRepository) {
            backupRepository->SaveUsers(m_iSqlDatabaseService, false);
        } else {
            oldRepository->SaveUsers(m_iSqlDatabaseService, false);
        }
    }
}

void WizardService::ApplySettingsWithRolesRepository(const QVarLengthArray<int, 4> &actionsWithRoleRepository, RolesAndStartupsWizardRepository *backupRepository)
{
    QStringList programsNamesToCopy;
    for (int i=0; i<Roles.count(); ++i)
    {
        if (0 == actionsWithRoleRepository.at(i)) {
            m_iSqlDatabaseService->CreateStartupsTableIfNotExists(i);
            m_iSqlDatabaseService->ClearStartupsTable(i);
            m_iSqlDatabaseService->CreateDesktopRolesIfNotExists(i);
            m_iSqlDatabaseService->ClearDesktopTable(i);
        } else {
            if (1 == actionsWithRoleRepository.at(i)) {
                backupRepository->SaveRoleDesktopsToDb(m_iSqlDatabaseService, i);
                backupRepository->SaveRoleExecsToDb(m_iSqlDatabaseService, i);
                programsNamesToCopy.append(backupRepository->GetAllUniqueDesktopExecsAndStarups(i));
            }
        }
    }
    if(!programsNamesToCopy.isEmpty())
    {
        programsNamesToCopy.removeDuplicates();
        CopyFilesFromRoleToFolder(m_backupFilePath, programsNamesToCopy);
    }
}

void WizardService::ApplySettingsWithDependencies()
{
    m_dependenciesService->InstallDependencies();
}

void WizardService::CopyFilesFromRoleToFolder(const QString &sourceFolder, const QStringList &programs)
{
    if (m_terminal->IsDirNotExists(m_rlsTiFolder, Q_FUNC_INFO, true))
    {
        m_terminal->CreateFolder(m_rlsTiFolder, Q_FUNC_INFO, true);
    }
    for (const QString &program : programs)
    {
        const QString fullpath=sourceFolder+program;
        if(m_terminal->IsFileExists(m_rlsTiFolder+program, Q_FUNC_INFO, true))
        {
            m_terminal->DeleteFileSudo(m_rlsTiFolder+program, Q_FUNC_INFO);
        }
        Q_EMIT ToCurrentActionDoing(QStringLiteral("Копирование: ")+ program);
        m_terminal->CopyFileSudo(fullpath, m_rlsTiFolder, Q_FUNC_INFO);
    }
}

void WizardService::GetDataToViewFromUserRepository(DbWizardDataType dataType, QString &FCS, QList<User> &userList) const
{
    const UsersDataWizardRepository * const currentRepository=GetUserReposiotry(dataType);
    if (currentRepository->HasData()) {
        FCS = currentRepository->GetCurrentUserFCS();
        userList = currentRepository->GetUsersList();
    }
}

int WizardService::GetUserCountFromUserRepository(DbWizardDataType dataType) const
{
    const UsersDataWizardRepository *const userRepository=GetUserReposiotry(dataType);
    return userRepository->GetUserCount();
}

void WizardService::GetDataToViewFromDesktopRepository(DbWizardDataType dataType,int roleIndex, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs)
{
    const RolesAndStartupsWizardRepository * const currentRepository=GetRolesRepositoryType(dataType);
    if (currentRepository->HasData(roleIndex)) {
        currentRepository->GetRoleDesktopsAndStartupsFromLocalRepository(roleIndex, roleDesktops, roleExecs);
    }
}

int WizardService::GetAppsCountFromDesktopRepository(DbWizardDataType dataType,int roleIndex) const
{
    const RolesAndStartupsWizardRepository * const currentRepository=GetRolesRepositoryType(dataType);
    return currentRepository->GetRoleDesktopsAppCount(roleIndex);
}

void WizardService::SetActionWithUserRepository(int actionIndex)
{
    m_indexActionWithUserRepository = actionIndex;
}

int WizardService::GetActionIndexWithUserRepository() const
{
    return m_indexActionWithUserRepository;
}

void WizardService::SetActionWithRoleRepository(int roleIndex, int actionIndex)
{
    m_actionWithRolesRepository.replace(roleIndex, actionIndex);
}

int WizardService::GetActionWithRoleRepository(int roleIndex) const
{
    return m_actionWithRolesRepository.at(roleIndex);
}

const QStringList &WizardService::GetAllDependenciesList() const
{
    return m_dependenciesService->GetAllDependenciesList();
}

RolesAndStartupsWizardRepository *WizardService::GetRolesRepositoryType(DbWizardDataType dataType) const
{
    if (DbWizardDataType::OldData==dataType) {
        return m_oldDataRolesAndStartupsWizardRepository;
    } else {
        return m_backupDataRolesAndStartupsWizardRepository;
    }
}

UsersDataWizardRepository *WizardService::GetUserReposiotry(DbWizardDataType dataType) const
{
    if (DbWizardDataType::OldData==dataType) {
        return m_oldDataCurrentUserWizardRepositry;
    } else {
        return m_backupDataUserWizardRepositry;
    }
}
