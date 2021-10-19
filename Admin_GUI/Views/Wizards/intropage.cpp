#include "intropage.h"
#include <QDebug>
#include <QMessageBox>


IntroPage::IntroPage(ProgramFilesState &state, WizardService *service, QWidget *parent)
    : QWizardPage(parent)
    , m_state(state)
    , m_wizardService(service)

{
    initUI();
    setWizardTitle();
    insertWidgetsIntoLayout();
    createConnections();
}

IntroPage::~IntroPage()
{
    delete m_backupFileLayout;
    delete m_backupFileLoadLayout;
    delete m_mainLayout;

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

void IntroPage::setWizardTitle()
{
    this->setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/1.jpg"));
    m_backupLineEdit->setStyleSheet("border: 1px solid black");
}

void IntroPage::initUI()
{
    m_mainLayout=new QVBoxLayout();

    QString needToUpdate;
    switch (m_state) {
    case NoFiles:
    {
        needToUpdate="Необходимо добавить: базу данных пользователей, и пути к исполняемым файлам.\n";
        break;
    }
    case NoUserDb:
    {
        needToUpdate="Необходимо добавить базу данных пользователей.\n";
        break;
    }
    case NoRoleData:
    {
        needToUpdate="Необходимо добавить: пути к исполняемым файлам. База пользователей имеется.\n";
        break;
    }

    }

    m_topLabel = new QLabel(("Администратор, добро пожаловать в программу управления пользователями РЛС ТИ\n"
                             +needToUpdate+
                             "Eсли у вас есть готовый файл настроек, нажмите на иконку папки справа сверху, выберите файл"
                             ",нажмите далее. Значения файла можно будет отредактировать позже.Если файла нет, ознакомьтесь с кратким руководством и нажмите далее.."
                             ));
    m_topLabel->setWordWrap(true);

    m_backupFileLayout=new QVBoxLayout();
    m_backupFileLoadLayout=new QHBoxLayout();
    m_backupLoadButton=new QPushButton();
    m_backupLoadButton->setIcon(QIcon(":/images/folderBlack"));
    m_backupLoadButton->setIconSize(QSize(30, 30));
    m_backupLoadButton->setFlat(true);
    m_backupLoadButton->setStyleSheet("border: 0px;");
    m_backupLabel=new QLabel("Путь к файлам настроек:");
    m_backupLineEdit=new QLineEdit();
    m_backupLineEdit->setText("");
    m_faqLabel=new QLabel("<html>Краткое руководство - процесс настройки состоит из шагов:<ul><li>Внесение в базу ФИО Администратора</li><li>Заполнение ролей</li></ul> Заполнение ролей состоит из:<ul><li></li><li>Заполнение доступных программ для запуска на рабочем столе</li><li>Заполнение списка программ которые будут перезапущены</li></ul></html>");
}

void IntroPage::insertWidgetsIntoLayout()
{
    m_backupFileLoadLayout->addWidget(m_backupLineEdit);
    m_backupFileLoadLayout->addWidget(m_backupLoadButton);

    m_backupFileLayout->addWidget(m_backupLabel);
    m_backupFileLayout->addLayout(m_backupFileLoadLayout);
    m_backupFileLayout->setContentsMargins(30, 0, 0, 0);

    m_mainLayout->addWidget(m_topLabel);
    m_mainLayout->addLayout(m_backupFileLayout);
    m_mainLayout->addWidget(m_faqLabel);

    setLayout(m_mainLayout);
}

void IntroPage::createConnections()
{
    connect(m_backupLoadButton, &QPushButton::clicked, this, &IntroPage::CheckBackupFile);
}

void IntroPage::CheckBackupFile()
{
    QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    m_backupFilePath = QFileDialog::getOpenFileName(nullptr, "Выберите исполняемый файл", strDesktop, "Файл настроек (*.kunzevo)");
    if (!m_backupFilePath.isEmpty())
    {
        if (m_wizardService->CheckAndParseBackupFile(m_backupFilePath))
        {
             m_backupLineEdit->setText(m_backupFilePath);
        }
        else
        {
            m_backupLineEdit->clear();
        }
    }
}
