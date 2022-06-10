#ifndef ADMIN_GUI_VIEWS_WIZARD_MYWIZARDPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_MYWIZARDPAGE_H

#include <QWizardPage>

#include "Admin_GUI/RestoreWizard/Views/wizardnavigtionbar.h"

class MyWizardPage: public QWizardPage
{
    Q_OBJECT
public:
    explicit MyWizardPage(QWidget *parent);
    ~MyWizardPage();
public:
    void SetNavigationBar(WizardNavigtionBar *wizardNavigtionBar);
    QVBoxLayout *MainLayout() const noexcept;

private:
    QVBoxLayout *const m_layout;
    QHBoxLayout *m_versionLayout;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_MYWIZARDPAGE_H
