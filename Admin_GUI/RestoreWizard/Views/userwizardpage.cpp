#include "userwizardpage.h"

UserWizardPage::UserWizardPage(WizardService *service, QWidget *parent)
    : MyWizardPage(parent)
    , m_backupTitle(QStringLiteral("Данные хранящиеся в файле восстановления (изменяемые):"))
    , m_oldTitle(QStringLiteral("Данные  уже хранящиеся в локальной базе:"))
    , m_noDataTitle(QStringLiteral("Введите имя и фамилию администратора:"))
    , m_tableHeader{QStringLiteral("Ид"), QStringLiteral("Имя в системе"), QStringLiteral("ФИО"), QStringLiteral("Роль")}
    , m_wizardService(service)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

UserWizardPage::~UserWizardPage()
{
    delete m_topLabel;
    delete m_oldWidget;
    delete m_backupWidget;
    delete m_actionComboBox;
}

int UserWizardPage::nextId() const
{
    return static_cast<int>(WizardPage::Page_FirstRole);
}

void UserWizardPage::initializePage()
{
    QString adminFCS, adminRank;
    QList<User> users;
    m_actionComboBox->clear();

    if (m_wizardService->HasUserData(DbWizardDataType::BackupData))
    {
        m_wizardService->GetDataToViewFromUserRepository(DbWizardDataType::BackupData, adminFCS, users);
        m_backupWidget->setVisible(true);
        m_backupWidget->FillWidget(m_backupTitle, adminFCS, users);

        if (m_wizardService->HasUserData(DbWizardDataType::OldData))
        {
            m_wizardService->GetDataToViewFromUserRepository(DbWizardDataType::OldData, adminFCS, users);
            m_oldWidget->setVisible(true);
            m_oldWidget->FillWidget(m_oldTitle, adminFCS, users);
            m_actionComboBox->addItems(wizardUsersAllAction);

        }
        else
        {
            m_oldWidget->setVisible(false);
            m_actionComboBox->addItems(wizardUsersDeleteAndRestoreActions);
        }

        m_actionComboBox->setCurrentIndex(1);
    }
    else
    {

        m_backupWidget->setVisible(false);
        m_oldWidget->setVisible(true);

        if (m_wizardService->HasUserData(DbWizardDataType::OldData))
        {
            m_wizardService->GetDataToViewFromUserRepository(DbWizardDataType::OldData, adminFCS, users);
            m_oldWidget->FillWidget(m_oldTitle, adminFCS, users);
            m_actionComboBox->addItems(wizardUsersDeleteAndOldActions);
            m_actionComboBox->setCurrentIndex(1);
        }
        else
        {
            m_oldWidget->FillWidget(m_noDataTitle, adminFCS, users);
            m_actionComboBox->addItems(wizardUsersDeleteActions);
        }
    }
}


void UserWizardPage::CreateUI()
{
    m_topLabel = new QLabel(QStringLiteral("Учетные записи"));
    m_oldWidget = new UserWizardWidget(m_tableHeader, Q_NULLPTR, this);
    m_backupWidget = new UserWizardWidget(m_tableHeader, m_wizardService, this);
    m_actionComboBox = new QComboBox();
}

void UserWizardPage::InsertWidgetsIntoLayout()
{
    MainLayout()->addWidget(m_topLabel, 0, Qt::AlignHCenter);

    MainLayout()->addWidget(m_oldWidget);
    MainLayout()->addWidget(m_backupWidget);
    MainLayout()->addWidget(m_actionComboBox);

}

void UserWizardPage::FillUI()
{
    QFont font = m_topLabel->font();
    font.setPointSize(font.pointSize() + 2);
    m_topLabel->setFont(font);
}

void UserWizardPage::ConnectObjects()
{
    connect(m_actionComboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this, &UserWizardPage::OnActionComboBoxChanged);
    connect(m_oldWidget, &UserWizardWidget::ToAdminFCSUpdated, this, &UserWizardPage::OnAdminFCSEdited);
    connect(m_backupWidget, &UserWizardWidget::ToAdminFCSUpdated, this, &UserWizardPage::OnAdminFCSEdited);
}

void UserWizardPage::OnActionComboBoxChanged(const QString &action)
{
    if (action == wizardUsersAllAction.back())
    {
        m_oldWidget->setEnabled(true);
        m_backupWidget->setEnabled(false);
    }
    else
    {
        if (action == wizardUsersAllAction.at(1))
        {
            m_oldWidget->setEnabled(false);
            m_backupWidget->setEnabled(true);
        }
        else
        {
            if (m_wizardService->HasUserData(DbWizardDataType::BackupData))
            {
                m_oldWidget->setEnabled(false);
                m_backupWidget->setEnabled(true);
            }
            else
            {
                m_oldWidget->setEnabled(true);
                m_backupWidget->setEnabled(false);
            }
        }
    }
    const int actionIndex = wizardUsersAllAction.indexOf(action);
    //нам надо узнать абсолютный индекс действия
    //0 убрать все данные, 1 восстановить из бэкапа, 2 - оставить старые данные
    m_wizardService->SetActionWithUserRepository(actionIndex);
}

void UserWizardPage::OnAdminFCSEdited(const QString &adminFCS)
{
    if (sender() == m_oldWidget)
    {
        m_wizardService->UpdateAdminFCS(adminFCS, DbWizardDataType::OldData);
    }
    else
    {
        m_wizardService->UpdateAdminFCS(adminFCS, DbWizardDataType::BackupData);
    }
}
