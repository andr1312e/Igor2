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
}

void StartupWizard::accept()
{
    int ccc=3;
}

void StartupWizard::reject()
{
    int ccccc=3;
}

void StartupWizard::showHelp()
{
    QString message;
    switch(currentId())
    {
    case Page_Intro:
        message = tr("Введите ФИО ");
    }
}

void StartupWizard::initServices()
{
    m_wizardService=new WizardService(m_programState, m_settingFileService->getUserName(), m_settingFileService->getUserId(), m_settingFileService->getValidSettingsPaths(), m_settingFileService->getDefaultSettingsPaths(), m_settingFileService->getTerminal(), nullptr);
}

void StartupWizard::initUI()
{
    m_introPage =new IntroPage(m_programState, m_wizardService,this);

    m_fcsPage=new FCSPage(m_wizardService, this);
}

void StartupWizard::initSizes()
{
    setMinimumSize(600, 400);
}

void StartupWizard::initStyles()
{
    setWizardStyle(QWizard::ModernStyle);
    this->setAutoFillBackground(true);
}

void StartupWizard::initBehaviour()
{
    setOption(QWizard::NoBackButtonOnStartPage);
    setOption(QWizard::HaveHelpButton);
    setPage(Page_Intro, m_introPage);
    setPage(Page_UserData, m_fcsPage);
    setStartId(Page_Intro);
}

void StartupWizard::createConnections()
{

//    connect(m_wizardService, &WizardService::setFCSForm, [&](bool isOldData, QString &adminFCS, QString &adminRank){m_fcsPage->set});
}
