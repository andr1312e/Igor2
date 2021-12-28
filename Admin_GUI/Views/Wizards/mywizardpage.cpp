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

void MyWizardPage::SetWizardNavigationBar(WizardNavigtionBar *wizardNavigtionBar)
{
   if (wizardNavigtionBar) {
      m_layout->insertWidget(0, wizardNavigtionBar, 0, Qt::AlignCenter);
   }
}

QVBoxLayout *MyWizardPage::MainLayout() const
{
   return m_layout;
}
