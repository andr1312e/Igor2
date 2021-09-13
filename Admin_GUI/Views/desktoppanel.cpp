#include "desktoppanel.h"

DesktopPanel::DesktopPanel(Terminal *terminal, ICONS_PANEL_TYPE type, QWidget *parent)
    : QWidget(parent)
    , m_type(type)
{
    initServices(terminal);
    initModel();
    initUI();
    applyStyle();
    insertWidgetsIntoLayout();
    createConnections();
}

DesktopPanel::~DesktopPanel()
{
    delete m_rootFileService;
    delete m_fileDelegate;

    delete m_bottomLayout;
    delete m_dialogLayout;
    delete m_mainLayout;

    delete m_programsToRun;
    delete m_allProgramsListView;
    delete m_addProgramButton;
    delete m_deleteProgramButton;

    delete m_dialogWidget;
    delete m_dialog;
}

void DesktopPanel::setParam(const QString &param, QStringList *users)
{
    QString rootFolder="/home/";
    QString titleText="Добавить ярлык";
    if (m_type==ICONS_PANEL_TYPE::USER_DESKTOP)
    {
        rootFolder=rootFolder+param+"/Desktop/";
        titleText=titleText+" на рабочий стол для пользователя " + param;
    }
    else
    {
        rootFolder=rootFolder+"user/RLS_TI/"+param+"/";
        titleText=titleText+" на рабочий стол для пользователей с ролью: " + param;
    }
    m_pararm=param;
    m_usersList=users;
    m_dialogWidget->setTitleText(titleText);
    m_rootFileService->setPath(rootFolder);
    m_addProgramButton->setEnabled(true);
    m_deleteProgramButton->setDisabled(true);
}

void DesktopPanel::setDefaultRoleApps(const QString &role)
{
    if (m_type==ICONS_PANEL_TYPE::USER_DESKTOP)
    {
        m_rootFileService->setDefaultIcons(role);
    }
    else
    {
        qFatal("DesktopPanel::setDefaultApps невозможно установить для роли иконки по умолчанию");
    }
}

void DesktopPanel::initServices(Terminal *terminal)
{
    m_rootFileService=new FileExplorer(terminal);
}

void DesktopPanel::initUI()
{

    m_mainLayout=new QVBoxLayout();

    m_programsToRun=new QLabel("Ярлыки на рабочем столе:");

    m_fileDelegate=new FileDelegate(this);

    m_allProgramsListView=new QListView(this);
    m_allProgramsListView->setModel(m_model);
    m_allProgramsListView->setItemDelegate(m_fileDelegate);

    m_bottomLayout=new QHBoxLayout();

    m_addProgramButton=new QPushButton("Добавить программу");
    m_deleteProgramButton=new QPushButton("Удалить выбранную программу");

    m_addProgramButton->setDisabled(true);
    m_deleteProgramButton->setDisabled(true);

    m_dialogLayout = new QVBoxLayout();
    m_dialog=new QtMaterialDialog(this);

    m_dialogWidget = new FileDialogWidget(this);
    setMinimumHeight(255);
    setMinimumWidth(420);
}

void DesktopPanel::applyStyle()
{
    m_addProgramButton->setObjectName("add");
    m_deleteProgramButton->setObjectName("remove");
}

void DesktopPanel::initModel()
{
    m_model=m_rootFileService->getModel();
}

void DesktopPanel::insertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_programsToRun);
    m_mainLayout->addWidget(m_allProgramsListView);

    m_bottomLayout->addWidget(m_addProgramButton);
    m_bottomLayout->addWidget(m_deleteProgramButton);

    m_mainLayout->addLayout(m_bottomLayout);

    m_dialog->setWindowLayout(m_dialogLayout);
    m_dialogLayout->addWidget(m_dialogWidget);

    setLayout(m_mainLayout);
}

void DesktopPanel::createConnections()
{
    connect(m_allProgramsListView, &QListView::clicked, this, &DesktopPanel::onProgramSelect);
    connect(m_addProgramButton, &QPushButton::clicked, m_dialog, &QtMaterialDialog::showDialog);
    connect(m_deleteProgramButton, &QPushButton::clicked, this, &DesktopPanel::deleteProgram);
    connect(m_dialogWidget, &FileDialogWidget::hideDialogSignal, m_dialog, &QtMaterialDialog::hideDialog);
    connect(m_dialogWidget, &FileDialogWidget::addFileToUserDesktop, this, &DesktopPanel::addProgram);
}

void DesktopPanel::updateAllUsersWithCurrentRole()
{
    if (m_type==ICONS_PANEL_TYPE::ROLE_DESKTOP)
    {
        for (QStringList::ConstIterator it=m_usersList->begin(); it!=m_usersList->end(); ++it)
        {
            m_rootFileService->setDefaultIconsToUser(m_pararm, *it);
        }
    }
}

void DesktopPanel::addProgram(const QString &exec, const QString &iconPath, const QString &iconName)
{
    m_rootFileService->addIcon(exec, iconPath, iconName);
    updateAllUsersWithCurrentRole();
}

void DesktopPanel::deleteProgram()
{
    m_rootFileService->deleteIcon(m_selectedItemName);
    m_deleteProgramButton->setDisabled(true);
    updateAllUsersWithCurrentRole();
}

void DesktopPanel::onProgramSelect(const QModelIndex &index)
{
    QVariant indexData=index.data(Qt::UserRole+1);
    DesktopEntity entity=indexData.value<DesktopEntity>();
    m_selectedItemName=entity.name;
    m_deleteProgramButton->setText("Удалить ярлык с рабочего стола");
    m_deleteProgramButton->setEnabled(true);
}
