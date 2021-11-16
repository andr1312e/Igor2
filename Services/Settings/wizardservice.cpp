#include "wizardservice.h"

WizardService::WizardService(LoadingState state, const QString &currentUserName, const QString &currentUserId, QStringList &validSettingsPaths, const QStringList &defaultSettingsValues, Terminal *terminal, QObject *parent)
    : QObject(parent)
    , m_validSettingsPaths(validSettingsPaths)
    , m_defaultSettingsValues(defaultSettingsValues)
    , m_resultSettingsPaths(QStringList({"", "", ""}))
    , m_terminal(terminal)
    , m_actionWithRolesRepository(QStringList({"", "", "", ""}))
    , m_oldDataCurrentUserWizardRepositry(new UsersDataWizardRepository(currentUserName, currentUserId, terminal))
    , m_backupDataUserWizardRepositry(new UsersDataWizardRepository(currentUserName, currentUserId, terminal))
    , m_oldDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(terminal))
    , m_backupDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(terminal))
{
    SetOldRepositoriesData(state, validSettingsPaths);
}

WizardService::~WizardService()
{
    delete m_oldDataCurrentUserWizardRepositry;
    delete m_backupDataUserWizardRepositry;
    delete m_oldDataRolesAndStartupsWizardRepository;
    delete m_backupDataRolesAndStartupsWizardRepository;
}

void WizardService::SetOldRepositoriesData(LoadingState &state, QStringList &validSettingsPaths)
{
    switch (state) {
    case CantRun:
        qFatal("Недопустимый код");

    case NoFiles:
        break;

    case NoRoleData:
        TryToSetCurrentUserOldsFcsAndRank(validSettingsPaths);
        break;

    case NoUserDb:
        TryToSetOldExecsAndDesktopFiles(validSettingsPaths);
        break;

    case Fine:
        qFatal("Недопустимый код");
    }
}

void WizardService::TryToSetCurrentUserOldsFcsAndRank(QStringList &validSettingsPaths)
{
    m_oldDataCurrentUserWizardRepositry->SetFCSAndRolesFromFile(validSettingsPaths.first());
}

void WizardService::TryToSetOldExecsAndDesktopFiles(QStringList &validSettingsPaths)
{
    m_oldDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromFile(validSettingsPaths.front(), validSettingsPaths.last());
}

bool WizardService::CheckAndParseBackupFile(const QString &backupPath)
{
    QByteArray arr = m_terminal->GetFileText(backupPath, "WizardService::CheckAndParseBackupFile", true).toUtf8();
    QDomDocument m_backupXMLDocument;

    if (m_backupXMLDocument.setContent(arr)) {
        QDomElement settings = m_backupXMLDocument.firstChildElement();

        if (settings.tagName() == "settings") {
            QDomNodeList list = settings.childNodes();
            QStringList tagList;

            for (int i = 0; i < settings.childNodes().count(); i++) {
                tagList.append(list.at(i).toElement().tagName());
            }

            if (tagList == m_backupCorrectTagsList) {
                ParseBackupFile(m_backupXMLDocument);
                return true;
            }
        }
    }

    return false;
}

bool WizardService::HasBackup() const
{
    return m_backupDataUserWizardRepositry->HasData() || m_backupDataRolesAndStartupsWizardRepository->HasData();
}

bool WizardService::HasOldData() const
{
    return m_oldDataCurrentUserWizardRepositry->HasData() || m_oldDataRolesAndStartupsWizardRepository->HasData();
}

void WizardService::ParseBackupFile(QDomDocument &backupXMLDocument)
{
    QDomElement settings = backupXMLDocument.firstChildElement();
    QDomElement users = settings.firstChildElement();
    m_backupDataUserWizardRepositry->GetFCSAndRolesFromXml(users);

    QDomNodeList usersAndRolesXmlList = settings.childNodes();
    for (int i = 0; i < Roles.count(); i++) {
        QDomElement role = usersAndRolesXmlList.at(i+1).toElement();
        m_backupDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromBackup(i, role);
    }
}

const QString &WizardService::ApplySettingsWithUserRepositoryAndReturnPath(const QString &actionWithUserRepository, const QStringList &validSettingsPaths, const QStringList &defautSettingsValues, UsersDataWizardRepository *backupRepository, UsersDataWizardRepository *oldRepository)
{
    if (userWizardPageComboBoxBackupAndOldDataActions.at(0) == actionWithUserRepository) {
        backupRepository->WriteUserRepositoryToFile(defautSettingsValues.first(), true);
        return defautSettingsValues.first();
    } else {
        if (userWizardPageComboBoxBackupAndOldDataActions.at(1) == actionWithUserRepository) {
            backupRepository->WriteUserRepositoryToFile(defautSettingsValues.first(), false);
            return defautSettingsValues.first();
        } else {
            oldRepository->WriteUserRepositoryToFile(validSettingsPaths.first(), false);
            return validSettingsPaths.first();
        }
    }
}

const QStringList WizardService::ApplySettingsWithRolesRepository(const QStringList &actionsWithRoleRepository, const QStringList &validSettingsPaths, const QStringList&defautSettingsValues, RolesAndStartupsWizardRepository *backupRepository, RolesAndStartupsWizardRepository *oldRepository)
{
    QStringList execsAndRolesPaths(GetPathsFromRolesAndExecs(actionsWithRoleRepository, validSettingsPaths, defautSettingsValues));
    for (int i=0; i<Roles.count(); ++i)
    {
        if (actionsWithRoleRepository.at(i) == m_rolesWizardPageComboBoxBackupAndOldDataActions.at(0)) {
            backupRepository->SaveRoleDesktops(execsAndRolesPaths.front(), i);
            backupRepository->SaveRoleStartups(execsAndRolesPaths.last(), i);
        } else {
            if (actionsWithRoleRepository.at(i) == m_rolesWizardPageComboBoxBackupAndOldDataActions.at(1)) {
                backupRepository->SaveRoleDesktops(execsAndRolesPaths.front(), i);
                backupRepository->SaveRoleStartups(execsAndRolesPaths.last(), i);
            }
            else
            {
                oldRepository->SaveRoleDesktops(execsAndRolesPaths.front(), i);
                oldRepository->SaveRoleStartups(execsAndRolesPaths.last(), i);
            }
        }
    }
    return execsAndRolesPaths;
}

const QStringList WizardService::GetPathsFromRolesAndExecs(const QStringList &actionsWithRoleRepository, const QStringList &validSettinsPath, const QStringList &defaultSettingsPaths) const
{
    QStringList resultSettingsPaths;
    if (m_validSettingsPaths.count()==2)
    {
        resultSettingsPaths.push_back(validSettinsPath.front());
        resultSettingsPaths.push_back(validSettinsPath.last());
        for (QStringList::const_iterator it=actionsWithRoleRepository.cbegin(); it!=actionsWithRoleRepository.cend(); ++it) {
            if (*it!=m_rolesWizardPageComboBoxBackupAndOldDataActions.last())
            {
                resultSettingsPaths.front()=defaultSettingsPaths.at(1);
                resultSettingsPaths.last()=defaultSettingsPaths.at(2);
            }
        }
    }
    else
    {
        resultSettingsPaths.push_back(defaultSettingsPaths.at(1));
        resultSettingsPaths.push_back(defaultSettingsPaths.at(2));
    }
    return resultSettingsPaths;
}

void WizardService::GetDataFromUserRepository(const bool isOldData, QString &FCS, QString &rank, QVector<User> &userList)
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
        currentRepository->RetunRoleDesktopsAndStartups(roleIndex, roleDesktops, roleExecs);
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

QStringList WizardService::ApplyWizard()
{
    QString newUserDbPath=ApplySettingsWithUserRepositoryAndReturnPath(m_actionWithUserRepository, m_validSettingsPaths, m_defaultSettingsValues, m_backupDataUserWizardRepositry, m_oldDataCurrentUserWizardRepositry);
    QStringList rolesAndExecsPaths=ApplySettingsWithRolesRepository(m_actionWithRolesRepository, m_validSettingsPaths, m_defaultSettingsValues, m_backupDataRolesAndStartupsWizardRepository, m_oldDataRolesAndStartupsWizardRepository);

    QStringList allPathsToReturn(newUserDbPath);
    allPathsToReturn.append(rolesAndExecsPaths);
    return allPathsToReturn;
}

QStringList& WizardService::GetResultSettingsPaths()
{
    return m_resultSettingsPaths;
}
