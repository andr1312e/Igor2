#include "conclusionwizardpage.h"

ConclusionWizardPage::ConclusionWizardPage(WizardService *service, IconMaker *iconMaker, QWidget *parent)
    : MyWizardPage(parent)
    , m_wizardService(service)
    , m_iconMakerSerivce(iconMaker)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

ConclusionWizardPage::~ConclusionWizardPage()
{
    delete m_oldDataLayout;
    delete m_backupLayout;
    delete m_dataLayout;
    delete m_installingPackesLayout;

    delete m_topLabel;
    delete m_descriptionLabel;

    delete m_backupLabel;
    delete m_backupValue;

    delete m_oldDataLabel;
    delete m_oldDataValue;
    delete m_actionsLabel;

    delete m_usersLabel;
    delete m_usersActionValue;
    delete m_usersCommentLabel;
    delete m_usersCommentValue;

    delete m_firstRoleLabel;
    delete m_firstRoleActionValue;
    delete m_firstRoleCommentLabel;
    delete m_firstRoleCommentValue;

    delete m_secondRoleLabel;
    delete m_secondRoleActionValue;
    delete m_secondRoleCommentLabel;
    delete m_secondRoleCommentValue;

    delete m_thirdRoleLabel;
    delete m_thirdRoleActionValue;
    delete m_thirdRoleCommentLabel;
    delete m_thirdRoleCommentValue;

    delete m_fourthRoleLabel;
    delete m_fourthRoleActionValue;
    delete m_fourthRoleCommentLabel;
    delete m_fourthRoleCommentValue;

    delete m_packagesToInstallLabel;
    delete m_packagesToInstallList;

    delete m_additionalActionsLabel;
    delete m_needInstallPackages;
    delete m_addIconToDesktopToCurrentUser;
    delete m_addIconToDesktopToAllUsers;
    delete m_addIconToStartMenu;

    delete m_currentDoingActionLabel;
    delete m_currentDoingActionName;
    delete m_totalProgressBar;
}

int ConclusionWizardPage::nextId() const
{
    return -1;
}

void ConclusionWizardPage::initializePage()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Перешли на страницу завершения"));
    if (m_wizardService->HasUserData(DbWizardDataType::OldData))
    {
        m_oldDataValue->setText(QStringLiteral("имеются"));
    }
    else
    {
        m_oldDataValue->setText(QStringLiteral("отсутствуют"));
    }
    if (m_wizardService->HasUserData(DbWizardDataType::BackupData))
    {
        m_backupValue->setText(m_wizardService->GetBackupFilePath());
    }
    else
    {
        m_backupValue->setText(QStringLiteral("папка восстановления отсутствует"));
    }
    const int actionIndexWithUserRepository = m_wizardService->GetActionIndexWithUserRepository();
    SetUserActionValueLabel(actionIndexWithUserRepository);

    for (int i = 0; i < Roles.count(); ++i)
    {
        const int actionIndexWithRoleRepository = m_wizardService->GetActionWithRoleRepository(i);
        SetRolesActionValueLabel(i, actionIndexWithRoleRepository);
    }
    SetFutureInstalledPackagesToList();

}

void ConclusionWizardPage::CreateUI()
{
    m_topLabel = new QLabel(QStringLiteral("Итоговые настройки"));
    m_descriptionLabel = new QLabel(QStringLiteral("Настройки пока НЕ применены, проверьте корректность ввода данных.\n"
                                    "Если все настройки выставлены правильно, нажмите кнопку Завершить."
                                    "\nИначе, нажмите Назад и выполните перенастройку..."));
    m_backupLayout = new QHBoxLayout();
    m_backupLabel = new QLabel(QStringLiteral("Папка восстановления: "));
    m_backupValue = new QLabel();

    m_oldDataLayout = new QHBoxLayout();
    m_oldDataLabel = new QLabel(QStringLiteral("Данные в системе: "));
    m_oldDataValue = new QLabel();

    m_actionsLabel = new QLabel(QStringLiteral("Выбранные действия над компонентами программы:"));
    m_dataLayout = new QGridLayout();

    m_usersLabel = new QLabel(QStringLiteral("База данных пользователей"));
    m_usersActionValue = new QLabel();
    m_usersCommentLabel = new QLabel(QStringLiteral("Колличество пользователей:"));
    m_usersCommentValue = new QLabel();

    m_firstRoleLabel = new QLabel(QStringLiteral("База роли ") + Roles.front());
    m_firstRoleActionValue = new QLabel();
    m_firstRoleCommentLabel = new QLabel(QStringLiteral("Колличество программ:"));
    m_firstRoleCommentValue = new QLabel();

    m_secondRoleLabel = new QLabel(QStringLiteral("База роли ") + Roles.at(1));
    m_secondRoleActionValue = new QLabel();
    m_secondRoleCommentLabel = new QLabel(QStringLiteral("Колличество программ:"));
    m_secondRoleCommentValue = new QLabel();

    m_thirdRoleLabel = new QLabel(QStringLiteral("База роли ") + Roles.at(2));
    m_thirdRoleActionValue = new QLabel();
    m_thirdRoleCommentLabel = new QLabel(QStringLiteral("Колличество программ:"));
    m_thirdRoleCommentValue = new QLabel();

    m_fourthRoleLabel = new QLabel(QStringLiteral("База роли ") + Roles.at(3));
    m_fourthRoleActionValue = new QLabel();
    m_fourthRoleCommentLabel = new QLabel(QStringLiteral("Колличество программ:"));
    m_fourthRoleCommentValue = new QLabel();

    m_packagesToInstallLabel = new QLabel(QStringLiteral("Пакеты для установки:"));
    m_packagesToInstallList = new QListWidget();

    m_additionalActionsLabel = new QLabel(QStringLiteral("Дополнительные действия:"));
    m_needInstallPackages = new QCheckBox(QStringLiteral("Установить пакеты (без этого программы могут не запуститься)"));
    m_addIconToDesktopToCurrentUser = new QCheckBox(QStringLiteral("Добавить икону на рабочий стол для текущего пользователя"));
    m_addIconToDesktopToAllUsers = new QCheckBox(QStringLiteral("Добавить иконку на рабочий стол для всех пользователей"));
    m_addIconToStartMenu = new QCheckBox(QStringLiteral("Добавить иконку в меню пуск для всех пользователей"));

    m_installingPackesLayout = new QHBoxLayout();
    m_currentDoingActionLabel = new QLabel();
    m_currentDoingActionName = new QLabel();
    m_totalProgressBar = new QProgressBar();
}

void ConclusionWizardPage::InsertWidgetsIntoLayout()
{

    MainLayout()->addWidget(m_topLabel, 0, Qt::AlignHCenter);
    MainLayout()->addWidget(m_descriptionLabel);

    MainLayout()->addLayout(m_backupLayout);

    m_oldDataLayout->addWidget(m_oldDataLabel);
    m_oldDataLayout->addWidget(m_oldDataValue, 1, Qt::AlignRight);

    MainLayout()->addLayout(m_oldDataLayout);
    MainLayout()->addWidget(m_actionsLabel);

    m_dataLayout->addWidget(m_usersLabel, 0, 0, 1, 1, Qt::AlignLeft);
    m_dataLayout->addWidget(m_usersActionValue, 0, 1, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_usersCommentLabel, 0, 2, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_usersCommentValue, 0, 3, 1, 1, Qt::AlignRight);

    m_dataLayout->addWidget(m_firstRoleLabel, 1, 0, 1, 1, Qt::AlignLeft);
    m_dataLayout->addWidget(m_firstRoleActionValue, 1, 1, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_firstRoleCommentLabel, 1, 2, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_firstRoleCommentValue, 1, 3, 1, 1, Qt::AlignRight);

    m_dataLayout->addWidget(m_secondRoleLabel, 2, 0, 1, 1, Qt::AlignLeft);
    m_dataLayout->addWidget(m_secondRoleActionValue, 2, 1, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_secondRoleCommentLabel, 2, 2, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_secondRoleCommentValue, 2, 3, 1, 1, Qt::AlignRight);

    m_dataLayout->addWidget(m_thirdRoleLabel, 3, 0, 1, 1, Qt::AlignLeft);
    m_dataLayout->addWidget(m_thirdRoleActionValue, 3, 1, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_thirdRoleCommentLabel, 3, 2, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_thirdRoleCommentValue, 3, 3, 1, 1, Qt::AlignRight);

    m_dataLayout->addWidget(m_fourthRoleLabel, 4, 0, 1, 1, Qt::AlignLeft);
    m_dataLayout->addWidget(m_fourthRoleActionValue, 4, 1, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_fourthRoleCommentLabel, 4, 2, 1, 1, Qt::AlignHCenter);
    m_dataLayout->addWidget(m_fourthRoleCommentValue, 4, 3, 1, 1, Qt::AlignRight);
    MainLayout()->addLayout(m_dataLayout, 1);
    MainLayout()->addWidget(m_packagesToInstallLabel);
    MainLayout()->addWidget(m_packagesToInstallList);


    MainLayout()->addWidget(m_additionalActionsLabel);
    MainLayout()->addWidget(m_needInstallPackages, 0, Qt::AlignHCenter);
    MainLayout()->addWidget(m_addIconToDesktopToCurrentUser, 0, Qt::AlignHCenter);
    MainLayout()->addWidget(m_addIconToDesktopToAllUsers, 0, Qt::AlignHCenter);
    MainLayout()->addWidget(m_addIconToStartMenu, 0, Qt::AlignHCenter);

    m_installingPackesLayout->addWidget(m_currentDoingActionLabel, 1);
    m_installingPackesLayout->addWidget(m_currentDoingActionName);
    m_installingPackesLayout->addSpacing(10);

    MainLayout()->addLayout(m_installingPackesLayout);
    MainLayout()->addWidget(m_totalProgressBar);
}

void ConclusionWizardPage::FillUI()
{
    QFont font = m_topLabel->font();
    font.setPointSize(font.pointSize() + 2);
    m_topLabel->setFont(font);
    m_needInstallPackages->setChecked(true);
    m_addIconToDesktopToCurrentUser->setChecked(true);
    m_addIconToDesktopToAllUsers->setChecked(true);
    m_addIconToStartMenu->setChecked(true);
    m_currentDoingActionLabel->setText(QStringLiteral("Выполняемое действие:"));
    m_currentDoingActionName->setText(QStringLiteral("процесс пока не запущен"));
}

void ConclusionWizardPage::ConnectObjects()
{
    connect(m_needInstallPackages, &QCheckBox::clicked, m_wizardService, &WizardService::OnChangeInstallState);
    connect(m_addIconToDesktopToCurrentUser, &QCheckBox::clicked, m_iconMakerSerivce, &IconMaker::OnAddIconToDesktopToCurrentUser);
    connect(m_addIconToDesktopToAllUsers, &QCheckBox::clicked, m_iconMakerSerivce, &IconMaker::OnAddIconToDesktopToAllUsers);
    connect(m_addIconToStartMenu, &QCheckBox::clicked, m_iconMakerSerivce, &IconMaker::OnAddIconToStartMenu);
    connect(m_wizardService, &WizardService::ToCurrentActionDoing, this, &ConclusionWizardPage::OnCurrentActionDoing);
    connect(m_wizardService, &WizardService::ToSetActionsCount, m_totalProgressBar, &QProgressBar::setMaximum);
}


void ConclusionWizardPage::OnCurrentActionDoing(const QString &actionDescription)
{
    m_totalProgressBar->setValue(m_totalProgressBar->value() + 1);
    m_currentDoingActionName->setText(actionDescription);
}

void ConclusionWizardPage::SetUserActionValueLabel(int actionIndexWithUserRepository)
{
    m_usersActionValue->setText(wizardUsersAllAction.at(actionIndexWithUserRepository));

    if (0 == actionIndexWithUserRepository)
    {
        m_usersCommentValue->setText(QStringLiteral("Один пользователь"));
    }
    else
    {
        if (1 == actionIndexWithUserRepository)
        {
            if (m_wizardService->HasUserData(DbWizardDataType::BackupData))
            {
                m_usersCommentValue->setText(QString::number(m_wizardService->GetUserCountFromUserRepository(DbWizardDataType::BackupData)));
            }
            else
            {
                qFatal("Не реализованное поведение: файл восстановления есть, но колличество пользователей с него не может быть получено. ConclusionWizardPage::setUserRepositoryLabel");
            }
        }
        else
        {
            if (m_wizardService->HasUserData(DbWizardDataType::OldData))
            {
                m_usersCommentValue->setText(QString::number(m_wizardService->GetUserCountFromUserRepository(DbWizardDataType::OldData)));
            }
            else
            {
                qFatal("Не реализованное поведение: старые данные есть, но колличество пользователей с него не может быть получено. ConclusionWizardPage::setUserRepositoryLabel");
            }
        }
    }
}

void ConclusionWizardPage::SetRolesActionValueLabel(int roleIndex,  int actionIndexWithRoleReposiory)
{
    switch (roleIndex)
    {
    case 0:
        m_firstRoleActionValue->setText(wizardRolesAllActions.at(actionIndexWithRoleReposiory));

        if (0 == actionIndexWithRoleReposiory)
        {
            m_firstRoleCommentValue->setText(QStringLiteral("нет программ"));
        }
        else
        {
            if (1 == actionIndexWithRoleReposiory)
            {
                m_firstRoleCommentValue->setText(QString::number(m_wizardService->GetAppsCountFromDesktopRepository(DbWizardDataType::BackupData, roleIndex)));
            }
            else
            {
                m_firstRoleCommentValue->setText(QString::number(m_wizardService->GetAppsCountFromDesktopRepository(DbWizardDataType::OldData, roleIndex)));
            }
        }

        break;

    case 1:
        m_secondRoleActionValue->setText(wizardRolesAllActions.at(actionIndexWithRoleReposiory));

        if (0 == actionIndexWithRoleReposiory)
        {
            m_secondRoleCommentValue->setText(QStringLiteral("нет программ"));
        }
        else
        {
            if (1 == actionIndexWithRoleReposiory)
            {
                m_secondRoleCommentValue->setText(QString::number(m_wizardService->GetAppsCountFromDesktopRepository(DbWizardDataType::BackupData, roleIndex)));
            }
            else
            {
                m_secondRoleCommentValue->setText(QString::number(m_wizardService->GetAppsCountFromDesktopRepository(DbWizardDataType::OldData, roleIndex)));
            }
        }

        break;

    case 2:
        m_thirdRoleActionValue->setText(wizardRolesAllActions.at(actionIndexWithRoleReposiory));

        if (0 == actionIndexWithRoleReposiory)
        {
            m_thirdRoleCommentValue->setText(QStringLiteral("нет программ"));
        }
        else
        {
            if (1 == actionIndexWithRoleReposiory)
            {
                m_thirdRoleCommentValue->setText(QString::number(m_wizardService->GetAppsCountFromDesktopRepository(DbWizardDataType::BackupData, roleIndex)));
            }
            else
            {
                m_thirdRoleCommentValue->setText(QString::number(m_wizardService->GetAppsCountFromDesktopRepository(DbWizardDataType::OldData, roleIndex)));
            }
        }

        break;

    case 3:
        m_fourthRoleActionValue->setText(wizardRolesAllActions.at(actionIndexWithRoleReposiory));

        if (0 == actionIndexWithRoleReposiory)
        {
            m_fourthRoleCommentValue->setText(QStringLiteral("нет программ"));
        }
        else
        {
            if (1 == actionIndexWithRoleReposiory)
            {
                m_fourthRoleCommentValue->setText(QString::number(m_wizardService->GetAppsCountFromDesktopRepository(DbWizardDataType::BackupData, roleIndex)));
            }
            else
            {
                m_fourthRoleCommentValue->setText(QString::number(m_wizardService->GetAppsCountFromDesktopRepository(DbWizardDataType::OldData, roleIndex)));
            }
        }

        break;

    default:
        qFatal("Роли только четыре");
        break;
    }
}

void ConclusionWizardPage::SetFutureInstalledPackagesToList()
{
    m_packagesToInstallList->clear();
    const QStringList items(m_wizardService->GetAllDependenciesList());
    if (items.isEmpty())
    {
        m_needInstallPackages->setVisible(false);
        m_packagesToInstallLabel->setVisible(false);
        m_packagesToInstallList->setVisible(false);
        m_currentDoingActionLabel->setVisible(false);
        m_currentDoingActionName->setVisible(false);
    }
    else
    {
        m_packagesToInstallLabel->setText(("Пакетов для установки: ") + QString::number(items.count()));
        m_currentDoingActionLabel->setVisible(true);
        m_currentDoingActionName->setVisible(true);
        m_needInstallPackages->setVisible(true);
        m_packagesToInstallLabel->setVisible(true);
        m_packagesToInstallList->setVisible(true);
        m_packagesToInstallList->addItems(items);
    }
}
