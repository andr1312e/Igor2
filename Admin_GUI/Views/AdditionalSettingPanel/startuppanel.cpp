#include "startuppanel.h"

StartupPanel::StartupPanel(ISqlDatabaseService *sqlDatabaseService, QWidget *parent)
    : QWidget(parent)
{
    CreateModel();
    CreateServices(sqlDatabaseService);
    CreateUI();
    SetBackGroundColor();
    InsertWidgetsIntoLayout();
    ConnectObjects();
}

StartupPanel::~StartupPanel()
{
    delete m_startupRepositoryPresenter;
    delete m_appsList;

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

void StartupPanel::CreateModel()
{
    m_appsList=new QStringListModel();
}

void StartupPanel::CreateServices(ISqlDatabaseService *sqlDatabaseService)
{
    m_startupRepositoryPresenter=new StartupRepositoryPresenter(sqlDatabaseService);
}

void StartupPanel::CreateUI()
{

    m_mainLayout=new QVBoxLayout();

    m_titleLabel=new QLabel("Список процессов которые будут перезапущены:");

    m_allProgramsListView=new QListView();
    m_allProgramsListView->setModel(m_appsList);
    m_allProgramsListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_bottomLayout=new QHBoxLayout();
    m_addProgramButton=new QPushButton("Добавить программу");
    m_addProgramButton->setEnabled(false);
    m_deleteProgramButton=new QPushButton("Удалить выбранную программу");
    m_deleteProgramButton->setEnabled(false);

    m_dialog=new QtMaterialDialog(this);

    m_dialog->setParent(this);

    m_dialogWidget = new StartupDialogWidget(this);

    m_dialogLayout = new QVBoxLayout();

    m_dialogWidget->setMaximumHeight(200);


}

void StartupPanel::SetBackGroundColor()
{
    m_addProgramButton->setObjectName("add");
    m_deleteProgramButton->setObjectName("remove");

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
    if (m_selectedItemIndex>-1 && m_selectedItemIndex<m_appsList->rowCount())
    {
        m_deleteProgramButton->setDisabled(true);
        QModelIndex index = m_appsList->index(m_selectedItemIndex, 0);
        QString startupFilePath=m_appsList->data(index, Qt::DisplayRole).toString();
        m_appsList->removeRow(m_selectedItemIndex);
        m_selectedItemIndex=-1;
        m_startupRepositoryPresenter->DeleteStartup(m_roleId, startupFilePath);
        QToast* pToast=QToast::CreateToast("Программа "  + startupFilePath + " удалена",QToast::LENGTH_LONG, this);
        pToast->show();

    }
}

void StartupPanel::OnAddProgram(const QString &startupPath)
{
    QStringList startupsList=m_appsList->stringList();
    if (startupsList.contains(startupPath))
    {
        QToast* pToast=QToast::CreateToast("Программа " + startupPath + " уже в базе есть", QToast::LENGTH_LONG, this);
        pToast->show();
    }
    else
    {
        m_startupRepositoryPresenter->AppendStartup(m_roleId, startupPath);
        AppendStartupToModel(startupPath);
        QToast* pToast=QToast::CreateToast("Программа " + startupPath + " добавлена",QToast::LENGTH_LONG, this);
        pToast->show();
    }
}

void StartupPanel::OnProgramSelect(const QModelIndex &index)
{
    m_selectedItemIndex=index.row();
    m_deleteProgramButton->setEnabled(true);
}

void StartupPanel::SetRoleId(const quint8 &roleId)
{
    m_roleId=roleId;
    m_startupRepositoryPresenter->CheckStartupTable(roleId);
    m_addProgramButton->setEnabled(true);
    GetAllStartups();
}

void StartupPanel::AppendStartupToModel(const QString &startupPath)
{
    if(m_appsList->insertRow(m_appsList->rowCount()))
    {
        QModelIndex index = m_appsList->index(m_appsList->rowCount() - 1, 0);
        m_appsList->setData(index, startupPath);
    }
}

void StartupPanel::GetAllStartups()
{
    QStringList list=m_startupRepositoryPresenter->GetAllStartups(m_roleId);
    m_appsList->setStringList(list);
}
