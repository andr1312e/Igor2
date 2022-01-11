#include "userwizardpage.h"

UserWizardPage::UserWizardPage(WizardService *service, QWidget *parent)
    : MyWizardPage(parent)
    , m_wizardService(service)
    , m_oldDataUsageValue(false)
{
    CreateUI();
    InsertWidgetsIntoLayout();
}

UserWizardPage::~UserWizardPage()
{
    delete m_topLabel;
    delete m_oldFCSWidget;
    delete m_backupFCSWidget;
    delete m_actionComboBox;
}

int UserWizardPage::nextId() const
{
    return Page_FirstRole;
}

void UserWizardPage::initializePage()
{
    QString adminFCS, adminRank;
    QList<User> users;
    m_actionComboBox->clear();

    if (m_wizardService->HasUserData(false)) {
        m_wizardService->GetDataToViewFromUserRepository(false, adminFCS, adminRank, users);
        m_backupFCSWidget->setVisible(true);
        m_backupFCSWidget->setWizardWidgetFileds(m_backupTitle, adminFCS, adminRank, users);

        if (m_wizardService->HasUserData(true)) {
            m_wizardService->GetDataToViewFromUserRepository(true, adminFCS, adminRank, users);
            m_oldFCSWidget->setVisible(true);
            m_oldFCSWidget->setWizardWidgetFileds(m_oldTitle, adminFCS, adminRank, users);
            m_actionComboBox->addItems(userWizardPageComboBoxBackupAndOldDataActions);

        } else {
            m_oldFCSWidget->setVisible(false);
            m_actionComboBox->addItems(userWizardPageComboBoxBackupActions);
        }

        m_actionComboBox->setCurrentIndex(1);
    } else {

        m_backupFCSWidget->setVisible(false);
        m_oldFCSWidget->setVisible(true);

        if (m_wizardService->HasUserData(true)) {
            m_wizardService->GetDataToViewFromUserRepository(true, adminFCS, adminRank, users);
            m_oldFCSWidget->setWizardWidgetFileds(m_oldTitle, adminFCS, adminRank, users);
            m_actionComboBox->addItems(userWizardPageComboBoxOldDataActions);
            m_actionComboBox->setCurrentIndex(1);
        } else {
            m_oldFCSWidget->setWizardWidgetFileds(m_noDataTitle, adminFCS, adminRank, users);
            m_actionComboBox->addItems(userWizardPageComboBoxNoDataActions);
        }
    }
}

bool UserWizardPage::isComplete() const
{
    //    qDebug()<<"fff";
    return true;
    //       return !m_backupFCSLineEdit->text().remove(' ').isEmpty();
}

void UserWizardPage::CreateUI()
{
    m_topLabel = new QLabel("Данные администратора");
    m_oldFCSWidget = new UserWizardWidget(m_tableHeader, this);
    m_backupFCSWidget = new UserWizardWidget(m_tableHeader, this);
    m_actionComboBox = new QComboBox();
}

void UserWizardPage::InsertWidgetsIntoLayout()
{
    MainLayout()->addWidget(m_topLabel);

    MainLayout()->addWidget(m_oldFCSWidget);
    MainLayout()->addWidget(m_backupFCSWidget);
    MainLayout()->addWidget(m_actionComboBox);

}

void UserWizardPage::CreateConnections()
{
    connect(m_actionComboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), m_wizardService, &WizardService::SetActionWithUserRepository);
}
