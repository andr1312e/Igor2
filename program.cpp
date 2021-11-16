#include "program.h"
#include <QDebug>


Program::Program(int &argc, char **argv)
   : QApplication(argc, argv, true)
   , m_loadedDbAdnRolesState(LoadingState::CantRun)
   , m_hasAdminPrivileges(false)
   , m_sharedMemory(new QSharedMemory("PROCESS_CONTROLLER", this))
   , m_terminal(nullptr)
   , m_linuxUserService(nullptr)
   , m_firstStartSettingsService(nullptr)
   , m_startupWizard(nullptr)
   , m_dataBaseService(nullptr)
   , m_indentifyService(nullptr)
   , m_startupRunnableService(nullptr)
   , m_socketToRarm(nullptr)
   , m_AdminGui(nullptr)
   , m_fakeUI(new FakeUI)
   , m_framelessWindow(nullptr)
   , m_styleChanger(nullptr)
{
   m_fakeUI->hide();
}

Program::~Program()
{
   m_sharedMemory->detach();
   delete m_sharedMemory;
   delete m_fakeUI;

   if (m_terminal != nullptr) {
      delete m_firstStartSettingsService;

      if (m_AdminGui != nullptr) {
         delete m_AdminGui;
      }

      delete m_indentifyService;
      delete m_dataBaseService;
      delete m_startupRunnableService;
      delete m_linuxUserService;
      delete m_socketToRarm;
      delete m_terminal;
   }
}

bool Program::HasNoRunningInscance()
{
   if (m_sharedMemory->attach(QSharedMemory::ReadOnly)) {
      m_sharedMemory->detach();
      return false;
   } else {
      if (m_sharedMemory->create(1, QSharedMemory::ReadWrite)) {
         return true;
      } else {
         qDebug() << m_sharedMemory->errorString();
         return false;
      }
   }
}

void Program::CreateApp()//MAIN
{
   InitTerminal();
   InitUserService();
   GetCurrentUserNameIdAndAdminPriviliges();
   InitSettingsService();
   GetProgramState();
   InitFramelessWindow();
   InitStyle();
   ProcessDataLoading();
}

void Program::ProcessDataLoading()
{

   switch (m_loadedDbAdnRolesState) {
      case NoFiles:
      case NoUserDb:
      case NoRoleData: {
         StartSettingsWizard();
         break;
      }

      case Fine: {
         OnContinueLoading();
         break;
      }

      case CantRun: {
         QMessageBox::critical(nullptr, "Приложение не может запуститься", "Файл настроек  имеет неверную структуру, обратитесь к Администратору для решения проблемы", QMessageBox::Ok);
         this->exit(0);
      }
   }
}

void Program::InitStyle()
{
   this->setStyle(QStringLiteral("Fusion"));
   m_styleChanger = new StyleChanger(this);
   m_styleChanger->OnChangeTheme(m_firstStartSettingsService->GetThemeValue());
}

void Program::InitFramelessWindow()
{
   m_framelessWindow = new FramelessWindow(Q_NULLPTR);
   m_framelessWindow->SetWindowIcon(QIcon(":/images/ico.png"));
   m_framelessWindow->SetWindowTitle("Мастер первоначальной настройки");
   m_framelessWindow->show();
}

void Program::StartSettingsWizard()
{
   m_startupWizard = new StartupWizard(m_loadedDbAdnRolesState, m_firstStartSettingsService, Q_NULLPTR);
   connect(m_startupWizard, &StartupWizard::ToChangeTheme, m_styleChanger, &StyleChanger::OnChangeTheme);
   connect(m_startupWizard, &StartupWizard::ToWizardFinished, this, &Program::OnContinueLoading);
   connect(m_startupWizard, &StartupWizard::ToQuit, this, &Program::quit);
   connect(m_startupWizard, &StartupWizard::ToSetDbAndIconsPaths, m_firstStartSettingsService, &FirstStartSettingsService::OnSetDbAndIconsPaths);
   m_framelessWindow->SetMainWidget(m_startupWizard);
}

void Program::OnContinueLoading()
{
   GetSettings();
   InitRunnableService();

   if (AllAppsRunned()) {
      InitRarmSocket();
      ConnectObjects();

      if (m_hasAdminPrivileges) {
         InitAdminServices();
         InitAdminUI();
      }
   }
}

void Program::InitTerminal()
{
   m_terminal = new Terminal();
}

void Program::InitUserService()
{
   m_linuxUserService = new LinuxUserService(m_terminal);
}

void Program::GetCurrentUserNameIdAndAdminPriviliges()
{
   m_hasAdminPrivileges = m_linuxUserService->hasCurrentUserAdminPrivileges();
   m_currentUserName = m_linuxUserService->getCurrentUserName();
   m_currentUserId = m_linuxUserService->getCurrentUserId();
}

void Program::InitSettingsService()
{
   m_firstStartSettingsService = new FirstStartSettingsService(m_currentUserName, m_currentUserId, m_hasAdminPrivileges, m_fakeUI, m_terminal);
}

void Program::GetProgramState()
{
   m_loadedDbAdnRolesState = m_firstStartSettingsService->IsAllDataLoaded();
}

void Program::GetSettings()
{
   m_userDBPath = m_firstStartSettingsService->GetUserDBPathValue();
}

void Program::InitRunnableService()
{
   m_startupRunnableService = new StartupRunnableService(m_terminal, this);
}

bool Program::AllAppsRunned()
{
   return true;
   //    return m_startupRunnableService->run(m_linuxUserService->getCurrentUserName());
}

void Program::InitRarmSocket()
{
   m_socketToRarm = new SocketToRarm("127.0.0.1:4242", this);
}

void Program::InitAdminServices()
{
   m_dataBaseService = new DatabaseService(m_terminal);
   m_dataBaseService->LoadFromFile(m_userDBPath);
   m_indentifyService = new IdentifyService(m_terminal, m_dataBaseService, m_linuxUserService);
   m_linuxUserService->getAllUsersInSystem();
}

void Program::InitAdminUI()
{
   if (m_indentifyService->canGetAccess()) {

      m_AdminGui = new Admin_GUI(m_dataBaseService, m_linuxUserService);
      connect(m_AdminGui, &Admin_GUI::ToChangeTheme, m_styleChanger, &StyleChanger::OnChangeTheme);
      m_framelessWindow->SetWindowIcon(QIcon(":/images/ico.png"));
      m_framelessWindow->SetWindowTitle("Панель управления пользователями и модулями РЛС ТИ");
      m_framelessWindow->SetMainWidget(m_AdminGui);
   }
}

void Program::ConnectObjects()
{
   connect(m_startupRunnableService, &StartupRunnableService::ToProgramFall, m_socketToRarm, &SocketToRarm::OnProgramFall);
}

void Program::StartAdminServices()
{
   m_linuxUserService->getAllUsersInSystem();
}
