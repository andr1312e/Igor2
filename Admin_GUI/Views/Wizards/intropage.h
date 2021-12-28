#ifndef ADMIN_GUI_VIEWS_WIZARD_INTROPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_INTROPAGE_H
#include <QLabel>
#include <QWizardPage>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QStandardPaths>

#include "Services/Settings/wizardservice.h"
#include "Services/Settings/programfilesstate.h"

#include "Admin_GUI/Views/Wizards/wizardnavigtionbar.h"
#include "Admin_GUI/Views/Wizards/wizardpages.h"
#include "Admin_GUI/Views/Wizards/mywizardpage.h"

class IntroPage : public MyWizardPage
{
   Q_OBJECT
public:
   explicit IntroPage(const LoadingState &state, WizardService *service, QWidget *parent);
   ~IntroPage();

private Q_SLOTS:
   void OnCheckBackupFile();

public:
   virtual int nextId() const Q_DECL_OVERRIDE;

private:
   void SetWizardStyles();
   void CreateUI();
   void InsertWidgetsIntoLayout();
   void CreateConnections();

private:
   const LoadingState m_state;
   WizardService *m_wizardService;

   QLabel *m_topLabel;
   QVBoxLayout *m_backupFileLayout;
   QHBoxLayout *m_backupFileLoadLayout;
   QPushButton *m_backupLoadButton;
   QLabel *m_backupLabel;
   QLineEdit *m_backupLineEdit;
   QLabel *m_faqLabel;

   QString m_backupFilePath;

};

#endif // ADMIN_GUI_VIEWS_WIZARD_INTROPAGE_H
