#include "startuppanel.h"

StartupPanel::StartupPanel(ISqlDatabaseService *sqlDatabaseService, QWidget *parent)
    : QWidget(parent)
    , m_currentRoleId(0)
    , m_selectedItemIndex(-1)
{
    CreateServices(sqlDatabaseService);
    CreateUI();
    SetBackGroundColor();
    InsertWidgetsIntoLayout();
    ConnectObjects();
}

StartupPanel::~StartupPanel()
{
    delete m_startupRepositoryPresenter;

    delete m_dialogLayout;
    delete m_bottomLayout;
    delete m_mainLayout;

    delete m_dialogWidget;
    delete m_dialog;

    delete m_titleLabel;
    delete m_allProgramsListView;
    delete m_addProgramButton;
    delete m_deleteProgramButton;
}

void StartupPanel::CreateServices(ISqlDatabaseService *sqlDatabaseService)
{
    m_startupRepositoryPresenter=new StartupPanelPresenter(sqlDatabaseService);
}

void StartupPanel::CreateUI()
{
    m_mainLayout=new QVBoxLayout();

    m_titleLabel=new QLabel(QStringLiteral("Список процессов которые будут перезапущены:"));

    m_allProgramsListView=new QListView();
    m_allProgramsListView->setModel(m_startupRepositoryPresenter->GetRoleStartupsModel());
    m_allProgramsListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_bottomLayout=new QHBoxLayout();
    m_addProgramButton=new QPushButton(QStringLiteral("Добавить программу"));
    m_addProgramButton->setEnabled(false);
    m_deleteProgramButton=new QPushButton(QStringLiteral("Удалить выбранную программу"));
    m_deleteProgramButton->setEnabled(false);

    m_dialog=new QtMaterialDialog(this);

    m_dialog->setParent(this);

    m_dialogWidget = new StartupDialogWidget(this);

    m_dialogLayout = new QVBoxLayout();

    m_dialogWidget->setMaximumHeight(200);
}

void StartupPanel::SetBackGroundColor()
{
    m_addProgramButton->setObjectName(QStringLiteral("add"));
    m_deleteProgramButton->setObjectName(QStringLiteral("remove"));

    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);

    m_dialogWidget->setBackgroundRole(QPalette::AlternateBase);
    m_dialogWidget->setAutoFillBackground(true);
}

void StartupPanel::InsertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_allProgramsListView);

    m_bottomLayout->addWidget(m_addProgramButton);
    m_bottomLayout->addWidget(m_deleteProgramButton);

    m_mainLayout->addLayout(m_bottomLayout);

    setLayout(m_mainLayout);

    m_dialog->setWindowLayout(m_dialogLayout);
    m_dialogLayout->addWidget(m_dialogWidget);
}

void StartupPanel::ConnectObjects()
{
    connect(m_allProgramsListView, &QListView::clicked, this, &StartupPanel::OnProgramSelect);
    connect(m_addProgramButton, &QPushButton::clicked, m_dialog, &QtMaterialDialog::OnShowDialog);
    connect(m_deleteProgramButton, &QPushButton::clicked, this, &StartupPanel::OnDeleteProgram);
    connect(m_dialogWidget, &StartupDialogWidget::ToHideDialog, m_dialog, &QtMaterialDialog::OnHideDialog);
    connect(m_dialogWidget, &StartupDialogWidget::ToAddExecPathToFile, this, &StartupPanel::OnAddProgram);
}

void StartupPanel::OnDeleteProgram()
{
    if (m_selectedItemIndex>-1 && m_selectedItemIndex<m_startupRepositoryPresenter->GetMaxStartupCount())
    {
        if(Log4Qt::Logger::rootLogger()->HasAppenders())
        {
            Log4Qt::Logger::rootLogger()->info(Q_FUNC_INFO + QStringLiteral(" Хотим удалить программу роль :") + QString::number(m_currentRoleId) + QStringLiteral(" индекс :") + QString::number(m_selectedItemIndex) );
        }
        m_deleteProgramButton->setDisabled(true);
        const QString currentStartupNameToDelete=m_startupRepositoryPresenter->DeleteStartup(m_currentRoleId, m_selectedItemIndex);
        QToast*  const pToast=QToast::CreateToast(QStringLiteral("Программа ")  + currentStartupNameToDelete + QStringLiteral(" удалена"),QToast::LENGTH_LONG, this);
        pToast->show();
        m_startupRepositoryPresenter->GetAllStartupsIntoModel(m_currentRoleId);
    }
}

void StartupPanel::OnAddProgram(const QString &startupPath)
{
    const int indexOfName=startupPath.lastIndexOf('/');
    const QString startupName=startupPath.mid(indexOfName+1);
    if (m_startupRepositoryPresenter->HasDuplicateStartup(startupName))
    {
        QToast* const pToast=QToast::CreateToast(QStringLiteral("Программа ") + startupName + QStringLiteral(" уже в базе есть. Если это другая, удалите старую"), QToast::LENGTH_LONG, this);
        pToast->show();
    }
    else
    {
        m_startupRepositoryPresenter->AppendStartup(m_currentRoleId, startupPath);
        QToast* const pToast=QToast::CreateToast(QStringLiteral("Программа ") + startupName + QStringLiteral(" добавлена"),QToast::LENGTH_LONG, this);
        pToast->show();
        m_startupRepositoryPresenter->GetAllStartupsIntoModel(m_currentRoleId);
    }
}

void StartupPanel::OnProgramSelect(const QModelIndex &index)
{
    m_selectedItemIndex=index.row();
    m_deleteProgramButton->setEnabled(true);
}

void StartupPanel::SetRoleId(const int &roleId)
{
    m_currentRoleId=roleId;
    m_startupRepositoryPresenter->CheckStartupTable(roleId);
    m_addProgramButton->setEnabled(true);
    GetAllStartupsIntoModel();
}

void StartupPanel::GetAllStartupsIntoModel()
{
    m_startupRepositoryPresenter->GetAllStartupsIntoModel(m_currentRoleId);
}
