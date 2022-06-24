#include "filedialogwidget.h"

DesktopUploadDialogWidget::DesktopUploadDialogWidget(QWidget *parent)
    : QWidget(parent)
    , m_desktopPath(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))
{
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObject();
}

DesktopUploadDialogWidget::~DesktopUploadDialogWidget()
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


void DesktopUploadDialogWidget::CreateUI()
{
    m_mainLayout = new QVBoxLayout;
    m_titleLabel = new QLabel();
    m_execPathLayout = new QHBoxLayout();
    m_exec = new QtMaterialTextField();
    m_execButton = new QPushButton();
    m_iconPathLayout = new QHBoxLayout();
    m_iconPath = new QtMaterialTextField();
    m_iconPathButton = new QPushButton();
    m_iconName = new QtMaterialTextField();
    m_dialogWidgetButtonsLayout = new QHBoxLayout();
    m_saveDialogButton = new QPushButton();
    m_closeDialogButton = new QPushButton();
    m_messagBox = new QMessageBox();
}

void DesktopUploadDialogWidget::InsertWidgetsIntoLayout()
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

void DesktopUploadDialogWidget::FillUI()
{
    m_iconPath->setLabel(QStringLiteral("Путь к иконке: (необязательно)"));
    m_iconPathButton->setText(QStringLiteral("Выбрать файл"));
    m_iconName->setLabel(QStringLiteral("Название на рабочем столе: (Обязательно)"));
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_execButton->setText(QStringLiteral("Выбрать файл"));
    m_exec->setLabel(QStringLiteral("Путь к файлу: (Обязательно)"));
    m_saveDialogButton->setText(QStringLiteral("Применить"));
    m_closeDialogButton->setText(QStringLiteral("Выйти без сохранения"));
    m_messagBox->setWindowTitle(QStringLiteral("Внимание!"));
}

void DesktopUploadDialogWidget::ConnectObject()
{
    connect(m_closeDialogButton, &QPushButton::clicked, this, &DesktopUploadDialogWidget::OnHideDialog);
    connect(m_execButton, &QPushButton::clicked, this, &DesktopUploadDialogWidget::OnAddingEcexutePath);
    connect(m_iconPathButton, &QPushButton::clicked, this, &DesktopUploadDialogWidget::OnAddIconPath);
    connect(m_saveDialogButton, &QPushButton::clicked, this, &DesktopUploadDialogWidget::OnAddIconToUserDesktop);
}

void DesktopUploadDialogWidget::SetTitleText(const QString &text)
{
    m_titleLabel->setText(text);
}


void DesktopUploadDialogWidget::ClearAllTextFiels()
{
    m_exec->clear();
    m_iconPath->clear();
    m_iconName->clear();
}

void DesktopUploadDialogWidget::OnHideDialog()
{
    ClearAllTextFiels();
    Q_EMIT ToDialogSignalHide();
}

void DesktopUploadDialogWidget::OnAddingEcexutePath()
{
    const QString loadPath = QFileDialog::getOpenFileName(this, QStringLiteral("Выберите исполняемый файл"), m_desktopPath);
    m_exec->setText(loadPath);
}

void DesktopUploadDialogWidget::OnAddIconPath()
{
    const QString loadPath = QFileDialog::getOpenFileName(this, QStringLiteral("Выберите исполняемый файл"), m_desktopPath);
    m_iconPath->setText(loadPath);
}

void DesktopUploadDialogWidget::OnAddIconToUserDesktop()
{
    if (m_exec->text().isEmpty())
    {
        m_messagBox->setText(QStringLiteral("Вы не ввели текст в поле \"Путь к исполняемому файлу\". Данное поле обязательно"));
        m_messagBox->exec();
    }
    else
    {
        if (m_iconName->text().isEmpty())
        {
            m_messagBox->setText(QStringLiteral("Вы не ввели текст в поле \"Название на рабочем столе\". Данное поле обязательно"));
            m_messagBox->exec();
        }
        else
        {
            if (FunctionsWithStrings::StringContainsBadCharecters(m_iconName->text()))
            {
                m_messagBox->setText(QStringLiteral("Текст в поле \"Название на рабочем столе\" не может содержать символы \\ / ` : & * ? \" \' < > | () : ;"));
                m_messagBox->exec();
            }
            else
            {
                if (QFile::exists(m_exec->text()))
                {
                    if (Log4Qt::Logger::rootLogger()->HasAppenders())
                    {
                        Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Добавили ярлыка файл: ") + m_exec->text() + QStringLiteral(" имя файла: ") + m_iconName->text() + QStringLiteral(" иконка файла: ") + m_iconPath->text());
                    }
                    Q_EMIT ToAddFileToUserDesktop(m_exec->text(), m_iconPath->text(), m_iconName->text());
                    OnHideDialog();
                }
                else
                {
                    m_messagBox->setText(QStringLiteral("Вы ввели путь к файлу, которого не существует, попробуйте воспользоваться кнопкой \"Выбрать файл\" справа от поля \"Путь к исполняемому файлу\""));
                    m_messagBox->exec();
                }
            }
        }
    }
}
