#include "desktoppanel.h"

#include <QMimeData>

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
    m_deleteProgramButton=new QPushButton(QStringLiteral("Удалить программу"));

    m_dialogLayout = new QVBoxLayout();
    m_dialog=new QtMaterialDialog(this);

    m_dialogWidget = new DesktopUploadDialogWidget(this);
}

void DesktopPanel::FillUI()
{
    m_allProgramsListView->setModel(m_model);
    m_allProgramsListView->setItemDelegate(m_fileDelegate);
    if(ICONS_PANEL_TYPE::ROLE_ICONS==m_type)
    {
        m_allProgramsListView->setDragDropMode(QAbstractItemView::DragOnly);
    }
    else
    {
        setAcceptDrops(true);
    }
    m_allProgramsListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_addProgramButton->setDisabled(true);
    m_deleteProgramButton->setDisabled(true);
    if(IsUserData())
    {
        m_dialogWidget->SetTitleText(QStringLiteral("Ярлык для пользователя:"));
        m_programsToRun->setText(QStringLiteral("Выберите пользователя"));
    }
    else
    {
        m_programsToRun->setText(QStringLiteral("Ярлыки на рабочий стол для роли"));
        m_dialogWidget->SetTitleText(QStringLiteral("Выберите роль"));
    }
    QWidget::setMinimumHeight(255);
    QWidget::setMinimumWidth(420);
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

void DesktopPanel::OnAddProgram(const QString &exec, const QString &iconPath, const QString &iconName)
{
    DesktopEntity entity;
    entity.exec=exec;
    entity.icon=iconPath;
    entity.name=iconName;
    entity.type=QStringLiteral("Application");
    if(IsUserData())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Хотим добавить юзеру программу: ") + entity.exec + QStringLiteral(" у пользователя: ") + m_userName);
        m_userDesktopService->AddIconToUser(m_userName, entity);
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Хотим добавить роли программу: ") + entity.exec + QStringLiteral(" у роли: ") + QString::number(m_currentRoleId));
        m_roleDesktopService->AddIconToRole(m_currentRoleId, entity);
    }
}

void DesktopPanel::OnDeleteProgram()
{
    const QModelIndex index=m_allProgramsListView->currentIndex();
    if(index.isValid())
    {
        const QVariant indexData=index.data(Qt::UserRole+1);
        const DesktopEntity entity=indexData.value<DesktopEntity>();
        const QString programName=entity.name;
        if(IsUserData())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Хотим удалить юзеру программу: ") + programName + QStringLiteral(" у пользователя: ") + m_userName);
            m_userDesktopService->DeleteIconToUser(m_userName, programName);
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Хотим удалить роли программу: ") + programName + QStringLiteral(" у роли: ") + QString::number(m_currentRoleId));
            m_roleDesktopService->DeleteIconToRole(m_currentRoleId, programName);
        }
        m_deleteProgramButton->setDisabled(true);
    }
}

void DesktopPanel::OnProgramSelect(const QModelIndex &index)
{
    Q_UNUSED(index)
    m_deleteProgramButton->setEnabled(true);
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Была сделан клик по меню "));
}

void DesktopPanel::SetUser(const User &user)
{
    if(IsUserData())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Устанавливаем для просмотра пользователя: ") + user.name);
        m_userName=user.name;
        m_programsToRun->setText(QStringLiteral("Ярлыки на рабочий стол для пользователя: %1").arg(m_userName));
        m_addProgramButton->setEnabled(true);
        m_deleteProgramButton->setDisabled(true);
        m_userDesktopService->GetAllUserDesktops(m_userName);
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Невозможно для виджета роли получить данные пользователя, так как это виджет роли а не рабочего стола ")).toUtf8().constData());
    }
}

void DesktopPanel::SetRoleId(int roleId)
{
    if(IsUserData())
    {
        qFatal("%s", QString(Q_FUNC_INFO+ QStringLiteral(" Невозможно для виджета рабочего стола получить ярлыки для роли, это другой виджет ")).toUtf8().constData());
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Устанавливаем для просмотра роль: ") + QString::number(roleId));
        m_currentRoleId=roleId;
        m_programsToRun->setText(QStringLiteral("Ярлыки на рабочий стол для роли: ")+ Roles.at(roleId));
        m_addProgramButton->setEnabled(true);
        m_deleteProgramButton->setDisabled(true);
        m_roleDesktopService->GetAllRoleDesktops(roleId);
    }
}

void DesktopPanel::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void DesktopPanel::dropEvent(QDropEvent *event)
{
    const QListView *sourceListView=static_cast<QListView*>(event->source());
    if(Q_NULLPTR==sourceListView)
    {
        qFatal("");
    }
    else
    {
        const QModelIndex index=sourceListView->currentIndex();
        if(index.isValid())
        {
            const QVariant indexData(index.data(Qt::UserRole+1));
            const DesktopEntity entity=indexData.value<DesktopEntity>();
            qInfo()<< "entity.name " << entity.name;
            InsertDragItem(entity);
        }
        else
        {
            qFatal("fd");
        }
    }
}

void DesktopPanel::keyPressEvent(QKeyEvent *event)
{
    if(Qt::Key_Delete==event->key())
    {
        OnDeleteProgram();
        return;
    }
    if(Qt::Key_N==event->key() && (event->modifiers()&Qt::ControlModifier))
    {
        m_dialog->OnShowDialog();
    }
}

void DesktopPanel::InsertDragItem(const DesktopEntity &entity)
{
    m_userDesktopService->AddIconToUser(m_userName, entity);
}

bool DesktopPanel::IsUserData() const
{
    return ICONS_PANEL_TYPE::USER_ICONS==m_type;
}
