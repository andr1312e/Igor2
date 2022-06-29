#include "desktoppanel.h"

#include <QMimeData>

DesktopPanel::DesktopPanel(const IconsPanelType type, UserDesktopService *userDesktopService, RoleDesktopService *roleDesktopService, QWidget *parent)
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
    if (IsUserData())
    {
        startTimer(4000, Qt::VeryCoarseTimer);
    }
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
    if (IsUserData())
    {
        m_model = m_userDesktopService->GetModel();
    }
    else
    {
        m_model = m_roleDesktopService->GetModel();
    }
}

void DesktopPanel::CreateUI()
{
    m_mainLayout = new QVBoxLayout();

    m_programsToRun = new QLabel();

    m_fileDelegate = new FileDelegate(this);
    m_allProgramsListView = new QListView(this);

    m_bottomLayout = new QHBoxLayout();
    if (IsUserData())
    {
        m_addProgramButton = new QPushButton(QStringLiteral("Добавить ярлык"));
        m_deleteProgramButton = new QPushButton(QStringLiteral("Удалить ярлык"));
    }
    else
    {
        m_addProgramButton = new QPushButton(QStringLiteral("Добавить программу"));
        m_deleteProgramButton = new QPushButton(QStringLiteral("Удалить программу"));
    }


    m_dialogLayout = new QVBoxLayout();
    m_dialog = new QtMaterialDialog(this);

    m_dialogWidget = new DesktopUploadDialogWidget(this);
}

void DesktopPanel::FillUI()
{
    m_allProgramsListView->setModel(m_model);
    m_allProgramsListView->setItemDelegate(m_fileDelegate);
    if (IconsPanelType::RoleIcons == m_type)
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
    if (IsUserData())
    {
        m_dialogWidget->SetTitleText(QStringLiteral("Добавить программу пользователю"));
        m_programsToRun->setText(QStringLiteral("Ярлыки для пользователей...."));
    }
    else
    {
        m_programsToRun->setText(QStringLiteral("Программы для ролей...."));
        m_dialogWidget->SetTitleText(QStringLiteral("Добавить программу для роли"));
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
    m_mainLayout->setContentsMargins(0, 0, 0, 0);

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
    const DesktopEntity entity(iconName, exec, iconPath);
    if (IsUserData())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Хотим добавить пользователю ярлык: ") + entity.GetExec() + QStringLiteral(" у пользователя: ") + m_userName);
        m_userDesktopService->AddIconToUser(m_userName, entity);
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Хотим добавить роли программу: ") + entity.GetExec() + QStringLiteral(" у роли: ") + QString::number(m_currentRoleId));
        m_roleDesktopService->AddIconToRole(m_currentRoleId, entity);
    }
}

void DesktopPanel::OnDeleteProgram()
{
    const QModelIndex index = m_allProgramsListView->currentIndex();
    if (index.isValid())
    {
        const QVariant indexData = index.data(Qt::UserRole + 1);
        const DesktopEntity entity = indexData.value<DesktopEntity>();
        const QString programName = entity.GetName();
        if (IsUserData())
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Хотим удалить пользователю ярлык: ") + programName + QStringLiteral(" у пользователя: ") + m_userName);
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
    if (IsUserData())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Устанавливаем для просмотра пользователя: ") + user.GetUserName());
        m_userName = user.GetUserName();
        m_programsToRun->setText(QStringLiteral("Ярлыки пользователя: %1").arg(m_userName));
        m_addProgramButton->setEnabled(true);
        m_deleteProgramButton->setDisabled(true);
        m_userDesktopService->GetAllUserDesktops(m_userName);
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Невозможно для виджета роли получить данные пользователя, так как это виджет роли а не рабочего стола ")).toUtf8().constData());
    }
}

void DesktopPanel::SetRoleId(int roleId)
{
    if (IsUserData())
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Невозможно для виджета рабочего стола получить ярлыки для роли, это другой виджет ")).toUtf8().constData());
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Устанавливаем для просмотра роль: ") + QString::number(roleId));
        m_currentRoleId = roleId;
        m_programsToRun->setText(QStringLiteral("Программы роли: ") + Roles.at(roleId));
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
    const QListView *const sourceListView = static_cast<QListView *>(event->source());
    if (Q_NULLPTR == sourceListView)
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Невозможно получить данные иконки, так как дропает сюда не лист вью ")).toUtf8().constData());
    }
    else
    {
        const QModelIndex index = sourceListView->currentIndex();
        if (index.isValid())
        {
            const QVariant indexData(index.data(Qt::UserRole + 1));
            const DesktopEntity entity(indexData.value<DesktopEntity>());
            qInfo() << "entity.name " << entity.GetName();
            InsertDragItem(entity);
        }
        else
        {
            qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Немогу получть данные из перетаскиваемого итема, так как индекс не валид ")).toUtf8().constData());
        }
    }
}

void DesktopPanel::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_Delete == event->key())
    {
        OnDeleteProgram();
        return;
    }
    if (Qt::Key_N == event->key() && (event->modifiers()&Qt::ControlModifier))
    {
        m_dialog->OnShowDialog();
    }
}

void DesktopPanel::timerEvent(QTimerEvent *event)
{
    if (IsUserData())
    {
        if (!m_userName.isEmpty())
        {
            m_userDesktopService->GetAllUserDesktops(m_userName);
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Невозможно обновить данные рабочего стола, так как это виджет роли")).toUtf8().constData());
    }
}

void DesktopPanel::InsertDragItem(const DesktopEntity &entity)
{
    m_userDesktopService->AddIconToUser(m_userName, entity);
}

bool DesktopPanel::IsUserData() Q_DECL_CONST_FUNCTION
{
    return IconsPanelType::UserIcons == m_type;
}
