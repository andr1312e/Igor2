#ifndef FCSPAGE_H
#define FCSPAGE_H
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>
#include <QLineEdit>
#include <QComboBox>
#include <QDomDocument>

#include <Services/Settings/usersettingsrepository.h>

class FCSPage: public QWizardPage
{
   Q_OBJECT
public:
    FCSPage(CurrentUserWizardRepository *userWizardRepository, QDomDocument *backup, QWidget *parent);
    void setUserFCSAndRole(QString &FCS, QString &role);
    ~FCSPage();
    int nextId() const override;
    void initializePage() override;
private:
    CurrentUserWizardRepository *m_userWizardRepository;
    QDomDocument *m_backup;

    QString m_FCSValue;
    QString m_RankValue;

    QVBoxLayout *m_mainLayout;
    QLabel *m_topLabel;
    QLabel *m_FCSLabel;
    QLineEdit *m_FCSlineEdit;
    QLabel *m_RankLabel;
    QComboBox *m_RankComboBox;
private:
    void initUI();
    void insertWidgetsIntoLayout();
private:
    void createNewUser();
Q_SIGNALS:
    void sendUpdatedDataToParentWizard(const QString &FCS,const QString &rank) const;
};

#endif // FCSPAGE_H