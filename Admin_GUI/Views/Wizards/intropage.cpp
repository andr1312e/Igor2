#include "intropage.h"
#include <QDebug>
#include <QMessageBox>


IntroPage::IntroPage(const LoadingState &state, WizardService *service, QWidget *parent)
   : MyWizardPage(parent)
   , m_state(state)
   , m_wizardService(service)
{
   CreateUI();
   SetWizardStyles();
   InsertWidgetsIntoLayout();
   CreateConnections();
}

IntroPage::~IntroPage()
{
   delete m_backupFileLayout;
   delete m_backupFileLoadLayout;

   delete m_topLabel;
   delete m_backupLoadButton;
   delete m_backupLabel;
   delete m_backupLineEdit;
   delete m_faqLabel;
}

int IntroPage::nextId() const
{
   return Page_UserData;
}

void IntroPage::SetWizardStyles()
{
   this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/1.jpg"));
   m_backupLineEdit->setObjectName("wizard");
}

void IntroPage::CreateUI()
{
   QString needToUpdate;

   switch (m_state) {
      case NoFiles: {
         needToUpdate = "Необходимо добавить: базу данных пользователей, и пути к исполняемым файлам.\n";
         break;
      }

      case NoUserDb: {
         needToUpdate = "Необходимо добавить базу данных пользователей.\n";
         break;
      }

      case NoRoleData: {
         needToUpdate = "Необходимо добавить: пути к исполняемым файлам. База пользователей имеется.\n";
         break;
      }

      default: {
         qFatal("Нереализованное поведение");
      }

   }

   m_topLabel = new QLabel(("Администратор, добро пожаловать в программу управления пользователями РЛС ТИ\n"
                            + needToUpdate +
                            "Eсли у вас есть готовый файл настроек, нажмите на иконку папки справа, выберите файл и нажмите далее..."
                            "\nЗначения файла можно будет отредактировать позже.\nЕсли файла нет, ознакомьтесь с кратким руководством и нажмите далее.."
                           ));
   m_topLabel->setWordWrap(true);

   m_backupFileLayout = new QVBoxLayout();
   m_backupFileLoadLayout = new QHBoxLayout();
   m_backupLoadButton = new QPushButton();
   m_backupLoadButton->setIcon(QIcon(":/images/folderBlack"));
   m_backupLoadButton->setIconSize(QSize(30, 30));
   m_backupLoadButton->setFlat(true);
   m_backupLoadButton->setStyleSheet("border: 0px;");
   m_backupLabel = new QLabel("Путь к файлам настроек:");
   m_backupLineEdit = new QLineEdit();
   m_backupLineEdit->setText("");
   m_faqLabel = new QLabel("<html>Краткое руководство - процесс настройки состоит из шагов:<ul><li>Внесение в базу ФИО Администратора</li><li>Заполнение ролей</li></ul> Заполнение ролей состоит из шагов:<ul><li></li><li>Заполнение доступных программ для запуска на рабочем столе</li><li>Заполнение списка программ которые будут перезапущены</li></ul></html>");

   m_backupLineEdit->setFrame(false);

}

void IntroPage::InsertWidgetsIntoLayout()
{
   m_backupFileLoadLayout->addWidget(m_backupLineEdit);
   m_backupFileLoadLayout->addWidget(m_backupLoadButton);

   m_backupFileLayout->addWidget(m_backupLabel);
   m_backupFileLayout->addLayout(m_backupFileLoadLayout);
   m_backupFileLayout->setContentsMargins(30, 0, 0, 0);

   MainLayout()->addWidget(m_topLabel);
   MainLayout()->addLayout(m_backupFileLayout);
   MainLayout()->addWidget(m_faqLabel);

}

void IntroPage::CreateConnections()
{
   connect(m_backupLoadButton, &QPushButton::clicked, this, &IntroPage::OnCheckBackupFile);
}

void IntroPage::OnCheckBackupFile()
{

   QPalette bgpal;
   bgpal.setColor(QPalette::Base, Qt::red);
   bgpal.setColor(QPalette::Text, Qt::green);
   m_backupLineEdit->setPalette(bgpal);
   QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
   m_backupFilePath = QFileDialog::getOpenFileName(nullptr, "Выберите исполняемый файл", strDesktop, "Файл настроек (*.sync)");

   if (!m_backupFilePath.isEmpty()) {
      if (m_wizardService->CheckAndParseBackupFile(m_backupFilePath)) {
         m_backupLineEdit->setText(m_backupFilePath);
      } else {
         m_backupLineEdit->clear();
      }
   }
}
