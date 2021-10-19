#ifndef FCSPAGE_H
#define FCSPAGE_H
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include "Admin_GUI/Views/Wizards/wizardpages.h"
#include "Admin_GUI/Validator/stringvalidator.h"

#include "Services/Settings/programfilesstate.h"
#include <Services/Settings/usersettingsrepository.h>

#include <Services/Settings/wizardservice.h>

#include <QListWidget>

class FCSPage: public QWizardPage
{
   Q_OBJECT
public:
    FCSPage(WizardService *service, QWidget *parent);
    void setUserFCSAndRole(bool isOld, QString &FCS, QString& rank, int &numOfUsers);
    ~FCSPage();
    int nextId() const override;
    void initializePage() override;
    bool isComplete() const override;
private:
    WizardService *m_wizardService;
    StringValidator *m_stringValidator;

    QString m_backupFCSValue;
    QString m_backupRankValue;

    QVBoxLayout *m_mainLayout;
    QLabel *m_topLabel;

    QHBoxLayout *m_userFCSLayout;
    QLabel *m_backupFCSLabel;
    QLineEdit *m_backupFCSLineEdit;

    QHBoxLayout *m_userRankLayout;
    QLabel *m_backupRankLabel;
    QComboBox *m_backupRankComboBox;

    QHBoxLayout *m_numOfUsersLayout;
    QLabel *m_backupNumOfUsersLabel;
    QLabel *m_backupNumOfUsersValue;

    QListWidget *m_accountsData;

    QLabel *m_oldUserDataLabel;

    QHBoxLayout *m_oldUserFCSLayout;
    QLabel *m_oldUserFCSLabel;
    QLineEdit *m_oldUserFCSLineEdit;

    QHBoxLayout *m_oldUserRankLayout;
    QLabel *m_oldUserRankLabel;
    QComboBox *m_oldUserRankComboBox;

    QHBoxLayout *m_oldNumOfUsersLayout;
    QLabel *m_oldNumOfUsersLabel;
    QLabel *m_oldNumOfUsersValue;

    QListWidget *m_oldAccountsData;

    QCheckBox *m_oldDataUsageCheckBox;
    bool m_oldDataUsageValue;

private:
    void initUI();
    void insertWidgetsIntoLayout();
    void createConnections();
private:
     void setOldFieldsVisible(bool state);
Q_SIGNALS:
    void sendUpdatedDataToParentWizard(const QString &FCS,const QString &rank) const;
};

#endif // FCSPAGE_H
