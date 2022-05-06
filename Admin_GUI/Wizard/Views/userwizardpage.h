#ifndef ADMIN_GUI_VIEWS_USERWIZARDPAGE_H
#define ADMIN_GUI_VIEWS_USERWIZARDPAGE_H

#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QHeaderView>
#include <QSizePolicy>
#include <QPushButton>

#include "Admin_GUI/Wizard/Structs/programfilesstate.h"
#include "Admin_GUI/Wizard/Repositories/usersettingsrepository.h"

#include "Admin_GUI/Wizard/Services/wizardservice.h"

#include "Admin_GUI/Wizard/Views/wizardpages.h"
#include "Admin_GUI/Validator/stringvalidator.h"


#include "Admin_GUI/Wizard/Views/wizardnavigtionbar.h"
#include "Admin_GUI/Wizard/Views/actions.h"
#include "Admin_GUI/Wizard/Views/userwizardwidget.h"
#include "Admin_GUI/Wizard/Views/mywizardpage.h"

class UserWizardPage: public MyWizardPage
{
   Q_OBJECT
public:
   explicit UserWizardPage(WizardService *service, QWidget *parent);
   ~UserWizardPage();
private:
   void CreateUI();
   void InsertWidgetsIntoLayout();
   void FillUI();
   void ConnectObjects();
Q_SIGNALS:
   void sendUpdatedDataToParentWizard(const QString &FCS, const QString &rank) const;
private Q_SLOTS:
   void OnActionComboBoxChanged(const QString &action);
public:
   virtual int nextId() const Q_DECL_OVERRIDE;
   virtual void initializePage() Q_DECL_OVERRIDE;
   virtual bool isComplete() const Q_DECL_OVERRIDE;

private:
   const QString m_backupTitle;
   const QString m_oldTitle;
   const QString m_noDataTitle;
   const QStringList m_tableHeader;

private:
   WizardService * const m_wizardService;

   QLabel *m_topLabel;
   UserWizardWidget *m_oldWidget;
   UserWizardWidget *m_backupWidget;
   QComboBox *m_actionComboBox;
};

#endif // ADMIN_GUI_VIEWS_USERWIZARDPAGE_H
