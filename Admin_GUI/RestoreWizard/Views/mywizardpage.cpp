#include "mywizardpage.h"

MyWizardPage::MyWizardPage(QWidget *parent)
    : QWizardPage(parent)
    , m_layout(new QVBoxLayout())
{
    setLayout(m_layout);
}

MyWizardPage::~MyWizardPage()
{
    delete m_layout;
}

void MyWizardPage::SetNavigationBar(WizardNavigtionBar *wizardNavigtionBar)
{
    m_layout->insertWidget(0, wizardNavigtionBar, 0, Qt::AlignCenter);
}

QVBoxLayout *MyWizardPage::MainLayout() const noexcept
{
    return m_layout;
}
