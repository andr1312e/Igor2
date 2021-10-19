#include "wizardservice.h"

WizardService::WizardService(ProgramFilesState state, QString &currentUserName, QString currentUserId, QStringList validSettingsPaths, QStringList defaultValues, Terminal *terminal, QObject *parent)
    : QObject(parent)
    , m_oldDataCurrentUserWizardRepositry(new CurrentUserWizardRepository(currentUserName))
    , m_backupDataUserWizardRepositry(new CurrentUserWizardRepository(currentUserName))
    , m_oldDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(terminal))
    , m_backupDataRolesAndStartupsWizardRepository(new RolesAndStartupsWizardRepository(terminal))
    , m_backupXmlNodesList({ "USERS" , "FIRSTROLE" , "SECONDROLE" , "THIRDROLE" , "FOURTHROLE"})
{
    SetOldRepositoriesData(state, currentUserName, validSettingsPaths, terminal);
}

WizardService::~WizardService()
{
    delete m_oldDataCurrentUserWizardRepositry;
    delete m_backupDataUserWizardRepositry;
    delete m_oldDataRolesAndStartupsWizardRepository;
    delete m_backupDataRolesAndStartupsWizardRepository;
}

void WizardService::SetOldRepositoriesData(ProgramFilesState state, QString &currentUserName, QStringList &validSettingsPaths, Terminal *terminal)
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
    m_oldDataCurrentUserWizardRepositry->setFCSAndRolesFromDb(validSettingsPaths.first());
}

void WizardService::TryToSetOldExecsAndDesktopFiles(QStringList &validSettingsPaths)
{
    m_oldDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromFile(validSettingsPaths.front(), validSettingsPaths.last());
}

bool WizardService::CheckAndParseBackupFile(QString &backupPath)
{
    QFile m_backupFile;
    m_backupFile.setFileName(backupPath);
    if (m_backupFile.open(QIODevice::ReadOnly))
    {
        QByteArray ar=m_backupFile.readAll();
        m_backupFile.close();
        QDomDocument m_backupXMLDocument;
        m_backupXMLDocument.setContent(ar);
        QDomElement settings= m_backupXMLDocument.firstChildElement();
        if (settings.tagName()=="settings")
        {
            QDomNodeList list=settings.childNodes();
            QStringList tagList;
            for (int i=0; i<settings.childNodes().count(); i++)
            {
                tagList.append(list.at(i).toElement().tagName());
            }
            if (tagList==m_backupXmlNodesList)
            {
                ParseBackupFile(m_backupXMLDocument);
                return true;
            }
        }
        return false;
    }
}

bool WizardService::HasBackup()
{
    return m_backupDataUserWizardRepositry->hasData() || m_backupDataRolesAndStartupsWizardRepository->hasData();
}

bool WizardService::AdminAlreadyHasAnAccount()
{
    return m_oldDataCurrentUserWizardRepositry->hasData();;
}

void WizardService::ParseBackupFile(QDomDocument &backupXMLDocument)
{
    QDomElement settings= backupXMLDocument.firstChildElement();
    QDomNodeList nodeList=settings.childNodes();
    QDomElement users=nodeList.at(0).toElement();
    m_backupDataUserWizardRepositry->getFCSAndRolesFromXml(users);
    for (int i=1; i<5; i++)
    {
        QDomElement role=nodeList.at(i).toElement();
        m_backupDataRolesAndStartupsWizardRepository->SetRoleDesktopsAndStartupsFromBackup(i-1, role);
    }
}

void WizardService::getDataFromUserRepository(bool isOldData, QString &FCS, QString &rank, int &userCount)
{
    CurrentUserWizardRepository *currentRepository;
    if (isOldData)
    {
        currentRepository=m_oldDataCurrentUserWizardRepositry;
    }
    else
    {
        currentRepository=m_backupDataUserWizardRepositry;
    }
    if (currentRepository->hasData())
    {
        FCS=currentRepository->GetCurrentUserFCS();
        rank=currentRepository->GetCurrentUserRank();
        userCount=currentRepository->GetUsersCount();
    }
}
