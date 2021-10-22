#ifndef FCSPAGE_H
#define FCSPAGE_H
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QHeaderView>
#include <QSizePolicy>

#include "Services/Settings/programfilesstate.h"
#include "Services/Settings/usersettingsrepository.h"
#include "Services/Settings/wizardservice.h"

#include "Admin_GUI/Views/Wizards/wizardpages.h"
#include "Admin_GUI/Validator/stringvalidator.h"

#include "Admin_GUI/Views/Wizards/actions.h"
#include "Admin_GUI/Views/Wizards/userwizardwidget.h"

class UserWizardPage: public QWizardPage
{
   Q_OBJECT
public:
   UserWizardPage(WizardService *service, QWidget *parent);
   ~UserWizardPage();
   int nextId() const override;
   void initializePage() override;
   bool isComplete() const override;
private:
   WizardService *m_wizardService;

   QVBoxLayout *m_mainLayout;
   QLabel *m_topLabel;
   UserWizardWidget *m_oldFCSWidget = nullptr;
   UserWizardWidget *m_backupFCSWidget = nullptr;
   QComboBox *m_actionComboBox;

   QString m_backupFCSValue;
   QString m_backupRankValue;

   bool m_oldDataUsageValue;
private:
   QString m_backupTitle = "Данные администратора хранящиеся в файле восстановления:";
   QString m_oldTitle = "Данные администратора уже хранящиеся в локальной базе:";
   QString m_noDataTitle = "Введите данные администратора: ФИО и Звание";
   const QStringList m_tableHeader = {"Ид", "Имя в системе", "ФИО", "Звание", "Роль"};
private:
   void initUI();
   void insertWidgetsIntoLayout();
   void createConnections();
Q_SIGNALS:
   void sendUpdatedDataToParentWizard(const QString &FCS, const QString &rank) const;

   // QWidget interface
public:
   //    QSize sizeHint() const override;
};

#endif // FCSPAGE_H
