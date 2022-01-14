#include "roleappswizardpage.h"

RoleAppsWizardPage::RoleAppsWizardPage(const QString &role, const int currentRoleIndex, WizardService * const service, QWidget *parent)
    : MyWizardPage(parent)
    , m_currentRoleIndex(currentRoleIndex)
    , m_wizardService(service)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    InitUI(role);
    ConnectObjects();
}

RoleAppsWizardPage::~RoleAppsWizardPage()
{

    delete m_titleLabel;
    delete m_backupWidget;
    delete m_oldWidget;
    delete m_actionComboBox;

}

void RoleAppsWizardPage::CreateUI()
{
    m_titleLabel = new QLabel();
    m_oldWidget = new RoleAppsWizardSubWidget(" уже имеющихся", nullptr);
    m_backupWidget = new RoleAppsWizardSubWidget(" файла восстановления", nullptr);
    m_actionComboBox = new QComboBox();
}

void RoleAppsWizardPage::InsertWidgetsIntoLayout()
{
    MainLayout()->addWidget(m_titleLabel);
    MainLayout()->addWidget(m_oldWidget);
    MainLayout()->addWidget(m_backupWidget);
    MainLayout()->addWidget(m_actionComboBox);
}

void RoleAppsWizardPage::InitUI(const QString &role)
{
    m_titleLabel->setText("Ярлыки и программы на рабочем столе для роли: " + role);
}

void RoleAppsWizardPage::ConnectObjects()
{
    //сохраняем при изменении
    connect(m_actionComboBox, &QComboBox::currentTextChanged, [&](const QString & actionWithRoleRepository) {
        m_wizardService->SetActionWithRoleRepository(m_currentRoleIndex, actionWithRoleRepository);
    });
}

int RoleAppsWizardPage::nextId() const
{
    return m_currentRoleIndex + 3;
}

void RoleAppsWizardPage::initializePage()
{
    QStringList currentStartups;
    QList<DesktopEntity> currentDesktops;
    m_actionComboBox->clear();

    if (m_wizardService->HasRoleIdAnyData(false, m_currentRoleIndex))
    {
        m_wizardService->GetDataToViewFromDesktopRepository(m_currentRoleIndex, false, currentDesktops, currentStartups);
        m_backupWidget->setVisible(true);
        m_backupWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
        if (m_wizardService->HasRoleIdAnyData(true, m_currentRoleIndex))
        {
            m_wizardService->GetDataToViewFromDesktopRepository(m_currentRoleIndex, true, currentDesktops, currentStartups);
            m_oldWidget->setVisible(true);
            m_oldWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
            m_actionComboBox->addItems(m_rolesWizardPageComboBoxBackupAndOldDataActions);

        } else {
            m_oldWidget->setVisible(false);
            m_actionComboBox->addItems(m_rolesWizardPageComboBoxBackupActions);
        }

        m_actionComboBox->setCurrentIndex(1);
    } else {
        m_backupWidget->setVisible(false);

        if (m_wizardService->HasRoleIdAnyData(true, m_currentRoleIndex)) {
            m_wizardService->GetDataToViewFromDesktopRepository(m_currentRoleIndex, true, currentDesktops, currentStartups);
            m_oldWidget->setVisible(true);
            m_oldWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
            m_actionComboBox->addItems(m_rolesWizardPageComboBoxOldDataActions);
            m_actionComboBox->setCurrentIndex(1);
        } else {
            m_oldWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
            m_actionComboBox->addItems(m_rolesWizardPageComboBoxNoDataActions);
        }
    }
}

QString RoleAppsWizardPage::GetUserChoise() const
{
    return m_actionComboBox->currentText();
}

