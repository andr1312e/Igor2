#ifndef WIZSERV_H
#define WIZSERV_H

#include <QObject>

#include "roleappsandstartupsettingsrepository.h"
#include "usersettingsrepository.h"

#include "Services/Settings/programfilesstate.h"
#include "Structs/programstruct.h"
#include "Structs/userstruct.h"

class WizardService : public QObject
{
    Q_OBJECT
public:
    explicit WizardService(ProgramFilesState state, QString &currentUserName, QString currentUserId, QStringList validSettingsPaths, QStringList defaultValues, Terminal *terminal, QObject *parent);

    ~WizardService();

    bool CheckAndParseBackupFile(QString &backupPath);

    bool HasBackup();

    bool AdminAlreadyHasAnAccount();

    void getDataFromUserRepository(bool isOldData, QString &FCS, QString &rank, int &userCount);

private:

    CurrentUserWizardRepository *m_oldDataCurrentUserWizardRepositry;

    CurrentUserWizardRepository *m_backupDataUserWizardRepositry;

    RolesAndStartupsWizardRepository *m_oldDataRolesAndStartupsWizardRepository;

    RolesAndStartupsWizardRepository *m_backupDataRolesAndStartupsWizardRepository;

private:

    QStringList m_backupXmlNodesList;

Q_SIGNALS:

    void backupIsCorrect(bool value);

    void setFCSForm(bool isOldData, QString &adminFCS, QString &adminRank);

    void finished();

private:

    void SetOldRepositoriesData(ProgramFilesState state, QString &currentUserName, QStringList &validSettingsPaths,  Terminal *terminal);

    void TryToSetCurrentUserOldsFcsAndRank(QStringList &validSettingsPaths);

    void TryToSetOldExecsAndDesktopFiles(QStringList &validSettingsPaths);

    void ParseBackupFile(QDomDocument &backupXMLDocument);



};

#endif // WIZSERV_H
