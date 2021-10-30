#ifndef ADMIN_GUI_VIEWS_WIZARD_MYWIZARDPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_MYWIZARDPAGE_H

#include <QVBoxLayout>
#include <QWizardPage>
#include <QPushButton>


class MyWizardPage: public QWizardPage
{
   Q_OBJECT
public:
   explicit MyWizardPage(QWidget *parent);
   ~MyWizardPage();

   void SetWidget(QPushButton *themeChangeButton);

   QVBoxLayout *MainLayout() const;

private:
   QVBoxLayout *m_layout;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_MYWIZARDPAGE_H
