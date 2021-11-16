#include "startupdialogwidget.h"

StartupDialogWidget::StartupDialogWidget(QWidget *parent)
    : QWidget(parent)
{
    CreateUI();
    InsertWidgetsIntoLayouts();
    ConnectObjects();
}

StartupDialogWidget::~StartupDialogWidget()
{
    delete m_bottomButtonsLayout;
    delete m_execPathLayout;
    delete m_mainLayout;

    delete m_titleLabel;
    delete m_exec;
    delete m_execButton;
    delete m_saveDialogButton;
    delete m_closeDialogButton;
    delete m_errorMessagBox;
}

void StartupDialogWidget::setTitleLabel(QString &userName)
{
    m_titleLabel->setText("Добавить контроль над закрытием программы для пользователя: " + userName);
}

void StartupDialogWidget::CreateUI()
{
    m_mainLayout= new QVBoxLayout;

    m_titleLabel=new QLabel();
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_execPathLayout=new QHBoxLayout();

    m_exec=new QtMaterialTextField();
    m_exec->setLabel("Путь к исполняемому файлу: (Обязательно)");

    m_execButton=new QPushButton("Выбрать файл");

    m_bottomButtonsLayout=new QHBoxLayout();
    m_saveDialogButton = new QPushButton("Применить");
    m_saveDialogButton->setObjectName("add");
    m_closeDialogButton = new QPushButton("Выйти без сохранения");

    m_errorMessagBox=new QMessageBox();
    m_errorMessagBox->setIcon(QMessageBox::Critical);
    m_errorMessagBox->setWindowTitle("Внимание!");
}

void StartupDialogWidget::InsertWidgetsIntoLayouts()
{
    m_mainLayout->addWidget(m_titleLabel);

    m_execPathLayout->addWidget(m_exec);
    m_execPathLayout->addWidget(m_execButton);

    m_mainLayout->addLayout(m_execPathLayout);

    m_bottomButtonsLayout->addWidget(m_saveDialogButton);
    m_bottomButtonsLayout->addWidget(m_closeDialogButton);
    m_bottomButtonsLayout->setAlignment(Qt::AlignBottom);

    m_mainLayout->addLayout(m_bottomButtonsLayout);

    setLayout(m_mainLayout);
}

void StartupDialogWidget::ConnectObjects()
{
    connect(m_closeDialogButton, &QPushButton::clicked, this, &StartupDialogWidget::OnHideAndClearDialog);
    connect(m_execButton, &QPushButton::clicked, this, &StartupDialogWidget::OnAddEcexPath);
    connect(m_saveDialogButton, &QPushButton::clicked, this, &StartupDialogWidget::OnCheckExec);

}

void StartupDialogWidget::CearAllTextFiels()
{
    m_exec->clear();
}

void StartupDialogWidget::OnHideAndClearDialog()
{
    CearAllTextFiels();
    emit ToHideDialog();
}

void StartupDialogWidget::OnAddEcexPath()
{
    QString strDesktop = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString loadPath = QFileDialog::getOpenFileName(nullptr, "Выберите исполняемый файл", strDesktop);
    m_exec->setText(loadPath);
}

void StartupDialogWidget::OnCheckExec()
{
    if (m_exec->text()=="")
    {
        m_errorMessagBox->setText("Вы не ввели текст в поле \"Путь к исполняемому файлу\". Данное поле обязательно");
        m_errorMessagBox->exec();
    }
    else
    {
        if (QFile::exists(m_exec->text()))
        {
            emit ToAddExecPathToFile(m_exec->text());
            OnHideAndClearDialog();
        }
        else
        {
            m_errorMessagBox->setText("Вы ввели путь к файлу, которого не существует, возможно у вашего пользователя недостаточно прав конфиденциальности для просмотра файла, попробуйте воспользоваться кнопкой \"Выбрать файл\" справа от поля \"Путь к исполняемому файлу\" или изменить уровень конфиденциальности пользователя");
            m_errorMessagBox->exec();
        }
    }
}
