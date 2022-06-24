#include "roleappswizardpage.h"

RoleAppsWizardPage::RoleAppsWizardPage(const QString &role, const int currentRoleIndex, WizardService *const service, QWidget *parent)
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
    delete m_topLabel;
    delete m_backupWidget;
    delete m_oldWidget;
    delete m_actionComboBox;
}

void RoleAppsWizardPage::CreateUI()
{
    m_topLabel = new QLabel();
    m_oldWidget = new RoleAppsWizardSubWidget(QStringLiteral(" уже имеющихся"), Q_NULLPTR);
    m_backupWidget = new RoleAppsWizardSubWidget(QStringLiteral(" файла восстановления"), Q_NULLPTR);
    m_actionComboBox = new QComboBox();
}

void RoleAppsWizardPage::InsertWidgetsIntoLayout()
{
    MainLayout()->addWidget(m_topLabel, 0, Qt::AlignHCenter);
    MainLayout()->addWidget(m_oldWidget);
    MainLayout()->addWidget(m_backupWidget);
    MainLayout()->addWidget(m_actionComboBox);
}

void RoleAppsWizardPage::InitUI(const QString &role)
{
    m_topLabel->setText("Ярлыки и программы на рабочем столе для роли: " + role);
    QFont font = m_topLabel->font();
    font.setPointSize(font.pointSize() + 2);
    m_topLabel->setFont(font);
}

void RoleAppsWizardPage::ConnectObjects()
{
    //сохраняем при изменении
    connect(m_actionComboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), this, &RoleAppsWizardPage::OnCurrentIndexChanged);
}

void RoleAppsWizardPage::OnCurrentIndexChanged(const QString &action)
{
    if (action == wizardRolesAllActions.at(1))
    {
        m_oldWidget->setEnabled(false);
        m_backupWidget->setEnabled(true);
    }
    else
    {
        m_oldWidget->setEnabled(true);
        m_backupWidget->setEnabled(false);
    }
    const int absoluteIndexOfAction = wizardRolesAllActions.indexOf(action);
    m_wizardService->SetActionWithRoleRepository(m_currentRoleIndex, absoluteIndexOfAction);
}

int RoleAppsWizardPage::nextId() const
{
    return m_currentRoleIndex + 3;
}

void RoleAppsWizardPage::initializePage()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Перешли на страницу роли"));
    QStringList currentStartups;
    QList<DesktopEntity> currentDesktops;
    m_actionComboBox->clear();

    if (m_wizardService->HasRoleIdAnyData(DbWizardDataType::BackupData, m_currentRoleIndex))
    {
        m_wizardService->GetDataToViewFromDesktopRepository(DbWizardDataType::BackupData, m_currentRoleIndex, currentDesktops, currentStartups);
        m_backupWidget->setVisible(true);
        m_backupWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
        if (m_wizardService->HasRoleIdAnyData(DbWizardDataType::OldData, m_currentRoleIndex))
        {
            m_wizardService->GetDataToViewFromDesktopRepository(DbWizardDataType::OldData, m_currentRoleIndex, currentDesktops, currentStartups);
            m_oldWidget->setVisible(true);
            m_oldWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
            m_actionComboBox->addItems(wizardRolesAllActions);

        }
        else
        {
            m_oldWidget->setVisible(false);
            m_actionComboBox->addItems(wizardRolesDeleteAndRestoreActions);
        }

        m_actionComboBox->setCurrentIndex(1);
    }
    else
    {
        m_backupWidget->setVisible(false);

        if (m_wizardService->HasRoleIdAnyData(DbWizardDataType::OldData, m_currentRoleIndex))
        {
            m_wizardService->GetDataToViewFromDesktopRepository(DbWizardDataType::OldData, m_currentRoleIndex, currentDesktops, currentStartups);
            m_oldWidget->setVisible(true);
            m_oldWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
            m_actionComboBox->addItems(wizardRolesDeleteAndOldActions);
            m_actionComboBox->setCurrentIndex(1);
        }
        else
        {
            m_oldWidget->SetWizardWidgetFileds(currentDesktops, currentStartups);
            m_actionComboBox->addItems(wizardRolesDeleteActions);
        }
    }
}

QString RoleAppsWizardPage::GetUserChoise() const
{
    return m_actionComboBox->currentText();
}

