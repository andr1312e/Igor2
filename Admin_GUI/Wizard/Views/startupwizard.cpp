#include "startupwizard.h"

StartupWizard::StartupWizard(const QString &currentUserName, const QString &currentUserId, const QString &applicationName, const QString &rlsTiFolder, const LoadingState &loadedDbAdnRolesState, LinuxUserService *linuxUserService, ISqlDatabaseService *iSqlDataBaseService, QWidget *parent)
    : QWizard(parent)
{
    CreateServices(currentUserName, currentUserId, applicationName, rlsTiFolder, loadedDbAdnRolesState, linuxUserService, iSqlDataBaseService);
    CreateUI(loadedDbAdnRolesState);
    InitSizes();
    InitStyles();
    InitBehaviour();
    ConnectObjects();
}

StartupWizard::~StartupWizard()
{
    delete m_wizardService;

    delete m_introPage;
    delete m_userWizardPage;

    for (int i = 0; i < Roles.count(); ++i) {
        delete m_rolesPages.at(i);
    }

    delete m_conclusionPage;
    delete m_wizardNavigationBar;
}


void StartupWizard::CreateServices(const QString &currentUserName, const QString &currentUserId, const QString &applicationName, const QString &rlsTiFolder, const LoadingState &loadedDbAdnRolesState, LinuxUserService* const linuxUserService, ISqlDatabaseService* const iSqlDataBaseService)
{
    m_wizardService = new WizardService(currentUserName, currentUserId, rlsTiFolder, loadedDbAdnRolesState, linuxUserService, iSqlDataBaseService, this);
    m_iconMakingService=new IconMaker(applicationName, rlsTiFolder, linuxUserService, this);
}

void StartupWizard::CreateUI(const LoadingState &loadedDbAdnRolesState)
{
    m_wizardNavigationBar = new WizardNavigtionBar(this);

    m_introPage = new IntroPage(loadedDbAdnRolesState, m_wizardService, this);

    m_userWizardPage = new UserWizardPage(m_wizardService, this);

    for (int i = 0; i < Roles.count(); i++) {
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

    setPage(Page_Intro, m_introPage);
    setPage(Page_UserData, m_userWizardPage);

    for (int i = 0; i < m_rolesPages.count(); i++) {
        addPage(m_rolesPages.at(i));
    }

    setPage(Page_Conclusion, m_conclusionPage);

    setStartId(Page_Intro);
    setOption(QWizard::HaveHelpButton);
}

void StartupWizard::ConnectObjects()
{
    connect(this, &QWizard::currentIdChanged, this, &StartupWizard::OnCurrentPageChanged);
    connect(this, &QWizard::helpRequested, this, &StartupWizard::OnHelpButtonClick);
    connect(m_wizardNavigationBar, &WizardNavigtionBar::ToPageNumMove, this, &StartupWizard::OnPageNumMove);
}

void StartupWizard::OnHelpButtonClick()
{
    QString message;
    bool hasBackUp = m_wizardService->HasUserData(false) || m_wizardService->HasAnyRolesData(false);
    bool hasOldData = m_wizardService->HasUserData(true) || m_wizardService->HasAnyRolesData(true);

    switch (currentId()) {
    case Page_Intro:
        message = "При запуске приложения, если необходимые данные не были обнаружены, инициируется процесс автоматического восстановления.\n"
                  "Несмотря на то, что приведенные ниже шаги могут показаться сложными на первом этапе, просто выполните их по порядку, и мы попробуем помочь вам.\n"
                  "Если у вас есть готовый файл восстановления, выберите его, нажав на кнопку папки справа. Если файл корректный он отобразится в поле файла, иначе изменений не произойдет. \n"
                  "Если файла нет прочитайте краткое руководство и нажмите Далее...";
        break;

    case Page_UserData:
        if (hasBackUp) {
            if (hasOldData) {
                message = "Поле ввода данных администратора.\n"
                          "Ваша программа обнаружила уже имеющиеся данные, так же вы предоставили файл восстановления, "
                          "все эти файлы были загружены.\n"
                          "Необходимо выбрать только один источник, выбрать необходимый вы можете в самом"
                          "нижнем поле программы.\n"
                          "Вы можете изменить ФИО, или воиское звание, в соответсвующих полях\n"
                          "Поля пользователей разрешается отредактировать в интерфейсе программы позже.\n"
                          "Затем, нажмите кнопку Далее...";
            } else {
                message = "Поле ввода данных администратора.\n "
                          "Вы предоставили программе предоставили файл восстановления, данный файл был успешно заружен.\n"
                          "Вы можете не использовать базу из файла восстановления, выбрав нужный вариант в самом нижнем поле, и создать новую, записав в нее только данные администратора.\n"
                          "При необходимости измените ФИО, или воиское звание.\n"
                          "Поля пользователей разрешается отредактировать в интерфейсе программы позже.\n"
                          "Затем, нажмите кнопку Далее...";
            }
        } else {
            if (hasOldData) {
                message = "Поле ввода данных администратора.\n "
                          "Ваша программа обнаружила уже имеющиеся данные, они были загружены, файл восстановления не выбран, "
                          "при необходимости вы можете изменить ФИО, или воиское звание.\n"
                          "Так же вы можете не использовать имеющуюся базу, выбрав нужный вариант в самом нижнем поле, и создать новую, записав в нее только данные администратора.\n"
                          "Поля пользователей разрешается отредактировать в интерфейсе программы позже.\n"
                          "Затем, нажмите кнопку Далее...";
            } else {
                message = "Поле ввода данных администратора.\n "
                          "К сожалению, у вас нет ни данных, ни файла восстановления, поэтому "
                          "введите пожалуйста ФИО, и выберите воиское звание снизу.\n"
                          "Поля пользователей разрешается отредактировать в интерфейсе программы позже.\n"
                          "Затем, нажмите кнопку Далее...";
            }
        }

        break;

    case Page_FirstRole:
    case Page_SecondRole:
    case Page_ThirdRole:
    case Page_FourthRole:
        if (hasBackUp) {
            if (hasOldData) {
                message = "Данные для программ-ярлыков в режиме киоска, а так же программ которые будут перезапущены для роли: " + Roles.at(currentId() - 2) +
                        "\nВаша программа обнаружила уже имеющиеся данные, так же вы предоставили файл восстановления, "
                        "все эти файлы были загружены.\n"
                        "Необходимо выбрать только один источник, выбрать необходимый вы можете в самом"
                        "нижнем поле программы.\n"
                        "Доступные программы разрешается отредактировать в интерфейсе программы позже.\n"
                        "Если программы отстувуют в списке, а в файле восстановления они имеются, проверьте их наличие в соответвующей папке рядом с файлом восстановления\n"
                        "Затем, нажмите кнопку Далее...";
            } else {
                message = "Данные для программ-ярлыков в режиме киоска, а так же программ которые будут перезапущены для роли: " + Roles.at(currentId() - 2) +
                        "\nВы предоставили программе предоставили файл восстановления, данный файл был успешно заружен.\n"
                        "Вы можете не использовать базу из файла восстановления, выбрав нужный вариант в самом нижнем поле, и создать новую, записав в нее только данные администратора.\n"
                        "Доступные программы разрешается отредактировать в интерфейсе программы позже.\n"
                        "Если программы отстувуют в списке, а в файле восстановления они имеются, проверьте их наличие в соответвующей папке рядом с файлом восстановления\n"
                        "Затем, нажмите кнопку Далее...";
            }
        } else {
            if (hasOldData) {
                message = "Данные для программ-ярлыков в режиме киоска, а так же программ которые будут перезапущены для роли: " + Roles.at(currentId() - 2) +
                        "\nВаша программа обнаружила уже имеющиеся данные, они были загружены, файл восстановления не выбран, "
                        "Так же вы можете не использовать имеющуюся базу, выбрав нужный вариант в самом нижнем поле, и создать новую, записав в нее только данные администратора.\n"
                        "Доступные программы разрешается отредактировать в интерфейсе программы позже.\n"
                        "Затем, нажмите кнопку Далее...";
            } else {
                message = "Данные для программ-ярлыков в режиме киоска, а так же программ которые будут перезапущены для роли: " + Roles.at(currentId() + 2) +
                        "\nК сожалению, у вас нет ни данных, ни файла восстановления, поэтому "
                        "Доступные программы разрешается отредактировать в интерфейсе программы позже.\n"
                        "Нажмите кнопку Далее...";
            }
        }

        break;

    case Page_Conclusion:
        message = "Заключительный этап настройки.\n"
                  "Проверьте правильность настроек, и импорт данных из источников.\n"
                  "Если все правильно, нажмите кнопку Завершить..\n."
                  "Иначе нажмите кнопку Назад, и измените необходимые параметры...\n";

        break;
    }

    QMessageBox::information(this, "Окно справки", message);
}

void StartupWizard::OnCurrentPageChanged(int id)
{
    Q_UNUSED(id);
    if(MyWizardPage *nextPage = qobject_cast<MyWizardPage*>(currentPage())){
         nextPage->SetWizardNavigationBar(m_wizardNavigationBar);
     }
}

void StartupWizard::OnPageNumMove(int pageIdToMove)
{
    if(currentId()==pageIdToMove)
    {
        return;
    }
    else
    {
        if(currentId()<pageIdToMove)
        {
            while (pageIdToMove!=currentId())
            {
                this->next();
            }
        }
        else
        {
            while (pageIdToMove!=currentId())
            {
                this->back();
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
