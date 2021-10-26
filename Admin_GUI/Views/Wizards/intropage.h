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

#include "Admin_GUI/Views/Wizards/wizardpages.h"

class IntroPage : public QWizardPage
{
   Q_OBJECT
public:
   IntroPage(const ProgramState &state, WizardService *service, QWidget *parent);
   ~IntroPage();
   int nextId() const override;
private:
   const ProgramState m_state;
   WizardService *m_wizardService;

   QVBoxLayout *m_mainLayout;
   QLabel *m_topLabel;
   QVBoxLayout *m_backupFileLayout;
   QHBoxLayout *m_backupFileLoadLayout;
   QPushButton *m_backupLoadButton;
   QLabel *m_backupLabel;
   QLineEdit *m_backupLineEdit;
   QLabel *m_faqLabel;

   QString m_backupFilePath;
private:
   void SetWizardTitle();
   void CreateUI();
   void InsertWidgetsIntoLayout();
   void CreateConnections();
private Q_SLOTS:
   void CheckBackupFile();
};

#endif // ADMIN_GUI_VIEWS_WIZARD_INTROPAGE_H
