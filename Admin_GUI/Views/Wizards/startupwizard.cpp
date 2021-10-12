#include "startupwizard.h"

StartupWizard::StartupWizard(QWidget *parent)
    : QWizard(parent)
{
    applyStyles();
    initWizardPages();

    setOption(QWizard::NoBackButtonOnStartPage);
    setPage(Page_Intro, m_introPage);
    setPage(Page_FirstRole, m_fcsPage);
    setStartId(Page_Intro);

}

StartupWizard::~StartupWizard()
{
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

void StartupWizard::initWizardPages()
{
    m_introPage =new IntroPage(this);
    m_fcsPage=new FCSPage(this);
}

void StartupWizard::applyStyles()
{
        setWizardStyle(QWizard::ModernStyle);
    setMinimumSize(600, 400);
    this->setAutoFillBackground(true);
}
