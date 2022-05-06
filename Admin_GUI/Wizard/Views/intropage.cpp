#include "intropage.h"
#include <QDebug>
#include <QMessageBox>


IntroPage::IntroPage(const LoadingState &state, WizardService *service, ThemesNames themeName, QWidget *parent)
    : MyWizardPage(parent)
    , m_state(state)
    , m_wizardService(service)
{
    CreateUI();
    SetWizardStyles(themeName);
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

IntroPage::~IntroPage()
{
    delete m_backupFileLoadLayout;
    delete m_backupFileLayout;

    delete m_topLabel;
    delete m_backupLoadButton;
    delete m_backupClearButton;
    delete m_backupLabel;
    delete m_backupLineEdit;
    delete m_faqLabel;
}

void IntroPage::CreateUI()
{
    m_backupFileLayout = new QVBoxLayout();
    m_backupFileLoadLayout = new QHBoxLayout();

    m_topLabel = new QLabel();

    m_backupLoadButton = new QPushButton();
    m_backupClearButton = new QPushButton();

    m_backupLabel = new QLabel();
    m_backupLineEdit = new QLineEdit();

    m_faqLabel = new QLabel();
    m_errorLabel=new QLabel();
}

void IntroPage::InsertWidgetsIntoLayout()
{
    m_backupFileLoadLayout->addWidget(m_backupLineEdit);
    m_backupFileLoadLayout->addWidget(m_backupLoadButton);
    m_backupFileLoadLayout->addWidget(m_backupClearButton);

    m_backupFileLayout->addWidget(m_backupLabel);
    m_backupFileLayout->addLayout(m_backupFileLoadLayout);
    m_backupFileLayout->setContentsMargins(30, 0, 0, 0);

    MainLayout()->addWidget(m_topLabel);
    MainLayout()->addLayout(m_backupFileLayout);
    MainLayout()->addWidget(m_faqLabel);
    MainLayout()->addWidget(m_errorLabel, 2, Qt::AlignBottom);

}

void IntroPage::FillUI()
{
    QString needToUpdate;

    switch (m_state) {
    case NoFiles: {
        needToUpdate = QStringLiteral("Необходимо добавить: базу данных пользователей, и пути к исполняемым файлам.\n");
        break;
    }

    case NoUserDb: {
        needToUpdate = QStringLiteral("Необходимо добавить базу данных пользователей.\n");
        break;
    }

    case NoRoleData: {
        needToUpdate = QStringLiteral("Необходимо добавить: пути к исполняемым файлам. База пользователей имеется.\n");
        break;
    }

    default: {
        qFatal("Нереализованное поведение");
    }

    }
    m_topLabel->setText(QStringLiteral("Администратор, добро пожаловать в программу управления пользователями РЛС ТИ\n")
                        + needToUpdate +
                        "Eсли у вас есть готовый файл настроек, нажмите на иконку папки справа, выберите файл и нажмите далее..."
                        "\nЕсли файл настроек правильный, то он отобразится в текстовом поле, иначе ничего не произойдет"
                        "\nЗначения файла можно будет отредактировать позже.\nЕсли файла нет, ознакомьтесь с кратким руководством и нажмите далее.."
                        );
    m_backupLabel->setText(QStringLiteral("Путь к файлам настроек:"));

    m_backupLoadButton->setIconSize(QSize(30, 30));
    m_backupLoadButton->setFlat(true);
    m_backupLoadButton->setToolTip(QStringLiteral("Выбрать файл восстановления"));
    m_backupLoadButton->setStyleSheet(QStringLiteral("border: 0px;"));

    m_backupClearButton->setIconSize(QSize(30, 30));
    m_backupClearButton->setFlat(true);
    m_backupClearButton->setToolTip(QStringLiteral("Убрать файл восстановления"));
    m_backupClearButton->setStyleSheet(QStringLiteral("border: 0px;"));

    m_backupLineEdit->clear();
    m_backupLineEdit->setFrame(false);
    m_faqLabel->setText(QStringLiteral("<html>Краткое руководство - процесс настройки состоит из шагов:<ul><li>Внесение в базу ФИО Администратора</li><li>Заполнение ролей</li></ul> Заполнение ролей состоит из шагов:<ul><li></li><li>Заполнение доступных программ для запуска на рабочем столе</li><li>Заполнение списка программ которые будут перезапущены</li></ul></html>"));
    m_backupLineEdit->setObjectName(QStringLiteral("wizard"));
}

void IntroPage::ConnectObjects()
{
    connect(m_backupLoadButton, &QPushButton::clicked, this, &IntroPage::OnCheckBackupFile);
    connect(m_backupClearButton, &QPushButton::clicked, this, &IntroPage::OnClearBackup);
    connect(m_wizardService, &WizardService::ToSendBackupParceError, this, &IntroPage::OnSendBackupParceError);
}


void IntroPage::OnCheckBackupFile()
{
    const QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    m_backupFilePath = QFileDialog::getOpenFileName(Q_NULLPTR, QStringLiteral("Выберите исполняемый файл"), strDesktop, QStringLiteral("Файл настроек (*.sync)"));

    if (!m_backupFilePath.isEmpty()) {
        if (m_wizardService->CheckAndParseBackupFile(m_backupFilePath)) {
            m_backupLineEdit->setText(m_backupFilePath);
            m_errorLabel->clear();
        } else {
            m_backupLineEdit->clear();
        }
    }
}

void IntroPage::OnClearBackup()
{
    m_backupLineEdit->clear();
    m_wizardService->ClearBackup();
}

void IntroPage::OnSendBackupParceError(const QString &error)
{
    m_errorLabel->setText(error);
}

int IntroPage::nextId() const
{
    return Page_UserData;
}

void IntroPage::SetWizardStyles(ThemesNames themeName)
{
    switch (themeName) {
    case BlackTheme:
        m_backupLoadButton->setIcon(QIcon(QStringLiteral(":/images/wizard/folderBlack.png")));
        m_backupClearButton->setIcon(QIcon(QStringLiteral(":/images/wizard/clearBackupBlack.png")));
        setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard/kbkBlack.png"));
        break;
    case AstraRedTheme:
        m_backupLoadButton->setIcon(QIcon(QStringLiteral(":/images/wizard/folderRed.png")));
        m_backupClearButton->setIcon(QIcon(QStringLiteral(":/images/wizard/clearBackupRed.png")));
        setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard/kbkRed.png"));
        break;
    case AstraBlueTheme:
        m_backupLoadButton->setIcon(QIcon(QStringLiteral(":/images/wizard/folderBlue.png")));
        m_backupClearButton->setIcon(QIcon(QStringLiteral(":/images/wizard/clearBackupBlue.png")));
        setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard/kbkBlue.png"));
        break;
    default:
        qFatal("Тема не добавлена");
        break;
    }
}
