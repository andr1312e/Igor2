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
    delete m_iconMakingService;
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
    m_iconMakingService=new IconMakingService(terminal, sqlDatabaseService);
}

void DesktopPanel::SetPresenterModelToView()
{
    m_model=m_iconMakingService->GetModel();
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

    m_addProgramButton=new QPushButton(QStringLiteral("Добавить программу"));
    m_deleteProgramButton=new QPushButton(QStringLiteral("Удалить выбранную программу"));

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
    m_addProgramButton->setObjectName(QStringLiteral("add"));
    m_deleteProgramButton->setObjectName(QStringLiteral("remove"));

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
    if (m_type==ICONS_PANEL_TYPE::USER_ICONS)
    {
        m_iconMakingService->SetDefaultIconsToUser(m_roleId, role, m_userName);
        m_roleId=role;
    }
    else
    {
        qDebug()<<QStringLiteral("невозможно установить для роли иконки по умолчанию").toUtf8();
        qFatal(Q_FUNC_INFO);
    }
}

void DesktopPanel::OnRoleDesktopChanges(const quint8 &roleId)
{
    if (roleId==m_roleId)
    {
        update();
    }
}


void DesktopPanel::UpdateAllUsersWithCurrentRole()
{
    if (m_type==ICONS_PANEL_TYPE::ROLE_ICONS)
    {
        for (const QString &user:*m_usersList)
        {
            m_iconMakingService->SetDefaultIconsToUser(m_roleId, 1, user);
        }
    }
    else
    {
        qDebug()<<QStringLiteral("невозможно обновить для виджета-роли иконки текущий роли").toUtf8();
        qFatal(Q_FUNC_INFO);
    }
}

void DesktopPanel::OnAddProgram(const QString &exec, const QString &iconPath, const QString &iconName)
{
    DesktopEntity entity;
    entity.exec=exec;
    entity.icon=iconPath;
    entity.name=iconName;
    if(m_type==ICONS_PANEL_TYPE::ROLE_ICONS)
    {
        m_iconMakingService->AddIconToRole(entity, m_roleId);
//        UpdateAllUsersWithCurrentRole();
    }
    else
    {
        m_iconMakingService->AddIconToUser(entity, m_userName);
    }
}

void DesktopPanel::OnDeleteProgram()
{
    if(m_type==ICONS_PANEL_TYPE::ROLE_ICONS)
    {
        m_iconMakingService->DeleteIconToRole(m_selectedItemName, m_roleId);
        UpdateAllUsersWithCurrentRole();
    }
    else
    {
        m_iconMakingService->DeleteIconToUser(m_userName, m_selectedItemName);
    }
    m_deleteProgramButton->setDisabled(true);
}

void DesktopPanel::OnProgramSelect(const QModelIndex &index)
{
    QVariant indexData=index.data(Qt::UserRole+1);
    DesktopEntity entity=indexData.value<DesktopEntity>();
    m_selectedItemName=entity.name;
    m_deleteProgramButton->setText(QStringLiteral("Удалить с рабочего стола"));
    m_deleteProgramButton->setEnabled(true);
}

void DesktopPanel::SetUser(const User &user)
{
    if(m_type==ICONS_PANEL_TYPE::ROLE_ICONS)
    {
        qFatal("Irregular type");
    }
    else
    {
        m_userName=user.name;
        m_programsToRun->setText(QStringLiteral("Ярлыки на рабочий стол для пользователя: %1").arg(m_userName));
        m_addProgramButton->setEnabled(true);
        m_deleteProgramButton->setDisabled(true);
        m_iconMakingService->GetAllUserDesktops(m_userName);
    }
}

void DesktopPanel::SetRoleId(const quint8 &roleId)
{
    if(m_type==ICONS_PANEL_TYPE::ROLE_ICONS)
    {
        m_roleId=roleId;
        m_programsToRun->setText("Ярлыки на рабочий стол для роли: "+ Roles.at(roleId));
        m_addProgramButton->setEnabled(true);
        m_deleteProgramButton->setDisabled(true);
        m_iconMakingService->GetAllRoleDesktops(roleId);

    }
    else
    {
        qDebug()<< QStringLiteral("Невозможно для виджета рабочего стола получить ярлыки для роли, это другой виджет");
        qFatal(Q_FUNC_INFO);
    }
}

void DesktopPanel::DeleteUserAllRoleIcons()
{
    if(m_roleId>0 && m_roleId<Roles.count())
    {
        if(!m_userName.isEmpty())
        {
            m_iconMakingService->DeleteRoleIconFromUser(m_userName, m_roleId);
        }
    }
}
