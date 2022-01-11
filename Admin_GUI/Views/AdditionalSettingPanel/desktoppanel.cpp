#include "desktoppanel.h"

DesktopPanel::DesktopPanel(const ICONS_PANEL_TYPE type, UserDesktopService * userDesktopService, RoleDesktopService * roleDesktopService, QWidget *parent)
    : QWidget(parent)
    , m_type(type)
    , m_userDesktopService(userDesktopService)
    , m_roleDesktopService(roleDesktopService)
{
    SetPresenterModelToView();
    CreateUI();
    SetBackgroundColor();
    InsertWidgetsIntoLayout();
    FillUI();
    ConnectObjects();
}

DesktopPanel::~DesktopPanel()
{
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

void DesktopPanel::SetPresenterModelToView()
{
    if(IsUserData())
    {
        m_model=m_userDesktopService->GetModel();
    }
    else
    {
        m_model=m_roleDesktopService->GetModel();
    }
}

void DesktopPanel::CreateUI()
{
    m_mainLayout=new QVBoxLayout();

    m_programsToRun=new QLabel();

    m_fileDelegate=new FileDelegate(this);
    m_allProgramsListView=new QListView(this);


    m_bottomLayout=new QHBoxLayout();

    m_addProgramButton=new QPushButton(QStringLiteral("Добавить программу"));
    m_deleteProgramButton=new QPushButton(QStringLiteral("Удалить выбранную программу"));

    m_dialogLayout = new QVBoxLayout();
    m_dialog=new QtMaterialDialog(this);

    m_dialogWidget = new DesktopUploadDialogWidget(this);
}

void DesktopPanel::FillUI()
{
    m_allProgramsListView->setModel(m_model);
    m_allProgramsListView->setItemDelegate(m_fileDelegate);
    m_allProgramsListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_addProgramButton->setDisabled(true);
    m_deleteProgramButton->setDisabled(true);

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

void DesktopPanel::OnSetDefaultRoleApps(const quint8 &roleId)
{
    if (IsUserData())
    {
        m_roleDesktopService->SetDefaultIconsToUserOnUserRoleUpdate(m_roleId, roleId, m_userName);
        m_roleId=roleId;
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

void DesktopPanel::OnAddProgram(const QString &exec, const QString &iconPath, const QString &iconName)
{
    DesktopEntity entity;
    entity.exec=exec;
    entity.icon=iconPath;
    entity.name=iconName;
    if(IsUserData())
    {
        m_userDesktopService->AddIconToUser(m_userName, entity);
    }
    else
    {
        m_roleDesktopService->AddIconToRole(m_roleId, entity);
    }
}

void DesktopPanel::OnDeleteProgram()
{
    if(IsUserData())
    {
        m_userDesktopService->DeleteIconToUser(m_userName, m_selectedItemName);
    }
    else
    {
        m_roleDesktopService->DeleteIconToRole(m_roleId, m_selectedItemName);
    }
    m_deleteProgramButton->setDisabled(true);
}

void DesktopPanel::OnProgramSelect(const QModelIndex &index)
{
    QVariant indexData=index.data(Qt::UserRole+1);
    const DesktopEntity entity=indexData.value<DesktopEntity>();
    m_selectedItemName=entity.name;
    m_deleteProgramButton->setText(QStringLiteral("Удалить с рабочего стола"));
    m_deleteProgramButton->setEnabled(true);
}

void DesktopPanel::SetUser(const User &user)
{
    if(IsUserData())
    {
        m_userName=user.name;
        m_programsToRun->setText(QStringLiteral("Ярлыки на рабочий стол для пользователя: %1").arg(m_userName));
        m_addProgramButton->setEnabled(true);
        m_deleteProgramButton->setDisabled(true);
        m_userDesktopService->GetAllUserDesktops(m_userName);
    }
    else
    {
        qDebug()<< QStringLiteral("Невозможно для виджета роли получить данные пользователя, так как это виджет роли а не рабочего стола");
        qFatal(Q_FUNC_INFO);
    }
}

void DesktopPanel::SetRoleId(const quint8 &roleId)
{
    if(IsUserData())
    {
        qDebug()<< QStringLiteral("Невозможно для виджета рабочего стола получить ярлыки для роли, это другой виджет").toUtf8();
        qFatal(Q_FUNC_INFO);
    }
    else
    {
        m_roleId=roleId;
        m_programsToRun->setText("Ярлыки на рабочий стол для роли: "+ Roles.at(roleId));
        m_addProgramButton->setEnabled(true);
        m_deleteProgramButton->setDisabled(true);
        m_roleDesktopService->GetAllRoleDesktops(roleId);
    }
}

void DesktopPanel::DeleteUserAllRoleIcons()
{
    if(IsUserData())
    {
        if(m_roleId>=0 && m_roleId<Roles.count())
        {
            m_userDesktopService->DeleteAllIconsToUser(m_roleId, m_userName);
        }
    }
    else
    {
        qDebug()<< QStringLiteral("Невозможно для виджета роли удалить все иконки с параметром пользователя").toUtf8();
        qFatal(Q_FUNC_INFO);
    }
}

bool DesktopPanel::IsUserData()
{
    return m_type==ICONS_PANEL_TYPE::USER_ICONS;
}
