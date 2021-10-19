#include "startupwizard.h"

StartupWizard::StartupWizard(int state, QString &currentUserName, QString &currentUserId, QStringList params, QWidget *parent)
    : QWizard(parent)
    , m_backupDocument(new QDomDocument())
{
    initServices(userWizardRepository, roleWizardRepostory);
    initUI();
    initSizes();
    initStyles();
    initBehaviour();
    createConnections();
}

StartupWizard::~StartupWizard()
{
    delete m_backupDocument;
    delete m_introPage;
    delete m_fcsPage;
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

void StartupWizard::initServices(CurrentUserWizardRepository *userWizardRepository, RoleAndStartupWizardRepository *roleWizardRepostory)
{
    m_userWizardRepository=userWizardRepository;
    m_roleWizardRepostory=roleWizardRepostory;
}

void StartupWizard::initUI()
{
    m_introPage =new IntroPage(m_backupDocument,this);

    m_fcsPage=new FCSPage(m_userWizardRepository->getCurrentUserName(), m_userWizardRepository->getCurrentUserId(),m_backupDocument, this);
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
    setPage(Page_Intro, m_introPage);
    setPage(Page_UserData, m_fcsPage);
    setStartId(Page_Intro);
}

void StartupWizard::createConnections()
{

}
