#ifndef ADMIN_GUI_VIEWS_WIZARD_MYWIZARDPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_MYWIZARDPAGE_H

#include <QVBoxLayout>
#include <QWizardPage>
#include <QPushButton>

#include "Admin_GUI/Views/Wizards/wizardnavigtionbar.h"

class MyWizardPage: public QWizardPage
{
   Q_OBJECT
public:
   explicit MyWizardPage(QWidget *parent);
   ~MyWizardPage();

   void SetWizardNavigationBar(WizardNavigtionBar *wizardNavigtionBar);

   QVBoxLayout *MainLayout() const;

private:
   QVBoxLayout* const m_layout;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_MYWIZARDPAGE_H
