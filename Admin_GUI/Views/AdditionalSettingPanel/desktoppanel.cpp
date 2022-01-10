#include "desktoppanel.h"

DesktopPanel::DesktopPanel(ICONS_PANEL_TYPE type, Terminal *terminal, ISqlDatabaseService *sqlDatabaseService, QWidget *parent)
    : QWidget(parent)
    , m_type(type)
{
    CreatePresenter(terminal, sqlDatabaseService);
    SetPresenterModelToView();
    CreateUI();
    SetBackgroundColor();
    InsertWidgetsIntoLayout();
    ConnectObjects();
}

DesktopPanel::~DesktopPanel()
{
    delete m_desktopPanelPresenter;
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

void DesktopPanel::CreatePresenter(Terminal *terminal, ISqlDatabaseService *sqlDatabaseService)
{
    m_desktopPanelPresenter=new IconMakingService(terminal, sqlDatabaseService);
}

void DesktopPanel::SetPresenterModelToView()
{
    m_model=m_desktopPanelPresenter->GetModel();
}

void DesktopPanel::CreateUI()
{

    m_mainLayout=new QVBoxLayout();

    m_programsToRun=new QLabel();

    m_fileDelegate=new FileDelegate(this);
    m_allProgramsListView=new QListView(this);
    m_allProgramsListView->setModel(m_model);
    m_allProgramsListView->setItemDelegate(m_fileDelegate);
    m_allProgramsListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_bottomLayout=new QHBoxLayout();

    m_addProgramButton=new QPushButton("Добавить программу");
    m_deleteProgramButton=new QPushButton("Удалить выбранную программу");

    m_addProgramButton->setDisabled(true);
    m_deleteProgramButton->setDisabled(true);

    m_dialogLayout = new QVBoxLayout();
    m_dialog=new QtMaterialDialog(this);

    m_dialogWidget = new DesktopUploadDialogWidget(this);
    setMinimumHeight(255);
    setMinimumWidth(420);
}

void DesktopPanel::SetBackgroundColor()
{
    m_addProgramButton->setObjectName("add");
    m_deleteProgramButton->setObjectName("remove");

    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);

    m_dialogWidget->setBackgroundRole(QPalette::AlternateBase);
    m_dialogWidget->setAutoFillBackground(true);
}


void DesktopPanel::InsertWidgetsIntoLayout()
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

void DesktopPanel::ConnectObjects()
{
    connect(m_allProgramsListView, &QListView::clicked, this, &DesktopPanel::OnProgramSelect);
    connect(m_addProgramButton, &QPushButton::clicked, m_dialog, &QtMaterialDialog::OnShowDialog);
    connect(m_deleteProgramButton, &QPushButton::clicked, this, &DesktopPanel::OnDeleteProgram);
    connect(m_dialogWidget, &DesktopUploadDialogWidget::ToDialogSignalHide, m_dialog, &QtMaterialDialog::OnHideDialog);
    connect(m_dialogWidget, &DesktopUploadDialogWidget::ToAddFileToUserDesktop, this, &DesktopPanel::OnAddProgram);
}

void DesktopPanel::OnSetDefaultRoleApps(const quint8 &role)
{
    if (m_type==ICONS_PANEL_TYPE::USER_DESKTOP)
    {
        m_desktopPanelPresenter->SetDefaultIconsToUser(m_roleId, role, m_userName);
        m_roleId=role;
    }
    else
    {
        qFatal("DesktopPanel::setDefaultApps невозможно установить для роли иконки по умолчанию");
    }
}

void DesktopPanel::OnRoleDesktopChanges(const quint8 &roleId)
{
    if (m_roleId==roleId)
    {
        update();
    }
}


void DesktopPanel::UpdateAllUsersWithCurrentRole()
{
    if (m_type==ICONS_PANEL_TYPE::ROLE_DESKTOP)
    {
        for (const QString &user:*m_usersList)
        {
            m_desktopPanelPresenter->SetDefaultIconsToUser(m_roleId, 1, user);
        }
    }
    else
    {
        qFatal("Can't do it");
    }
}

void DesktopPanel::OnAddProgram(const QString &exec, const QString &iconPath, const QString &iconName)
{
    DesktopEntity entity;
    entity.exec=exec;
    entity.icon=iconPath;
    entity.name=iconName;
    if(m_type==ICONS_PANEL_TYPE::ROLE_DESKTOP)
    {
        m_desktopPanelPresenter->AddIconToRole(entity, m_roleId);
//        UpdateAllUsersWithCurrentRole();
    }
    else
    {
        m_desktopPanelPresenter->AddIconToUser(entity, m_userName);
    }
}

void DesktopPanel::OnDeleteProgram()
{
    if(m_type==ICONS_PANEL_TYPE::ROLE_DESKTOP)
    {
        m_desktopPanelPresenter->DeleteIconToRole(m_selectedItemName, m_roleId);
        UpdateAllUsersWithCurrentRole();
    }
    else
    {
        m_desktopPanelPresenter->DeleteIconToUser(m_userName, m_selectedItemName);
    }
    m_deleteProgramButton->setDisabled(true);
}

void DesktopPanel::OnProgramSelect(const QModelIndex &index)
{
    QVariant indexData=index.data(Qt::UserRole+1);
    DesktopEntity entity=indexData.value<DesktopEntity>();
    m_selectedItemName=entity.name;
    m_deleteProgramButton->setText("Удалить с рабочего стола");
    m_deleteProgramButton->setEnabled(true);
}

void DesktopPanel::SetUser(const User &user)
{
    if(m_type==ICONS_PANEL_TYPE::ROLE_DESKTOP)
    {
        qFatal("Irregular type");
    }
    else
    {
        m_userName=user.name;
        m_programsToRun->setText("Ярлыки на рабочий стол для пользователя: "+ m_userName);
        m_addProgramButton->setEnabled(true);
        m_deleteProgramButton->setDisabled(true);
        m_desktopPanelPresenter->GetAllUserDesktops(m_userName);
    }
}

void DesktopPanel::SetRoleId(const int &roleId)
{
    if(m_type==ICONS_PANEL_TYPE::USER_DESKTOP)
    {
        qFatal("Irregular type");
    }
    else
    {
        m_roleId=roleId;
        m_programsToRun->setText("Ярлыки на рабочий стол для роли: "+ Roles.at(roleId));
        m_addProgramButton->setEnabled(true);
        m_deleteProgramButton->setDisabled(true);
        m_desktopPanelPresenter->GetAllRoleDesktops(roleId);
    }
}

void DesktopPanel::DeleteUserAllRoleIcons()
{
    if(m_roleId>0 && m_roleId<Roles.count())
    {
        if(!m_userName.isEmpty())
        {
            m_desktopPanelPresenter->DeleteRoleIconFromUser(m_userName, m_roleId);
        }
    }
}
