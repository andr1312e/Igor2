#include "intropage.h"
#include <QDebug>
#include <QMessageBox>


IntroPage::IntroPage(LoadingStates state, WizardService *service, ThemesNames themeName, QWidget *parent)
    : MyWizardPage(parent)
    , m_wizardService(service)
{
    CreateUI();
    SetWizardStyles(themeName);
    InsertWidgetsIntoLayout();
    FillUI(state);
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
    delete m_errorLabel;
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
    m_errorLabel = new QLabel();
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

void IntroPage::FillUI(LoadingStates state)
{
    QString needToUpdate;
    switch (state)
    {
    case LoadingState::NoFiles:
    {
        needToUpdate = QStringLiteral("Необходимо добавить: базу данных пользователей, и пути к исполняемым файлам.\n");
        break;
    }

    case LoadingState::NoUserDb:
    {
        needToUpdate = QStringLiteral("Необходимо добавить базу данных пользователей.\n");
        break;
    }

    case LoadingState::NoRoleData:
    {
        needToUpdate = QStringLiteral("Необходимо добавить: пути к исполняемым файлам. База пользователей имеется.\n");
        break;
    }
    case LoadingState::NoFiles   | LoadingState::ForceReset:
    case LoadingState::NoRoleData| LoadingState::ForceReset:
    case LoadingState::NoUserDb  | LoadingState::ForceReset:
    case LoadingState::Fine  | LoadingState::ForceReset:
    {
        needToUpdate = QStringLiteral("ВКЛЮЧЕН режим принудительного восстановления.\n");
        break;
    }
    default:
    {
        qFatal("Нереализованное поведение");
    }

    }
    m_topLabel->setText(QStringLiteral("Администратор, добро пожаловать в программу управления пользователями РЛС ТИ\n")
                        + needToUpdate +
                        QStringLiteral("Eсли у вас есть готовый файл настроек, нажмите на иконку папки справа"
                                       "\nВыберите файл и нажмите далее..."
                                       "\nЕсли файл корректный, он отобразится в текстовом поле, иначе ничего не произойдет"
                                       "\nЗначения файла можно будет отредактировать позже"
                                       "\nЕсли файла нет, ознакомьтесь с кратким руководством и нажмите далее..")
                       );
    m_backupLabel->setText(QStringLiteral("Путь к файлам настроек:"));

    m_backupLoadButton->setIconSize(QSize(30, 30));
    m_backupLoadButton->setFlat(true);
    m_backupLoadButton->setToolTip(QStringLiteral("Выбрать файл восстановления"));
    m_backupLoadButton->setStyleSheet(QStringLiteral("border: 0px;"));

    m_backupClearButton->setIconSize(QSize(30, 30));
    m_backupClearButton->setFlat(true);
    m_backupClearButton->setToolTip(QStringLiteral("Убрать файл восстановления"));
    m_backupClearButton->setStyleSheet(QLatin1Literal("border: 0px;"));

    m_backupLineEdit->clear();
    m_backupLineEdit->setFrame(false);
    m_faqLabel->setText(QStringLiteral("<html>Краткое руководство - процесс настройки состоит из шагов:<ul><li>Внесение в базу ФИО Администратора</li><li>Заполнение ролей</li></ul> Заполнение ролей состоит из шагов:<ul><li></li><li>Заполнение доступных программ для запуска на рабочем столе</li><li>Заполнение списка программ которые будут перезапущены</li></ul></html>"));
    m_backupLineEdit->setObjectName(QLatin1Literal("wizard"));
    m_backupLineEdit->setReadOnly(true);
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

    if (!m_backupFilePath.isEmpty())
    {
        if (m_wizardService->CheckAndParseBackupFile(m_backupFilePath))
        {
            m_backupLineEdit->setText(m_backupFilePath);
            const QString kitVersion = m_wizardService->GetBackupVersion();
            m_errorLabel->setText(QStringLiteral("Версия комплекта: ") + kitVersion + QStringLiteral(" Дата редактирования: ") + QFileInfo(m_backupFilePath).lastModified().toString(QStringLiteral("dddd, d MMMM yyyy г. hh:mm:ss")));
        }
        else
        {
            m_backupLineEdit->clear();
        }
    }
}

void IntroPage::OnClearBackup()
{
    m_backupLineEdit->clear();
    m_errorLabel->clear();
    m_wizardService->ClearBackup();
}

void IntroPage::OnSendBackupParceError(const QString &error)
{
    m_errorLabel->setText(error);
}

int IntroPage::nextId() const
{
    return static_cast<int>(WizardPage::Page_UserData);
}

void IntroPage::SetWizardStyles(ThemesNames themeName)
{
    switch (themeName)
    {
    case ThemesNames::BlackTheme:
        m_backupLoadButton->setIcon(QIcon(QLatin1Literal(":/images/wizard/folderBlack.png")));
        m_backupClearButton->setIcon(QIcon(QLatin1Literal(":/images/wizard/clearBackupBlack.png")));
        setPixmap(QWizard::WatermarkPixmap, QPixmap(QLatin1Literal(":/images/wizard/kbkBlack.png")));
        break;
    case ThemesNames::AstraRedTheme:
        m_backupLoadButton->setIcon(QIcon(QLatin1Literal(":/images/wizard/folderRed.png")));
        m_backupClearButton->setIcon(QIcon(QLatin1Literal(":/images/wizard/clearBackupRed.png")));
        setPixmap(QWizard::WatermarkPixmap, QPixmap(QLatin1Literal(":/images/wizard/kbkRed.png")));
        break;
    case ThemesNames::AstraBlueTheme:
        m_backupLoadButton->setIcon(QIcon(QLatin1Literal(":/images/wizard/folderBlue.png")));
        m_backupClearButton->setIcon(QIcon(QLatin1Literal(":/images/wizard/clearBackupBlue.png")));
        setPixmap(QWizard::WatermarkPixmap, QPixmap(QLatin1Literal(":/images/wizard/kbkBlue.png")));
        break;
    default:
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Нереализованное поведение")).toUtf8().constData());
        break;
    }
}
