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

void MyWizardPage::SetWidget(QPushButton *themeChangeButton)
{
   if (themeChangeButton) {
      m_layout->insertWidget(0, themeChangeButton);
   }
}

QVBoxLayout *MyWizardPage::MainLayout() const
{
   return m_layout;
}
