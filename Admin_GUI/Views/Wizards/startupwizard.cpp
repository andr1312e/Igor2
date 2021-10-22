#include "startupwizard.h"

StartupWizard::StartupWizard(ProgramFilesState &loadedDbAdnRolesState, AppFirstLoadlingSettingsService *appFirstLoadingService, QWidget *parent)
   : QWizard(parent)
   , m_programState(loadedDbAdnRolesState)
   , m_settingFileService(appFirstLoadingService)
{
   initServices();
   initUI();
   initSizes();
   initStyles();
   initBehaviour();
   createConnections();
}

StartupWizard::~StartupWizard()
{
   delete m_wizardService;
   delete m_introPage;
   delete m_fcsPage;

   for (int i = 0; i < Roles.size(); ++i) {
      delete m_rolesPages.at(i);
   }

   delete m_conclusionPage;
}

void StartupWizard::accept()
{
   m_wizardService->ApplySettings();
}

void StartupWizard::reject()
{
   int ccccc = 3;
}

void StartupWizard::showHelp()
{
   QString message;

   switch (currentId()) {
      case Page_Intro:
         message = "Выберите файл восстановления, нажав на кнопку папки справа. Если файл корректный он отобразится в поле файла. Если файла нет прочитайте руководство и нажмите далее";
         break;

      case Page_UserData:
         message = "Поле ввода данных администратора. Поле пользователей редактируется в программе позже...";
         break;

      case Page_FirstRole:
         message = "Данные для программ-ярлыков в режиме киоска, а так же программ которые будут перезапущены для роли %s", Roles.at(0).toLatin1().constData();
         break;

      case Page_SecondRole:
         message = "Данные для программ-ярлыков в режиме киоска, а так же программ которые будут перезапущены для роли %s", Roles.at(1).toLatin1().constData();
         break;

      case Page_ThirdRole:
         message = "Данные для программ-ярлыков в режиме киоска, а так же программ которые будут перезапущены для роли %s", Roles.at(2).toLatin1().constData();
         break;

      case Page_FourthRole:
         message = "Данные для программ-ярлыков в режиме киоска, а так же программ которые будут перезапущены для роли %s", Roles.at(3).toLatin1().constData();
         break;
   }

   QMessageBox::information(this, "Окно справки", message);
}

void StartupWizard::initServices()
{
   m_wizardService = new WizardService(m_programState, m_settingFileService->getUserName(), m_settingFileService->getUserId(), m_settingFileService->getValidSettingsPaths(), m_settingFileService->getDefaultSettingsPaths(), m_settingFileService->getTerminal(), nullptr);
}

void StartupWizard::initUI()
{
   m_introPage = new IntroPage(m_programState, m_wizardService, this);

   m_fcsPage = new UserWizardPage(m_wizardService, this);
   m_rolesPages.resize(Roles.size());

   for (int i = 0; i < Roles.size(); i++) {
      m_rolesPages[i] = new RoleAppsWizardPage(Roles.at(i), i, m_wizardService, this);
   }

   m_conclusionPage = new ConclusionWizardPage(m_wizardService, this);
}

void StartupWizard::initSizes()
{
   setMinimumSize(900, 700);
}

void StartupWizard::initStyles()
{
   setWizardStyle(QWizard::ModernStyle);
   this->setAutoFillBackground(true);
}

void StartupWizard::initBehaviour()
{

   setPage(Page_Intro, m_introPage);
   setPage(Page_UserData, m_fcsPage);

   for (int i = 0; i < m_rolesPages.size(); i++) {
      setPage(i + 2, m_rolesPages.at(i));
   }

   setPage(Page_Conclusion, m_conclusionPage);

   setStartId(Page_Intro);
   setOption(QWizard::HaveHelpButton);
}

void StartupWizard::createConnections()
{
   connect(this, &QWizard::helpRequested, this, &StartupWizard::showHelp);
}
