#include "conclusionwizardpage.h"

ConclusionWizardPage::ConclusionWizardPage(WizardService *service, QWidget *parent)
   : QWizardPage(parent)
   , m_wizardService(service)
{
   CreateUI();
   InsertWidgetsIntoLayout();
}

ConclusionWizardPage::~ConclusionWizardPage()
{
   delete m_oldDataLayout;
   delete m_backupLayout;
   delete m_dataLayout;
   delete m_mainLayout;

   delete m_titleLabel;
   delete m_backupLabel;
   delete m_backupValue;
   delete m_oldDataLabel;
   delete m_oldDataValue;
   delete m_actionsLabel;
   delete m_usersLabel;
   delete m_usersActionValue;
   delete m_usersCommentLabel;
   delete m_usersCommentValue;
   delete m_firstRoleLabel;
   delete m_firstRoleActionValue;
   delete m_firstRoleCommentLabel;
   delete m_firstRoleCommentValue;
   delete m_secondRoleLabel;
   delete m_secondRoleActionValue;
   delete m_secondRoleCommentLabel;
   delete m_secondRoleCommentValue;
   delete m_thirdRoleLabel;
   delete m_thirdRoleActionValue;
   delete m_thirdRoleCommentLabel;
   delete m_thirdRoleCommentValue;
   delete m_fourthRoleLabel;
   delete m_fourthRoleActionValue;
   delete m_fourthRoleCommentLabel;
   delete m_fourthRoleCommentValue;
}

int ConclusionWizardPage::nextId() const
{
   return -1;
}

void ConclusionWizardPage::initializePage()
{
   QString actionWithUserRepository = m_wizardService->GetActionWithUserRepository();
   SetUserActionValueLabel(actionWithUserRepository);


   QString actionWithRoleRepository ;

   for (int i = 0; i < Roles.count(); ++i) {
      actionWithRoleRepository = m_wizardService->GetActionWithRoleRepository(i);
      SetRolesActionValueLabel(i, actionWithRoleRepository);
   }
}

void ConclusionWizardPage::CreateUI()
{
   m_mainLayout = new QVBoxLayout();
   m_titleLabel = new QLabel("Итоговые настройки\n"
                             "Настройки пока НЕ применены, проверьте корректность ввода данных.\n"
                             "Если все настройки выставлены правильно, нажмите кнопку Завершить. Иначе, нажмите Назад и выполните настройку еще раз...");
   m_backupLayout = new QHBoxLayout();
   m_backupLabel = new QLabel("Файл восстановления: ");
   m_backupValue = new QLabel();

   m_oldDataLayout = new QHBoxLayout();
   m_oldDataLabel = new QLabel("Уже имеющиеся файлы: ");
   m_oldDataValue = new QLabel();

   m_actionsLabel = new QLabel("Выбранные действия");
   m_dataLayout = new QGridLayout();

   m_usersLabel = new QLabel("База данных пользователей:");
   m_usersActionValue = new QLabel();
   m_usersCommentLabel = new QLabel("Колличество пользователей:");
   m_usersCommentValue = new QLabel();

   m_firstRoleLabel = new QLabel("Роль " + Roles.at(0));
   m_firstRoleActionValue = new QLabel();
   m_firstRoleCommentLabel = new QLabel("Колличество программ:");
   m_firstRoleCommentValue = new QLabel();

   m_secondRoleLabel = new QLabel("Роль " + Roles.at(1));
   m_secondRoleActionValue = new QLabel();
   m_secondRoleCommentLabel = new QLabel("Колличество программ:");
   m_secondRoleCommentValue = new QLabel();

   m_thirdRoleLabel = new QLabel("Роль " + Roles.at(2));
   m_thirdRoleActionValue = new QLabel();
   m_thirdRoleCommentLabel = new QLabel("Колличество программ:");
   m_thirdRoleCommentValue = new QLabel();

   m_fourthRoleLabel = new QLabel("Роль " + Roles.at(3));
   m_fourthRoleActionValue = new QLabel();
   m_fourthRoleCommentLabel = new QLabel("Колличество программ:");
   m_fourthRoleCommentValue = new QLabel();
}

void ConclusionWizardPage::InsertWidgetsIntoLayout()
{
   m_backupLayout->addWidget(m_backupLabel);
   m_backupLayout->addWidget(m_backupValue);

   m_oldDataLayout->addWidget(m_oldDataLabel);
   m_oldDataLayout->addWidget(m_oldDataValue);

   m_dataLayout->addWidget(m_usersLabel, 0, 0);
   m_dataLayout->addWidget(m_usersActionValue, 0, 1);
   m_dataLayout->addWidget(m_usersCommentLabel, 0, 2);
   m_dataLayout->addWidget(m_usersCommentValue, 0, 3);

   m_dataLayout->addWidget(m_firstRoleLabel, 1, 0);
   m_dataLayout->addWidget(m_firstRoleActionValue, 1, 1);
   m_dataLayout->addWidget(m_firstRoleCommentLabel, 1, 2);
   m_dataLayout->addWidget(m_firstRoleCommentValue, 1, 3);

   m_dataLayout->addWidget(m_secondRoleLabel, 2, 0);
   m_dataLayout->addWidget(m_secondRoleActionValue, 2, 1);
   m_dataLayout->addWidget(m_secondRoleCommentLabel, 2, 2);
   m_dataLayout->addWidget(m_secondRoleCommentValue, 2, 3);

   m_dataLayout->addWidget(m_thirdRoleLabel, 3, 0);
   m_dataLayout->addWidget(m_thirdRoleActionValue, 3, 1);
   m_dataLayout->addWidget(m_thirdRoleCommentLabel, 3, 2);
   m_dataLayout->addWidget(m_thirdRoleCommentValue, 3, 3);

   m_dataLayout->addWidget(m_fourthRoleLabel, 4, 0);
   m_dataLayout->addWidget(m_fourthRoleActionValue, 4, 1);
   m_dataLayout->addWidget(m_fourthRoleCommentLabel, 4, 2);
   m_dataLayout->addWidget(m_fourthRoleCommentValue, 4, 3);

   m_mainLayout->addWidget(m_titleLabel);
   m_mainLayout->addLayout(m_backupLayout);
   m_mainLayout->addLayout(m_oldDataLayout);
   m_mainLayout->addWidget(m_actionsLabel);
   m_mainLayout->addLayout(m_dataLayout);

   setLayout(m_mainLayout);
}

void ConclusionWizardPage::SetUserActionValueLabel(QString &actionWithUserRepository)
{
   m_usersActionValue->setText(actionWithUserRepository);

   if (actionWithUserRepository == userWizardPageComboBoxBackupAndOldDataActions.at(0)) {
      m_usersCommentValue->setText("один пользователь");
   } else {
      if (actionWithUserRepository == userWizardPageComboBoxBackupAndOldDataActions.at(1)) {
         if (m_wizardService->HasUserBackup()) {
            m_usersCommentValue->setText(QString::number(m_wizardService->GetUserCountFromUserRepository(false)));
         } else {
            qFatal("Не реализованное поведение: файл восстановления есть, но колличество пользователей с него не может быть получено. ConclusionWizardPage::setUserRepositoryLabel");
         }
      } else {
         if (m_wizardService->HasUserOldData()) {
            m_usersCommentValue->setText(QString::number(m_wizardService->GetUserCountFromUserRepository(true)));
         } else {
            qFatal("Не реализованное поведение: старые данные есть, но колличество пользователей с него не может быть получено. ConclusionWizardPage::setUserRepositoryLabel");
         }
      }
   }
}

void ConclusionWizardPage::SetRolesActionValueLabel(int &roleIndex, const QString &actionWithRoleReposiory)
{
   switch (roleIndex) {
      case 0:
         m_firstRoleActionValue->setText(actionWithRoleReposiory);

         if (m_rolesWizardPageComboBoxBackupAndOldDataActions.at(0) == actionWithRoleReposiory) {
            m_firstRoleCommentValue->setText("нет программ");
         } else {
            if (m_rolesWizardPageComboBoxBackupAndOldDataActions.at(1) == actionWithRoleReposiory) {
               m_firstRoleCommentValue->setText(QString::number(m_wizardService->GetUserCountFromDesktopRepository(roleIndex, false)));
            } else {
               m_firstRoleCommentValue->setText(QString::number(m_wizardService->GetUserCountFromDesktopRepository(roleIndex, true)));
            }
         }

         break;

      case 1:
         m_secondRoleActionValue->setText(actionWithRoleReposiory);

         if (m_rolesWizardPageComboBoxBackupAndOldDataActions.at(0) == actionWithRoleReposiory) {
            m_secondRoleCommentValue->setText("нет программ");
         } else {
            if (m_rolesWizardPageComboBoxBackupAndOldDataActions.at(1) == actionWithRoleReposiory) {
               m_secondRoleCommentValue->setText(QString::number(m_wizardService->GetUserCountFromDesktopRepository(roleIndex, false)));
            } else {
               m_secondRoleCommentValue->setText(QString::number(m_wizardService->GetUserCountFromDesktopRepository(roleIndex, true)));
            }
         }

         break;

      case 2:
         m_thirdRoleActionValue->setText(actionWithRoleReposiory);

         if (m_rolesWizardPageComboBoxBackupAndOldDataActions.at(0) == actionWithRoleReposiory) {
            m_thirdRoleCommentValue->setText("нет программ");
         } else {
            if (m_rolesWizardPageComboBoxBackupAndOldDataActions.at(1) == actionWithRoleReposiory) {
               m_thirdRoleCommentValue->setText(QString::number(m_wizardService->GetUserCountFromDesktopRepository(roleIndex, false)));
            } else {
               m_thirdRoleCommentValue->setText(QString::number(m_wizardService->GetUserCountFromDesktopRepository(roleIndex, true)));
            }
         }

         break;

      case 3:
         m_fourthRoleActionValue->setText(actionWithRoleReposiory);

         if (m_rolesWizardPageComboBoxBackupAndOldDataActions.at(0) == actionWithRoleReposiory) {
            m_fourthRoleCommentValue->setText("нет программ");
         } else {
            if (m_rolesWizardPageComboBoxBackupAndOldDataActions.at(1) == actionWithRoleReposiory) {
               m_fourthRoleCommentValue->setText(QString::number(m_wizardService->GetUserCountFromDesktopRepository(roleIndex, false)));
            } else {
               m_fourthRoleCommentValue->setText(QString::number(m_wizardService->GetUserCountFromDesktopRepository(roleIndex, true)));
            }
         }

         break;

      default:
         qFatal("Роли только четрые");
         break;
   }
}
