#include "startuppanel.h"

StartupPanel::StartupPanel(Terminal *terminal,const STARTUP_PANEL_TYPE type, QWidget *parent)
    : QWidget(parent)
    , m_type(type)
{
    initModel();
    initServices(terminal);
    initUI();
    applyStyle();
    insertWidgetsIntoLayout();
    createConnections();
}

StartupPanel::~StartupPanel()
{
    delete m_startupRepositoryService;
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

void StartupPanel::setParam(const QString &param, QStringList *users)
{
    QString m_startupFolder;
    if (m_type==STARTUP_PANEL_TYPE::USER_APPS)
    {
        m_startupFolder="/home/"+param+"/RLS_TI/";
        m_startupFilePath=m_startupFolder+"Startup";
    }
    else
    {

        m_startupFolder="/home/user/RLS_TI/";
        m_startupFilePath=m_startupFolder+param+".start";
    }
    m_usersList=users;
    m_startupRepositoryService->checkStartupFile(m_startupFolder, m_startupFilePath);
    m_addProgramButton->setEnabled(true);
    updateModel();
}

void StartupPanel::setDefaultRoleApps(const QString &role)
{
    m_startupRepositoryService->setDefaultApps(role, m_startupFilePath);
    updateModel();
}

void StartupPanel::initServices(Terminal *terminal)
{
    m_startupRepositoryService=new StartupRepositoryService(terminal);
}

void StartupPanel::applyStyle()
{
    m_addProgramButton->setObjectName("add");
    m_deleteProgramButton->setObjectName("remove");
}

void StartupPanel::initUI()
{

    m_mainLayout=new QVBoxLayout();

    m_titleLabel=new QLabel("Список процессов которые будут перезапущены:");

    m_allProgramsListView=new QListView();
    m_allProgramsListView->setModel(m_appsList);

    m_bottomLayout=new QHBoxLayout();
    m_addProgramButton=new QPushButton("Добавить программу");
    m_addProgramButton->setEnabled(false);
    m_deleteProgramButton=new QPushButton("Удалить выбранную программу");
    m_deleteProgramButton->setEnabled(false);

    m_dialog=new QtMaterialDialog();

    m_dialog->setParent(this);

    m_dialogWidget = new StartupDialogWidget(this);

    m_dialogLayout = new QVBoxLayout();

    m_dialogWidget->setMaximumHeight(200);

}

void StartupPanel::initModel()
{
    m_appsList=new QStringListModel();
}

void StartupPanel::insertWidgetsIntoLayout()
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

void StartupPanel::createConnections()
{
    connect(m_allProgramsListView, &QListView::clicked, this, &StartupPanel::onProgramSelect);
    connect(m_addProgramButton, &QPushButton::clicked, m_dialog, &QtMaterialDialog::showDialog);
    connect(m_deleteProgramButton, &QPushButton::clicked, this, &StartupPanel::deleteProgram);
    connect(m_dialogWidget, &StartupDialogWidget::hideDialog, m_dialog, &QtMaterialDialog::hideDialog);
    connect(m_dialogWidget, &StartupDialogWidget::addExecPathToFile, this, &StartupPanel::addProgram);
}

void StartupPanel::deleteProgram()
{
    if (m_selectedItemIndex>-1 && m_selectedItemIndex<m_appsList->rowCount())
    {
        m_deleteProgramButton->setDisabled(true);
        m_appsList->removeRow(m_selectedItemIndex);
        QStringList appsList=m_appsList->stringList();
        m_startupRepositoryService->clearFile(m_startupFilePath);
        m_startupRepositoryService->writeExecToStartupFile(m_startupFilePath, appsList);
        updateAllUsersWithCurrentRole(appsList);
        m_selectedItemIndex=-1;
        QToast* pToast=QToast::CreateToast("Программа удалена",QToast::LENGTH_LONG, this);
        pToast->show();
    }
}

void StartupPanel::addProgram(const QString &exec)
{
    QStringList appsList=m_appsList->stringList();
    if (!appsList.contains(exec))
    {
        appsList.append(exec);
        m_startupRepositoryService->writeExecToStartupFile(m_startupFilePath, appsList);
        m_appsList->setStringList(appsList);
        updateAllUsersWithCurrentRole(appsList);
        QToast* pToast=QToast::CreateToast("Программа " + exec + " добавлена",QToast::LENGTH_LONG, this);
        pToast->show();
    }
    else
    {
        QToast* pToast=QToast::CreateToast("Программа " + exec + " уже в базе есть",QToast::LENGTH_LONG, this);
        pToast->show();
    }
}

void StartupPanel::onProgramSelect(const QModelIndex &index)
{
    m_selectedItemIndex=index.row();
    m_deleteProgramButton->setEnabled(true);
}

void StartupPanel::updateAllUsersWithCurrentRole(QStringList &appsList)
{
    if (m_type==STARTUP_PANEL_TYPE::ROLE_APPS)
    {
        writeAppListToAllUsersWithRole(appsList);
    }
}

void StartupPanel::updateModel()
{
    QStringList list=m_startupRepositoryService->getAllEcexFromStartupFile(m_startupFilePath);
    m_appsList->setStringList(list);
}

void StartupPanel::writeAppListToAllUsersWithRole(QStringList &appsList)
{
    for (QStringList::ConstIterator it=m_usersList->begin(); it!=m_usersList->end(); ++it)
    {
        QString startupFolder="/home/"+*it+"/RLS_TI/";
        QString startupFilePath=startupFolder+"Startup";
        m_startupRepositoryService->checkStartupFile(startupFolder, startupFilePath);
        m_startupRepositoryService->clearFile(startupFilePath);
        m_startupRepositoryService->writeExecToStartupFile(startupFilePath, appsList);
    }
}
