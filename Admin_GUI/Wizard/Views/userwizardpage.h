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
   UserWizardPage(WizardService *service, QWidget *parent);
   ~UserWizardPage();
private:
   void CreateUI();
   void InsertWidgetsIntoLayout();
   void CreateConnections();
Q_SIGNALS:
   void sendUpdatedDataToParentWizard(const QString &FCS, const QString &rank) const;
public:
   virtual int nextId() const Q_DECL_OVERRIDE;
   virtual void initializePage() Q_DECL_OVERRIDE;
   virtual bool isComplete() const Q_DECL_OVERRIDE;

private:
   const QString m_backupTitle = "Данные администратора хранящиеся в файле восстановления:";
   const QString m_oldTitle = "Данные администратора уже хранящиеся в локальной базе:";
   const QString m_noDataTitle = "Введите данные администратора: ФИО и Звание";
   const QStringList m_tableHeader = {"Ид", "Имя в системе", "ФИО", "Звание", "Роль"};

private:
   WizardService *m_wizardService;

   QLabel *m_topLabel;
   UserWizardWidget *m_oldFCSWidget = nullptr;
   UserWizardWidget *m_backupFCSWidget = nullptr;
   QComboBox *m_actionComboBox;

   QString m_backupFCSValue;
   QString m_backupRankValue;

   bool m_oldDataUsageValue;

};

#endif // ADMIN_GUI_VIEWS_USERWIZARDPAGE_H
