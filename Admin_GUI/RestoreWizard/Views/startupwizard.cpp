#include "startupwizard.h"

StartupWizard::StartupWizard(const QString &applicationPath, const QString &applicationName, const QString &rlsTiFolder, LoadingStates states, LinuxUserService *linuxUserService, ISqlDatabaseService *iSqlDataBaseService, ThemesNames themeName, QWidget *parent)
    : QWizard(parent)
{
    CreateServices(applicationPath, applicationName, rlsTiFolder, states, linuxUserService, iSqlDataBaseService);
    CreateUI(states, themeName);
    InitSizes();
    InitStyles();
    InitBehaviour();
    ConnectObjects();
}

StartupWizard::~StartupWizard()
{
    delete m_wizardService;

    delete m_wizardNavigationBar;
    delete m_introPage;
    delete m_userWizardPage;

    for (int i = 0; i < Roles.count(); ++i)
    {
        delete m_rolesPages.at(i);
    }

    delete m_conclusionPage;
}


void StartupWizard::CreateServices(const QString &applicationPath, const QString &applicationName, const QString &rlsTiFolder, LoadingStates states, LinuxUserService *const linuxUserService, ISqlDatabaseService *const iSqlDataBaseService)
{
    m_wizardService = new WizardService(rlsTiFolder, states, linuxUserService, iSqlDataBaseService, this);
    m_iconMakingService = new IconMaker(applicationPath, applicationName, rlsTiFolder, linuxUserService, this);
}

void StartupWizard::CreateUI(LoadingStates states, ThemesNames themeName)
{
    m_wizardNavigationBar = new WizardNavigtionBar(this);

    m_introPage = new IntroPage(states, m_wizardService, themeName, this);

    m_userWizardPage = new UserWizardPage(m_wizardService, this);

    for (int i = 0; i < Roles.count(); i++)
    {
        m_rolesPages.append(new RoleAppsWizardPage(Roles.at(i), i, m_wizardService, this));
    }
    m_conclusionPage = new ConclusionWizardPage(m_wizardService, m_iconMakingService, this);
}

void StartupWizard::InitSizes()
{
    setMinimumSize(900, 700);
}

void StartupWizard::InitStyles()
{
    setWizardStyle(QWizard::ModernStyle);
    this->setAutoFillBackground(true);
}

void StartupWizard::InitBehaviour()
{

    setPage(static_cast<int>(WizardPage::Page_Intro), m_introPage);
    setPage(static_cast<int>(WizardPage::Page_UserData), m_userWizardPage);

    for (int i = 0; i < m_rolesPages.count(); i++)
    {
        addPage(m_rolesPages.at(i));
    }

    setPage(static_cast<int>(WizardPage::Page_Conclusion), m_conclusionPage);

    setStartId(static_cast<int>(WizardPage::Page_Intro));
    setOption(QWizard::HaveHelpButton);
}

void StartupWizard::ConnectObjects()
{
    connect(this, &QWizard::currentIdChanged, this, &StartupWizard::OnCurrentPageChanged);
    connect(this, &QWizard::helpRequested, this, &StartupWizard::OnHelpButtonClick);
    connect(m_wizardNavigationBar, &WizardNavigtionBar::ToPageNumMove, this, &StartupWizard::OnPageNumMove);
}

void StartupWizard::OnChangeTheme(ThemesNames themeName)
{
    m_introPage->SetWizardStyles(themeName);
}

void StartupWizard::OnHelpButtonClick()
{
    QString message;
    const bool hasBackUp = m_wizardService->HasUserData(DbWizardDataType::BackupData) || m_wizardService->HasAnyRolesData(DbWizardDataType::BackupData);
    const bool hasOldData = m_wizardService->HasUserData(DbWizardDataType::OldData) || m_wizardService->HasAnyRolesData(DbWizardDataType::OldData);

    switch (WizardPage(currentId()))
    {
    case WizardPage::Page_Intro:
        message = QStringLiteral("При запуске приложения, если необходимые данные не были обнаружены, инициируется процесс автоматического восстановления.\n"
                                 "Несмотря на то, что приведенные ниже шаги могут показаться сложными на первом этапе, просто выполните их по порядку, и мы попробуем помочь вам.\n"
                                 "Если у вас есть готовый файл восстановления, выберите его, нажав на кнопку папки справа. Если файл корректный он отобразится в поле файла, иначе изменений не произойдет. \n"
                                 "Если файла нет прочитайте краткое руководство и нажмите Далее...");
        break;

    case WizardPage::Page_UserData:
        if (hasBackUp)
        {
            if (hasOldData)
            {
                message = QStringLiteral("Это страница мастера настроки пользователей программы, их данных и роли.\n"
                                         "Ваша программа обнаружила уже имеющиеся данные, так же вы предоставили файл восстановления, "
                                         "все эти файлы были загружены.\n"
                                         "Необходимо выбрать только один источник, выбрать необходимый вы можете в"
                                         " нижнем поле (комбинированный список) программы.\n"
                                         "Вы можете изменить ФИО Администратора, в соответсвующем поле\n"
                                         "Поля пользователей из файла восстановления разрешается отредактировать.\n"
                                         "Затем, нажмите кнопку Далее...");
            }
            else
            {
                message = QStringLiteral("Это страница мастера настроки пользователей программы, их данных и роли.\n"
                                         "Вы предоставили программе предоставили файл восстановления, данный файл был успешно заружен.\n"
                                         "Вы можете не использовать базу из файла восстановления, выбрав нужный вариант в нижнем поле, и создать новую, записав в нее только данные администратора.\n"
                                         "При необходимости измените ФИО Администратора.\n"
                                         "Поля пользователей из файла восстановления разрешается отредактировать.\n"
                                         "Затем, нажмите кнопку Далее...");
            }
        }
        else
        {
            if (hasOldData)
            {
                message = QStringLiteral("Это страница мастера настроки пользователей программы, их данных и роли.\n"
                                         "Ваша программа обнаружила уже имеющиеся данные, они были загружены, файл восстановления не выбран, "
                                         "при необходимости вы можете изменить ФИО Администратора.\n"
                                         "Так же вы можете не использовать имеющуюся базу, выбрав нужный вариант в нижнем поле, и создать новую, записав в нее только данные администратора.\n"
                                         "Поля пользователей разрешается отредактировать в интерфейсе программы позже.\n"
                                         "Затем, нажмите кнопку Далее...");
            }
            else
            {
                message = QStringLiteral("Поле ввода данных администратора.\n "
                                         "К сожалению, у вас нет ни данных, ни файла восстановления, поэтому "
                                         "введите пожалуйста ФИО Администратора.\n"
                                         "Затем, нажмите кнопку Далее...");
            }
        }

        break;

    case WizardPage::Page_FirstRole:
    case WizardPage::Page_SecondRole:
    case WizardPage::Page_ThirdRole:
    case WizardPage::Page_FourthRole:
        if (hasBackUp)
        {
            if (hasOldData)
            {
                message = QStringLiteral("Данные для программ-ярлыков на рабочем столе, а так же программ которые будут перезапущены для роли: ") + Roles.at(currentId() - 2) +
                          QStringLiteral("\nВаша программа обнаружила уже имеющиеся данные, так же вы предоставили файл восстановления, "
                                         "все эти файлы были загружены.\n"
                                         "Необходимо выбрать только один источник, выбрать необходимый вы можете в самом "
                                         "нижнем поле программы.\n"
                                         "Доступные программы разрешается отредактировать в интерфейсе программы позже.\n"
                                         "Если программы отстувуют в списке, а в файле восстановления они имеются, проверьте их наличие в соответвующей папке рядом с файлом восстановления\n"
                                         "Затем, нажмите кнопку Далее...");
            }
            else
            {
                message = QStringLiteral("Данные для программ-ярлыков на рабочем столе, а так же программ которые будут перезапущены для роли: ") + Roles.at(currentId() - 2) +
                          QStringLiteral("\nВы предоставили программе предоставили файл восстановления, данный файл был успешно заружен.\n"
                                         "Вы можете не использовать базу из файла восстановления, выбрав нужный вариант в самом нижнем поле, и создать новую, записав в нее только данные администратора.\n"
                                         "Доступные программы разрешается отредактировать в интерфейсе программы позже.\n"
                                         "Если программы отстувуют в списке, а в файле восстановления они имеются, проверьте их наличие в соответвующей папке рядом с файлом восстановления\n"
                                         "Затем, нажмите кнопку Далее...");
            }
        }
        else
        {
            if (hasOldData)
            {
                message = QStringLiteral("Данные для программ-ярлыков на рабочем столе, а так же программ которые будут перезапущены для роли: ") + Roles.at(currentId() - 2) +
                          QStringLiteral("\nВаша программа обнаружила уже имеющиеся данные, они были загружены, файл восстановления не выбран, "
                                         "Так же вы можете не использовать имеющуюся базу, выбрав нужный вариант в самом нижнем поле, и создать новую, записав в нее только данные администратора.\n"
                                         "Доступные программы разрешается отредактировать в интерфейсе программы позже.\n"
                                         "Затем, нажмите кнопку Далее...");
            }
            else
            {
                message = QStringLiteral("Данные для программ-ярлыков на рабочем столе, а так же программ которые будут перезапущены для роли: ") + Roles.at(currentId() + 2) +
                          QStringLiteral("\nК сожалению, у вас нет ни данных, ни файла восстановления, поэтому "
                                         "Доступные программы разрешается отредактировать в интерфейсе программы позже.\n"
                                         "Нажмите кнопку Далее...");
            }
        }

        break;

    case WizardPage::Page_Conclusion:
        message = QStringLiteral("Заключительный этап настройки.\n"
                                 "Проверьте правильность настроек, и импорт данных из источников.\n"
                                 "Если все правильно, нажмите кнопку Завершить...\n"
                                 "Иначе нажмите кнопку Назад, и измените необходимые параметры...\n");

        break;
    }

    QMessageBox::information(this, QStringLiteral("Окно справки"), message);
}

void StartupWizard::OnCurrentPageChanged(int id)
{
    m_wizardNavigationBar->SetButtonDown(id);
    if (MyWizardPage *const nextPage = qobject_cast<MyWizardPage *>(currentPage()))
    {
        nextPage->SetNavigationBar(m_wizardNavigationBar);
    }
}

void StartupWizard::OnPageNumMove(int pageIdToMove)
{
    if (QWizard::currentId() == pageIdToMove)
    {
        return;
    }
    else
    {
        if (QWizard::currentId() < pageIdToMove)
        {
            while (pageIdToMove != QWizard::currentId())
            {
                QWizard::next();
            }
        }
        else
        {
            while (pageIdToMove != QWizard::currentId())
            {
                QWizard::back();
            }
        }
    }
}

void StartupWizard::accept()
{
    m_iconMakingService->ApplyWizardActions();
    m_wizardService->ApplyWizardActions();
    QWizard::accept();
}

void StartupWizard::reject()
{
    QWizard::reject();
}
