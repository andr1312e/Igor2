#include <QDebug>
#include "usereditpanel.h"

UserEditPanel::UserEditPanel(QStringView currentUserName, QWidget *parent)
    : QWidget(parent)
    , m_currentUserName(currentUserName)
    , m_rolesToolTip{  QStringLiteral("Интерфейс редактирования недоступен. Доступны только базовые возможности в запускаемом ПО"),
                       QStringLiteral("Интерфейс редактирования недоступен. В запускаемых программах будут включены доп. возможности"),
                       QStringLiteral("Обладает возможностями по просмотру базы данных, а так же доп. возможности запускаемого ПО"),
                       QStringLiteral("Обладает неограниченными возможностями по редактированию базы данных, устанавливаете эту роль только по необходимости"),
                    }
{
    InitServicesAndModel();
    CreateUI();
    InsertWidgetsIntoLayout();
    FillUI();
    SetObjectNames();
    ConnectObjects();
}

UserEditPanel::~UserEditPanel()
{
    delete m_kioskService;

    delete m_actionLayout;
    delete m_kioskLayout;
    delete m_mainLayout;

    delete m_editFieldsLabel;

    delete m_FCSFieldsLabel;
    delete m_FCSLineEdit;

    delete m_roleEditLabel;
    delete m_roleComboBox;

    delete m_saveUserDataButton;
    delete m_removeUserButton;

    delete m_kioskModeIsEnabledLabel;
    delete m_kioskModeIsDisabledLabel;
    delete m_kioskModeState;

    delete m_messagBox;
}

void UserEditPanel::InitServicesAndModel()
{
    m_kioskService = new KioskService();
}

void UserEditPanel::CreateUI()
{
    m_mainLayout = new QVBoxLayout();

    m_editFieldsLabel = new QLabel();

    m_FCSFieldsLabel = new QLabel();
    m_FCSLineEdit = new QLineEdit();

    m_roleEditLabel = new QLabel();
    m_roleComboBox = new QComboBox();

    m_actionLayout = new QHBoxLayout();
    m_saveUserDataButton = new QPushButton();
    m_removeUserButton = new QPushButton();

    m_kioskLayout = new QHBoxLayout();
    m_kioskModeIsEnabledLabel = new QLabel();
    m_kioskModeIsDisabledLabel = new QLabel();
    m_kioskModeState = new QtMaterialToggle();

    m_messagBox = new QMessageBox(this);
}

void UserEditPanel::InsertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_editFieldsLabel);
    m_mainLayout->addWidget(m_FCSFieldsLabel);
    m_mainLayout->addWidget(m_FCSLineEdit);
    m_mainLayout->addWidget(m_roleEditLabel);
    m_mainLayout->addWidget(m_roleComboBox);

    m_actionLayout->addWidget(m_saveUserDataButton);
    m_actionLayout->addWidget(m_removeUserButton);

    m_kioskLayout->addWidget(m_kioskModeIsDisabledLabel);
    m_kioskLayout->addWidget(m_kioskModeState);
    m_kioskLayout->addWidget(m_kioskModeIsEnabledLabel);

    m_mainLayout->addLayout(m_actionLayout);
    m_mainLayout->addLayout(m_kioskLayout);
    m_mainLayout->setContentsMargins(2, 0, 2, 0);
    m_mainLayout->setSpacing(3);

    setLayout(m_mainLayout);
}

void UserEditPanel::FillUI()
{
    m_editPanelFont = QFont(m_FCSFieldsLabel->font());

    m_editFieldsLabel->setText(userEditFields.front());
    m_FCSFieldsLabel->setText(QStringLiteral("Фамилия имя отчество: "));
    m_roleEditLabel->setText(QStringLiteral("Роль в системе: "));

    for (int i = 0; i < Roles.count(); ++i)
    {
        m_roleComboBox->addItem(Roles.at(i));
        m_roleComboBox->setItemData(i, m_rolesToolTip.at(i), Qt::ToolTipRole);
    }

    m_saveUserDataButton->setText(QStringLiteral("Сохранить"));
    m_removeUserButton->setText(QStringLiteral("Удалить"));

    m_kioskModeIsEnabledLabel->setText("Режим киоска включен");
    m_kioskModeIsDisabledLabel->setText("Режим киоска выключен");
    m_kioskModeState->setMinimumHeight(45);
    m_kioskModeState->setOrientation(Qt::Horizontal);
    m_kioskModeState->setDisabled(true);

    m_messagBox->setText(QStringLiteral("Введите все параметры"));
}

void UserEditPanel::SetObjectNames()
{
    m_saveUserDataButton->setObjectName(QStringLiteral("add"));
    m_removeUserButton->setObjectName(QStringLiteral("remove"));
}

void UserEditPanel::ConnectObjects()
{
    connect(m_saveUserDataButton, &QPushButton::clicked, this, &UserEditPanel::OnSaveUser);
    connect(m_removeUserButton, &QPushButton::clicked, this, &UserEditPanel::OnDeleteUser);
    connect(m_roleComboBox, QOverload<int>::of(&QComboBox::activated), this, &UserEditPanel::ToRoleToViewChanged);
    connect(m_kioskModeState, &QtMaterialToggle::clicked, this, &UserEditPanel::OnSetKioskMode);
}

void UserEditPanel::OnSaveUser()
{
    const QString FCS = m_FCSLineEdit->text();
    if (FCS.isEmpty() || m_userId.isEmpty())
    {
        m_messagBox->show();
    }
    else
    {
        const int roleIndex = m_roleComboBox->currentIndex();
        Q_EMIT ToSaveUser(m_userId, m_userName, FCS, m_oldRoleIndex, roleIndex);
        m_oldRoleIndex = roleIndex;
        ShowSaveUserToast(m_userName);
    }
}

void UserEditPanel::OnDeleteUser()
{
    if (!m_userId.isEmpty())
    {
        m_FCSLineEdit->clear();
        m_roleComboBox->setCurrentIndex(0);
        Q_EMIT ToDeleteUser(m_userId, m_userName);
    }
}

void UserEditPanel::OnSetKioskMode(bool kioskModeState)
{
    if (kioskModeState)
    {
        m_kioskService->LockUser(m_userName);
    }
    else
    {
        m_kioskService->UnLockUser(m_userName);
    }
}

void UserEditPanel::SetUser(const User &user)
{
    m_userName = user.GetUserName();
    m_userId = user.GetUserId();
    m_oldRoleIndex = user.GetUserRole();
    GetUserKioskState(m_userName);
    InsertUserData(user);
    if (m_currentUserName == m_userName)
    {
        m_kioskModeState->setDisabled(true);
        m_kioskModeState->setToolTip(QStringLiteral("Режим системного киоска для текущего пользователя установить нельзя"));
    }
    else
    {
        m_kioskModeState->setEnabled(true);
        m_kioskModeState->setToolTip(QStringLiteral("Режим киоска -  пользователь не имеет возможности запустить ни одну системную программу, т.к. эти действия замаскированы "));
    }
}

void UserEditPanel::SetFontSize(int fontSize)
{
    m_editPanelFont.setPointSize(fontSize);
    m_editFieldsLabel->setFont(m_editPanelFont);
    m_FCSFieldsLabel->setFont(m_editPanelFont);
    m_roleEditLabel->setFont(m_editPanelFont);
    m_FCSLineEdit->setFont(m_editPanelFont);
    m_kioskModeIsEnabledLabel->setFont(m_editPanelFont);
    m_kioskModeIsDisabledLabel->setFont(m_editPanelFont);
    m_saveUserDataButton->setFont(m_editPanelFont);
    m_FCSLineEdit->setFont(m_editPanelFont);
    m_removeUserButton->setFont(m_editPanelFont);
    m_roleComboBox->setFont(m_editPanelFont);
}


void UserEditPanel::SetButtonSize(int size)
{
    m_saveUserDataButton->setFixedHeight(size);
    m_FCSLineEdit->setFixedHeight(size);
    m_removeUserButton->setFixedHeight(size);
    m_roleComboBox->setFixedHeight(size);
}

void UserEditPanel::GetUserKioskState(const QString &userName)
{
    if (m_kioskService->IsUserInKiosk(userName))
    {
        m_kioskModeState->setChecked(true);
    }
    else
    {
        m_kioskModeState->setChecked(false);
    }
    m_kioskModeState->setEnabled(true);
}

void UserEditPanel::InsertUserData(const User &user)
{
    if (user.RoleIsValid())
    {
        m_editFieldsLabel->setText(userEditFields.back());
        m_FCSLineEdit->setText(user.GetUserFCS());
        m_roleComboBox->setCurrentText(Roles.at(user.GetUserRole()));
    }
    else
    {
        m_editFieldsLabel->setText(userEditFields.front());
        m_FCSLineEdit->clear();
        m_roleComboBox->setCurrentIndex(0);
    }
}

void UserEditPanel::ShowSaveUserToast(const QString &userName)
{
    QToast *const pToast = QToast::CreateToast("Данные пользователя " + userName + " сохранены в базе", QToast::LENGTH_LONG, this);
    pToast->show();
}




