#ifndef STARTUPWIZARD_H
#define STARTUPWIZARD_H

#include <QWizard>
#include <QLabel>
#include <QLineEdit>

#include "Admin_GUI/Views/Wizards/wizardpages.h"
#include "Admin_GUI/Views/Wizards/intropage.h"
#include "Admin_GUI/Views/Wizards/fcspage.h"

class StartupWizard : public QWizard
{
    Q_OBJECT
public:
    StartupWizard(QWidget *parent);
    ~StartupWizard();
private Q_SLOTS:
    void showHelp();
private:
    IntroPage *m_introPage;
    FCSPage *m_fcsPage;
private:
    void initWizardPages();
    void applyStyles();
};

#endif // STARTUPWIZARD_H
