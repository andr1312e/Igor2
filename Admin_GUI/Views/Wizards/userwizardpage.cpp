#include "userwizardpage.h"

UserWizardPage::UserWizardPage(WizardService *service, QWidget *parent)
   : QWizardPage(parent)
   , m_wizardService(service)
   , m_oldDataUsageValue(false)
{
   initUI();
   insertWidgetsIntoLayout();
   createConnections();
}

UserWizardPage::~UserWizardPage()
{
   delete m_mainLayout;

   delete m_topLabel;
   delete m_oldFCSWidget;
   delete m_backupFCSWidget;
   delete m_actionComboBox;
}

int UserWizardPage::nextId() const
{
   return Page_FirstRole;
}

void UserWizardPage::initializePage()
{
   QString adminFCS, adminRank;
   QVector<User> users;

   if (m_wizardService->HasBackup()) {
      m_backupFCSWidget->setVisible(true);
      m_wizardService->GetDataFromUserRepository(false, adminFCS, adminRank, users);
      m_backupFCSWidget->setWizardWidgetFileds(m_backupTitle, adminFCS, adminRank, users);

      if (m_wizardService->HasOldData()) {
         m_oldFCSWidget->setVisible(true);
         m_wizardService->GetDataFromUserRepository(true, adminFCS, adminRank, users);
         m_oldFCSWidget->setWizardWidgetFileds(m_oldTitle, adminFCS, adminRank, users);
         m_actionComboBox->addItems(userWizardPageComboBoxBackupAndOldDataActions);

      } else {
         m_oldFCSWidget->setVisible(false);
         m_actionComboBox->addItems(userWizardPageComboBoxBackupActions);
      }

      m_actionComboBox->setCurrentIndex(1);
   } else {
      m_oldFCSWidget->setVisible(true);
      m_backupFCSWidget->setVisible(false);

      if (m_wizardService->HasOldData()) {
         m_wizardService->GetDataFromUserRepository(true, adminFCS, adminRank, users);
         m_oldFCSWidget->setWizardWidgetFileds(m_oldTitle, adminFCS, adminRank, users);
         m_actionComboBox->addItems(userWizardPageComboBoxOldDataActions);
         m_actionComboBox->setCurrentIndex(1);
      } else {
         m_oldFCSWidget->setWizardWidgetFileds(m_noDataTitle, adminFCS, adminRank, users);
         m_actionComboBox->addItems(userWizardPageComboBoxNoDataActions);
      }
   }
}

bool UserWizardPage::isComplete() const
{
   //    qDebug()<<"fff";
   return true;
   //    return !m_backupFCSLineEdit->text().remove(' ').isEmpty();
}

void UserWizardPage::initUI()
{
   m_mainLayout = new QVBoxLayout();
   m_topLabel = new QLabel("Данные администратора");
   m_oldFCSWidget = new UserWizardWidget(m_tableHeader, this);
   m_backupFCSWidget = new UserWizardWidget(m_tableHeader, this);
   m_actionComboBox = new QComboBox();
}

void UserWizardPage::insertWidgetsIntoLayout()
{
   m_mainLayout->addWidget(m_topLabel);

   m_mainLayout->addWidget(m_oldFCSWidget);
   m_mainLayout->addWidget(m_backupFCSWidget);
   m_mainLayout->addWidget(m_actionComboBox);
   //    m_mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

   setLayout(m_mainLayout);

}

void UserWizardPage::createConnections()
{
   connect(m_actionComboBox, &QComboBox::currentTextChanged, [&](const QString & text) {
      m_wizardService->SetActionWithUserRepository(text);
   });
}

//QSize FCSPage::sizeHint() const
//{
//     return QSize(1800, 1500);
//}
