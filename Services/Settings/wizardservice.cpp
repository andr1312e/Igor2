#include "wizardservice.h"

WizardService::WizardService(const QString &rlsTiFolder, const LoadingState &state, LinuxUserService *service, ISqlDatabaseService *iSqlDataBaseService, QObject *parent)
    : QObject(parent)
    , m_terminal(service->GetTerminal())
    , m_dependenciesService(QSharedPointer<DependenciesService>(new DependenciesService(m_terminal)))
    , m_rlsTiFolder(rlsTiFolder)
    , m_actionWithUserRepository(userWizardPageComboBoxNoDataActions.front())
    , m_actionWithRolesRepository(QStringList({m_rolesWizardPageComboBoxNoDataActions.front(), m_rolesWizardPageComboBoxNoDataActions.front(), m_rolesWizardPageComboBoxNoDataActions.front(), m_rolesWizardPageComboBoxNoDataActions.front()}))
    , m_oldDataCurrentUserWizardRepositry(new UsersDataWizardRepository(service))
    , m_backupDataUserWizardRepositry(new UsersDataWizardRepository(service))
    , m_oldDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(m_terminal))
    , m_backupDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(m_terminal))
    , m_iSqlDatabaseService(iSqlDataBaseService)
{
    GetExsistsRepositoriesData(state);
}

WizardService::~WizardService()
{
    delete m_oldDataCurrentUserWizardRepositry;
    delete m_backupDataUserWizardRepositry;
    delete m_oldDataRolesAndStartupsWizardRepository;
    delete m_backupDataRolesAndStartupsWizardRepository;
}

void WizardService::GetExsistsRepositoriesData(const LoadingState &state)
{
    switch (state) {
    case CantRun:
        Q_UNREACHABLE();

    case NoFiles:
        break;

    case NoRoleData:
        GetExsistsUsersListFromDb();
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
    m_oldDataCurrentUserWizardRepositry->GetExsistsUsersListFromDb(m_iSqlDatabaseService);
}

void WizardService::GetExsistsExecsAndDesktopFilesFromDb()
{
    m_oldDataRolesAndStartupsWizardRepository->GetRoleDesktopsAndStartupsFromDb(m_iSqlDatabaseService);
}

bool WizardService::CheckAndParseBackupFile(const QString &backupPath)
{
    const QByteArray backupByteArray = m_terminal->GetFileText(backupPath, "WizardService::CheckAndParseBackupFile", true).toUtf8();
    QDomDocument backupXmlDocument;
    QString errorMessage;
    int errorLine=0, errorColumn=0;
    if (backupXmlDocument.setContent(backupByteArray, false, &errorMessage, &errorLine, &errorColumn)) {
        const QDomElement settings = backupXmlDocument.firstChildElement();

        if (settings.tagName() == m_globalTagName) {
            const QDomNodeList list = settings.childNodes();
            QStringList tagList;

            for (int i = 0; i < settings.childNodes().count(); i++) {
                tagList.append(list.at(i).toElement().tagName());
            }

            if (m_backupCorrectTagsList==tagList) {
                m_backupFolderPath=backupPath.left(backupPath.lastIndexOf('/')+1);
                ParseBackupFile(m_backupFolderPath, backupXmlDocument);
                return true;
            }
            else
            {
                m_backupFolderPath.clear();
            }
        }
    }
    else
    {
        qDebug()<< errorMessage << " at line: " << errorLine << " at column: " << errorColumn;
    }
    return false;
}

bool WizardService::HasUserData(bool isOldData) const
{
    UsersDataWizardRepository *currentRepository;

    if (isOldData) {
        currentRepository = m_oldDataCurrentUserWizardRepositry;
    } else {
        currentRepository = m_backupDataUserWizardRepositry;
    }
    return currentRepository->HasData();
}

bool WizardService::HasAnyRolesData(bool isOldData) const
{
    RolesAndStartupsWizardRepository *currentRepository;

    if (isOldData) {
        currentRepository = m_oldDataRolesAndStartupsWizardRepository;
    } else {
        currentRepository = m_backupDataRolesAndStartupsWizardRepository;
    }
    bool hasOldData=false;
    for (int roleId=0; roleId<Roles.count(); ++roleId)
    {
        hasOldData=hasOldData||currentRepository->HasData(roleId);
    }
    return hasOldData;
}

bool WizardService::HasRoleIdAnyData(bool isOldData, quint8 roleId) const
{
    RolesAndStartupsWizardRepository *currentRepository;

    if (isOldData) {
        currentRepository = m_oldDataRolesAndStartupsWizardRepository;
    } else {
        currentRepository = m_backupDataRolesAndStartupsWizardRepository;
    }
    return currentRepository->HasData(roleId);
}

void WizardService::ParseBackupFile(const QString &backupPath, const QDomDocument &backupXMLDocument)
{
    const QDomElement settingsDomElement = backupXMLDocument.firstChildElement();
    const QDomElement usersDomElement = settingsDomElement.firstChildElement();
    m_backupDataUserWizardRepositry->SetUsersFromBackup(usersDomElement);

    const QDomNodeList usersAndRolesXmlList = settingsDomElement.childNodes();
    for (int i = 0; i < Roles.count(); i++)
    {
        const QDomElement role = usersAndRolesXmlList.at(i+1).toElement();
        m_backupDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromBackup(i, role, backupPath);
    }
    const QDomElement backupDomElement=settingsDomElement.lastChildElement();
    m_dependenciesService->GetDependenciesFromBackUp(backupDomElement);
}

void WizardService::ApplySettingsWithUserRepository(const QString &actionWithUserRepository, UsersDataWizardRepository *backupRepository, UsersDataWizardRepository *oldRepository)
{
    if (userWizardPageComboBoxBackupAndOldDataActions.at(0) == actionWithUserRepository) {
        oldRepository->WriteUserRepositoryToDB(m_iSqlDatabaseService, true);
    } else {
        if (userWizardPageComboBoxBackupAndOldDataActions.at(1) == actionWithUserRepository) {
            backupRepository->WriteUserRepositoryToDB(m_iSqlDatabaseService, false);
        } else {
            oldRepository->WriteUserRepositoryToDB(m_iSqlDatabaseService, false);
        }
    }
}

void WizardService::ApplySettingsWithRolesRepository(const QStringList &actionsWithRoleRepository, RolesAndStartupsWizardRepository *backupRepository)
{
    QStringList programsNamesToCopy;
    for (int i=0; i<Roles.count(); ++i)
    {
        if (actionsWithRoleRepository.at(i) == m_rolesWizardPageComboBoxBackupAndOldDataActions.at(0)) {
            m_iSqlDatabaseService->CreateStartupsTableInNotExists(i);
            m_iSqlDatabaseService->ClearStartupsTable(i);
            m_iSqlDatabaseService->CreateDesktopRolesIfNotExists(i);
            m_iSqlDatabaseService->ClearDesktopTable(i);
        } else {
            if (actionsWithRoleRepository.at(i) == m_rolesWizardPageComboBoxBackupAndOldDataActions.at(1)) {
                backupRepository->SaveRoleDesktopsToDb(m_iSqlDatabaseService, i);
                backupRepository->SaveRoleExecsToDb(m_iSqlDatabaseService, i);
                programsNamesToCopy.append(backupRepository->GetAllUniqueDesktopExecsAndStarups(i));
            }
        }
    }
    if(!programsNamesToCopy.isEmpty())
    {
        programsNamesToCopy.removeDuplicates();
        CopyFilesFromRoleToFolder(m_backupFolderPath, programsNamesToCopy);
    }
}

void WizardService::ApplySettingsWithDependencies()
{
    m_dependenciesService->InstallDependencies();
}

void WizardService::CopyFilesFromRoleToFolder(const QString &sourceFolder, const QStringList programs)
{
    if (m_terminal->IsDirNotExists(m_rlsTiFolder, "WizardService::CopyFilesFromRoleToFolder", true))
    {
        m_terminal->CreateFolder(m_rlsTiFolder, "WizardService::CopyFilesFromRoleToFolder", true);
    }
    for (const QString &program : programs)
    {
        const QString fullpath=sourceFolder+program;
        if(m_terminal->IsFileExists(m_rlsTiFolder+program, "WizardService::CopyFilesFromRoleToFolder", true))
        {
            m_terminal->DeleteFileSudo(m_rlsTiFolder+program, "WizardService::CopyFilesFromRoleToFolder");
        }
        m_terminal->CopyFileSudo(fullpath, m_rlsTiFolder, "WizardService::CopyFilesFromRoleToFolder");
    }
}

void WizardService::GetDataToViewFromUserRepository(const bool isOldData, QString &FCS, QString &rank, QList<User> &userList)
{
    UsersDataWizardRepository *currentRepository;

    if (isOldData) {
        currentRepository = m_oldDataCurrentUserWizardRepositry;
    } else {
        currentRepository = m_backupDataUserWizardRepositry;
    }

    if (currentRepository->HasData()) {
        FCS = currentRepository->GetCurrentUserFCS();
        rank = currentRepository->GetCurrentUserRank();
        userList = currentRepository->GetUsersList();
    }
}

int WizardService::GetUserCountFromUserRepository(const bool isOldData) const
{
    if (isOldData) {
        return m_oldDataCurrentUserWizardRepositry->GetUserCount();
    } else {
        return m_backupDataUserWizardRepositry->GetUserCount();
    }
}

void WizardService::GetDataToViewFromDesktopRepository(const int roleIndex, const bool isOldData, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs)
{
    RolesAndStartupsWizardRepository *currentRepository;


    if (currentRepository->HasData(roleIndex)) {
        currentRepository->GetRoleDesktopsAndStartupsFromLocalRepository(roleIndex, roleDesktops, roleExecs);
    }
}

int WizardService::GetAppsCountFromDesktopRepository(const int roleIndex, const bool isOldData) const
{
    if (isOldData) {
        return m_oldDataRolesAndStartupsWizardRepository->GetRoleDesktopsAppCount(roleIndex);
    } else {
        return m_backupDataRolesAndStartupsWizardRepository->GetRoleDesktopsAppCount(roleIndex);
    }
}

void WizardService::SetActionWithUserRepository(const QString &actionWithUserRepository)
{
    m_actionWithUserRepository = actionWithUserRepository;
}

const QString &WizardService::GetActionWithUserRepository() const
{
    return m_actionWithUserRepository;
}

void WizardService::SetActionWithRoleRepository(const int roleIndex, const QString &actionWithRoleRepository)
{
    Q_ASSERT(roleIndex>=0 && roleIndex<Roles.count());
    m_actionWithRolesRepository.replace(roleIndex, actionWithRoleRepository);
}

const QString &WizardService::GetActionWithRoleRepository(const int roleIndex) const
{
    return m_actionWithRolesRepository.at(roleIndex);
}

void WizardService::ApplyWizardActions()
{
    ApplySettingsWithUserRepository(m_actionWithUserRepository, m_backupDataUserWizardRepositry, m_oldDataCurrentUserWizardRepositry);
    ApplySettingsWithRolesRepository(m_actionWithRolesRepository, m_backupDataRolesAndStartupsWizardRepository);
    ApplySettingsWithDependencies();
}
