#include "startupdialogwidget.h"

StartupDialogWidget::StartupDialogWidget(QWidget *parent)
    : QWidget(parent)
    , m_desktopPath(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation))
{
    CreateUI();
    InsertWidgetsIntoLayouts();
    InitUI();
    ConnectObjects();
}

StartupDialogWidget::~StartupDialogWidget()
{
    delete m_bottomButtonsLayout;
    delete m_execPathLayout;
    delete m_mainLayout;

    delete m_titleLabel;
    delete m_execTextField;
    delete m_execButton;
    delete m_saveDialogButton;
    delete m_closeDialogButton;
    delete m_errorMessagBox;
}

void StartupDialogWidget::CreateUI()
{
    m_mainLayout= new QVBoxLayout;

    m_titleLabel=new QLabel();


    m_execPathLayout=new QHBoxLayout();

    m_execTextField=new QtMaterialTextField();

    m_execButton=new QPushButton();

    m_bottomButtonsLayout=new QHBoxLayout();
    m_saveDialogButton = new QPushButton();

    m_closeDialogButton = new QPushButton();

    m_errorMessagBox=new QMessageBox();
}

void StartupDialogWidget::InsertWidgetsIntoLayouts()
{
    m_mainLayout->addWidget(m_titleLabel);

    m_execPathLayout->addWidget(m_execTextField);
    m_execPathLayout->addWidget(m_execButton);

    m_mainLayout->addLayout(m_execPathLayout);

    m_bottomButtonsLayout->addWidget(m_saveDialogButton);
    m_bottomButtonsLayout->addWidget(m_closeDialogButton);
    m_bottomButtonsLayout->setAlignment(Qt::AlignBottom);

    m_mainLayout->addLayout(m_bottomButtonsLayout);

    setLayout(m_mainLayout);
}

void StartupDialogWidget::InitUI()
{
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_execTextField->setLabel("Путь к исполняемому файлу: (Обязательно)");
    m_execTextField->setText("Выбрать файл");

    m_saveDialogButton->setText("Применить");
    m_closeDialogButton->setText("Выйти без сохранения");

    m_saveDialogButton->setObjectName("add");

    m_errorMessagBox->setIcon(QMessageBox::Critical);
    m_errorMessagBox->setWindowTitle("Внимание!");
}

void StartupDialogWidget::ConnectObjects()
{
    connect(m_closeDialogButton, &QPushButton::clicked, this, &StartupDialogWidget::OnHideAndClearDialog);
    connect(m_execButton, &QPushButton::clicked, this, &StartupDialogWidget::OnAddEcexPath);
    connect(m_saveDialogButton, &QPushButton::clicked, this, &StartupDialogWidget::OnCheckExec);
}

void StartupDialogWidget::CearAllTextFiels()
{
    m_execTextField->clear();
}

void StartupDialogWidget::OnHideAndClearDialog()
{
    CearAllTextFiels();
    Q_EMIT ToHideDialog();
}

void StartupDialogWidget::OnAddEcexPath()
{
    QString loadPath = QFileDialog::getOpenFileName(this, "Выберите исполняемый файл", m_desktopPath);
    m_execTextField->setText(loadPath);
}

void StartupDialogWidget::OnCheckExec()
{
    if (m_execTextField->text().isEmpty())
    {
        m_errorMessagBox->setText("Вы не ввели текст в поле \"Путь к исполняемому файлу\". Данное поле обязательно");
        m_errorMessagBox->exec();
    }
    else
    {
        if (QFile::exists(m_execTextField->text()))
        {
            Q_EMIT ToAddExecPathToFile(m_execTextField->text().simplified());
            OnHideAndClearDialog();
        }
        else
        {
            m_errorMessagBox->setText("Вы ввели путь к файлу, которого не существует, возможно у вашего пользователя недостаточно прав конфиденциальности для просмотра файла, попробуйте воспользоваться кнопкой \"Выбрать файл\" справа от поля \"Путь к исполняемому файлу\" или изменить уровень конфиденциальности пользователя");
            m_errorMessagBox->exec();
        }
    }
}
