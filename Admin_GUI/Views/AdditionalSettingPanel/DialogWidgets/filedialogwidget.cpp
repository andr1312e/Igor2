#include "filedialogwidget.h"

FileDialogWidget::FileDialogWidget(QWidget *parent)
   : QWidget(parent)
{
   CreateUI();
   InsertWidgetsIntoLayout();
   FillUI();
   ConnectObject();
}

FileDialogWidget::~FileDialogWidget()
{
   delete m_dialogWidgetButtonsLayout;
   delete m_iconPathLayout;
   delete m_execPathLayout;
   delete m_mainLayout;

   delete m_titleLabel;
   delete m_exec;
   delete m_execButton;
   delete m_iconPath;
   delete m_iconPathButton;
   delete m_iconName;
   delete m_saveDialogButton;
   delete m_closeDialogButton;
   delete m_messagBox;
}


void FileDialogWidget::CreateUI()
{
   m_mainLayout = new QVBoxLayout;

   m_titleLabel = new QLabel();


   m_execPathLayout = new QHBoxLayout();

   m_exec = new QtMaterialTextField();


   m_execButton = new QPushButton();

   m_iconPathLayout = new QHBoxLayout();
   m_iconPath = new QtMaterialTextField();


   m_iconPathButton = new QPushButton("Выбрать файл");

   m_iconName = new QtMaterialTextField();
   m_iconName->setLabel("Название на рабочем столе: (Обязательно)");

   m_dialogWidgetButtonsLayout = new QHBoxLayout();
   m_saveDialogButton = new QPushButton("Применить");
   m_closeDialogButton = new QPushButton("Выйти без сохранения");

   m_messagBox = new QMessageBox();
}

void FileDialogWidget::InsertWidgetsIntoLayout()
{
   m_mainLayout->addWidget(m_titleLabel);

   m_execPathLayout->addWidget(m_exec);
   m_execPathLayout->addWidget(m_execButton);

   m_mainLayout->addLayout(m_execPathLayout);

   m_iconPathLayout->addWidget(m_iconPath);
   m_iconPathLayout->addWidget(m_iconPathButton);
   m_mainLayout->addLayout(m_iconPathLayout);

   m_mainLayout->addWidget(m_iconName);

   m_dialogWidgetButtonsLayout->addWidget(m_saveDialogButton);
   m_dialogWidgetButtonsLayout->addWidget(m_closeDialogButton);
   m_dialogWidgetButtonsLayout->setAlignment(Qt::AlignBottom);

   m_mainLayout->addLayout(m_dialogWidgetButtonsLayout);

   setLayout(m_mainLayout);
}

void FileDialogWidget::FillUI()
{
   m_iconPath->setLabel("Путь к иконке: (если отдельно от приложения)");
   m_titleLabel->setAlignment(Qt::AlignCenter);
   m_execButton->setText("Выбрать файл");
   m_exec->setLabel("Путь к исполняемому файлу: (Обязательно)");
   m_messagBox->setWindowTitle("Внимание!");
}

void FileDialogWidget::ConnectObject()
{
   connect(m_closeDialogButton, &QPushButton::clicked, this, &FileDialogWidget::OnHideDialog);
   connect(m_execButton, &QPushButton::clicked, this, &FileDialogWidget::OnAddingEcexutePath);
   connect(m_iconPathButton, &QPushButton::clicked, this, &FileDialogWidget::OnAddIconPath);
   connect(m_saveDialogButton, &QPushButton::clicked, this, &FileDialogWidget::OnAddIconToUserDesktop);
}

void FileDialogWidget::SetTitleText(QString &text)
{
   m_titleLabel->setText(text);
}


void FileDialogWidget::ClearAllTextFiels()
{
   m_exec->clear();
   m_iconPath->clear();
   m_iconName->clear();
}

void FileDialogWidget::OnHideDialog()
{
   ClearAllTextFiels();
   emit ToDialogSignalHide();
}

void FileDialogWidget::OnAddingEcexutePath()
{
   QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
   QString loadPath = QFileDialog::getOpenFileName(nullptr, "Выберите исполняемый файл", strDesktop);
   m_exec->setText(loadPath);
}

void FileDialogWidget::OnAddIconPath()
{
   QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
   QString loadPath = QFileDialog::getOpenFileName(nullptr, "Выберите исполняемый файл", strDesktop);
   m_iconPath->setText(loadPath);
}

void FileDialogWidget::OnAddIconToUserDesktop()
{
   if (m_exec->text() == "") {
      m_messagBox->setText("Вы не ввели текст в поле \"Путь к исполняемому файлу\". Данное поле обязательно");
      m_messagBox->exec();
   } else {
      if (m_iconName->text() == "") {
         m_messagBox->setText("Вы не ввели текст в поле \"Название на рабочем столе\". Данное поле обязательно");
         m_messagBox->exec();
      } else {
         if (FunctionsWithStrings::StringContainsBadCharecters(m_iconName->text())) {
            m_messagBox->setText("Текст в поле \"Название на рабочем столе\" не может содержать символы \ / ` : & * ? \" \' < > | () : ;");
            m_messagBox->exec();
         } else {
            if (QFile::exists(m_exec->text())) {
               emit ToAddFileToUserDesktop(m_exec->text(), m_iconPath->text(), m_iconName->text());
               OnHideDialog();
            } else {
               m_messagBox->setText("Вы ввели путь к файлу, которого не существует, попробуйте воспользоваться кнопкой \"Выбрать файл\" справа от поля \"Путь к исполняемому файлу\"");
               m_messagBox->exec();
            }
         }
      }
   }
}
