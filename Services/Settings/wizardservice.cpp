#include "wizardservice.h"

WizardService::WizardService(const QString &rlsTiFolder, LoadingState state, LinuxUserService *service, ISqlDatabaseService *iSqlDataBaseService, QObject *parent)
    : QObject(parent)
    , m_terminal(service->GetTerminal())
    , m_rlsTiFolder(rlsTiFolder)
    , m_actionWithRolesRepository(QStringList({"", "", "", ""}))
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

void WizardService::GetExsistsRepositoriesData(LoadingState &state)
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
    QByteArray backupByteArray = m_terminal->GetFileText(backupPath, "WizardService::CheckAndParseBackupFile", true).toUtf8();
    QDomDocument backupXmlDocument;
    QString errorMessage;
    int errorLine=0, errorColumn=0;
    if (backupXmlDocument.setContent(backupByteArray, false, &errorMessage, &errorLine, &errorColumn)) {
        QDomElement settings = backupXmlDocument.firstChildElement();

        if (settings.tagName() == "settings") {
            QDomNodeList list = settings.childNodes();
            QStringList tagList;

            for (int i = 0; i < settings.childNodes().count(); i++) {
                tagList.append(list.at(i).toElement().tagName());
            }

            if (tagList == m_backupCorrectTagsList) {
                m_backupFolder=backupPath.left(backupPath.lastIndexOf('/')+1);
                ParseBackupFile(m_backupFolder, backupXmlDocument);
                return true;
            }
            else
            {
                m_backupFolder.clear();
            }
        }
    }
    else
    {
        qDebug()<< errorMessage << " at line: " << errorLine << " at column: " << errorColumn;
    }
    return false;
}

bool WizardService::HasUserBackup() const
{
    return m_backupDataUserWizardRepositry->HasData();
}

bool WizardService::HasProgramBackUp() const
{
    return m_backupDataRolesAndStartupsWizardRepository->HasData();
}

bool WizardService::HasUserOldData() const
{
    return m_oldDataCurrentUserWizardRepositry->HasData();
}

bool WizardService::HastProgramOldData() const
{
    return m_oldDataRolesAndStartupsWizardRepository->HasData();
}

void WizardService::ParseBackupFile(const QString &backupPath, QDomDocument &backupXMLDocument)
{
    QDomElement settings = backupXMLDocument.firstChildElement();
    QDomElement users = settings.firstChildElement();
    m_backupDataUserWizardRepositry->GetFCSAndRolesFromXml(users);

    QDomNodeList usersAndRolesXmlList = settings.childNodes();
    for (int i = 0; i < Roles.count(); i++) {
        QDomElement role = usersAndRolesXmlList.at(i+1).toElement();
        m_backupDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromBackup(i, role, backupPath);
    }
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

void WizardService::ApplySettingsWithRolesRepository(const QStringList &actionsWithRoleRepository, RolesAndStartupsWizardRepository *backupRepository, RolesAndStartupsWizardRepository *oldRepository)
{
    QStringList programsNamesToCopy;
    for (int i=0; i<Roles.count(); ++i)
    {
        if (actionsWithRoleRepository.at(i) == m_rolesWizardPageComboBoxBackupAndOldDataActions.at(0)) {
            m_iSqlDatabaseService->ClearExecsTable(i);
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
        CopyFilesFromRoleToFolder(m_backupFolder, programsNamesToCopy);
    }
}

void WizardService::CopyFilesFromRoleToFolder(const QString &sourceFolder, const QStringList programs)
{
    if (m_terminal->IsDirNotExists(m_rlsTiFolder, "WizardService::CopyFilesFromRoleToFolder", true))
    {
        m_terminal->CreateFolder(m_rlsTiFolder, "WizardService::CopyFilesFromRoleToFolder", true);
    }
    for (const QString &program : programs)
    {
        QString fullpath=sourceFolder+program;
        if(m_terminal->IsFileExists(m_rlsTiFolder+program, "WizardService::CopyFilesFromRoleToFolder", true))
        {
            m_terminal->DeleteFileSudo(m_rlsTiFolder+program, "WizardService::CopyFilesFromRoleToFolder");
        }
        m_terminal->CopyFileSudo(fullpath, m_rlsTiFolder, "WizardService::CopyFilesFromRoleToFolder");
    }
}

void WizardService::GetDataFromUserRepository(const bool isOldData, QString &FCS, QString &rank, QList<User> &userList)
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

void WizardService::GetDataFromDesktopRepository(const int roleIndex, const bool isOldData, QList<DesktopEntity> &roleDesktops, QStringList &roleExecs)
{
    RolesAndStartupsWizardRepository *currentRepository;

    if (isOldData) {
        currentRepository = m_oldDataRolesAndStartupsWizardRepository;
    } else {
        currentRepository = m_backupDataRolesAndStartupsWizardRepository;
    }

    if (currentRepository->HasData()) {
        currentRepository->GetRoleDesktopsAndStartupsFromLocalRepository(roleIndex, roleDesktops, roleExecs);
    }
}

int WizardService::GetUserCountFromDesktopRepository(const int roleIndex, const bool isOldData)
{
    if (isOldData) {
        return m_oldDataRolesAndStartupsWizardRepository->GetRoleDesktopsAppCount(roleIndex);
    } else {
        return m_backupDataRolesAndStartupsWizardRepository->GetRoleDesktopsAppCount(roleIndex);
    }
}

void WizardService::OnSetActionWithUserRepository(const QString &actionWithUserRepository)
{
    m_actionWithUserRepository = actionWithUserRepository;
}

QString &WizardService::GetActionWithUserRepository()
{
    return m_actionWithUserRepository;
}

void WizardService::SetActionWithRoleRepository(const int roleIndex, const QString &actionWithRoleRepository)
{
    m_actionWithRolesRepository[roleIndex] = actionWithRoleRepository;
}

const QString &WizardService::GetActionWithRoleRepository(const int roleIndex)
{
    return m_actionWithRolesRepository.at(roleIndex);
}

void WizardService::ApplyWizardActions()
{
    ApplySettingsWithUserRepository(m_actionWithUserRepository, m_backupDataUserWizardRepositry, m_oldDataCurrentUserWizardRepositry);
    ApplySettingsWithRolesRepository(m_actionWithRolesRepository, m_backupDataRolesAndStartupsWizardRepository, m_oldDataRolesAndStartupsWizardRepository);
}
