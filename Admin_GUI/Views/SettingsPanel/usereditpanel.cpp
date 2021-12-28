#include <QDebug>
#include "usereditpanel.h"

UserEditPanel::UserEditPanel(const QString &userName, Terminal *terminal, QWidget *parent)
    : QWidget(parent)
    , m_currentUserName(userName)
{
    InitServicesAndModel(terminal);
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

    delete m_rankEditLabel;
    delete m_roleEditLabel;
    delete m_rankComboBox;
    delete m_roleComboBox;

    delete m_saveUserDataButton;
    delete m_removeUserButton;

    delete m_kioskModeIsEnabledLabel;
    delete m_kioskModeIsDisabledLabel;
    delete m_kiosModeState;

    delete m_messagBox;
}

void UserEditPanel::InitServicesAndModel(Terminal *terminal)
{
    m_kioskService=new KioskService(terminal);
}

void UserEditPanel::CreateUI()
{
    m_mainLayout=new QVBoxLayout();

    m_editFieldsLabel=new QLabel();

    m_FCSFieldsLabel=new QLabel();
    m_FCSLineEdit=new QLineEdit();

    m_rankEditLabel=new QLabel();
    m_rankComboBox=new QComboBox();

    m_roleEditLabel=new QLabel();
    m_roleComboBox=new QComboBox();

    m_actionLayout=new QHBoxLayout();
    m_saveUserDataButton=new QPushButton();
    m_removeUserButton=new QPushButton();

    m_kioskLayout=new QHBoxLayout();
    m_kioskModeIsEnabledLabel=new QLabel();
    m_kioskModeIsDisabledLabel=new QLabel();
    m_kiosModeState=new QtMaterialToggle();

    m_messagBox=new QMessageBox(this);
}

void UserEditPanel::InsertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_editFieldsLabel);
    m_mainLayout->addWidget(m_FCSFieldsLabel);
    m_mainLayout->addWidget(m_FCSLineEdit);
    m_mainLayout->addWidget(m_rankEditLabel);
    m_mainLayout->addWidget(m_rankComboBox);
    m_mainLayout->addWidget(m_roleEditLabel);
    m_mainLayout->addWidget(m_roleComboBox);

    m_actionLayout->addWidget(m_saveUserDataButton);
    m_actionLayout->addWidget(m_removeUserButton);

    m_kioskLayout->addWidget(m_kioskModeIsDisabledLabel);
    m_kioskLayout->addStretch(3);
    m_kioskLayout->addWidget(m_kiosModeState);
    m_kioskLayout->addStretch(3);
    m_kioskLayout->addWidget(m_kioskModeIsEnabledLabel);

    m_mainLayout->addLayout(m_actionLayout);
    m_mainLayout->addLayout(m_kioskLayout);

    setLayout(m_mainLayout);
}

void UserEditPanel::FillUI()
{
    m_editPanelFont= QFont(m_FCSFieldsLabel->font());

    m_editFieldsLabel->setText(userEditFields.front());
    m_FCSFieldsLabel->setText("Фамилия имя отчество: ");

    m_rankEditLabel->setText("Звание: ");
    QStringList::const_iterator it_first = Ranks.cbegin();
    QVarLengthArray<QString, 18>::const_iterator it_second = RanksICO.cbegin();
    while (it_first!=Ranks.cend() || it_second!=RanksICO.cend())
    {
        m_rankComboBox->addItem(QIcon(*it_second), *it_first);
        ++it_first;
        ++it_second;
    }
    m_rankComboBox->setIconSize(QSize(70, 30));

    m_roleEditLabel->setText("Роль в системе: ");

    m_roleComboBox->addItems(Roles);
    quint8 index=0;
    for (const QString role: RolesToolTip)
    {
        m_roleComboBox->setItemData(index, role, Qt::ToolTipRole);
        index++;
    }

    m_saveUserDataButton->setText("Сохранить");
    m_removeUserButton->setText("Удалить");

    m_kioskModeIsEnabledLabel->setText("Режим киоска включен");
    m_kioskModeIsDisabledLabel->setText("Режим киоска выключен");
    m_kiosModeState->setMinimumHeight(45);
    m_kiosModeState->setOrientation(Qt::Horizontal);
    m_kiosModeState->setDisabled(true);

    m_messagBox->setText("Введите все параметры");
}

void UserEditPanel::SetObjectNames()
{
    m_saveUserDataButton->setObjectName("add");
    m_removeUserButton->setObjectName("remove");
}

void UserEditPanel::ConnectObjects()
{
    connect(m_saveUserDataButton, &QPushButton::clicked, this, &UserEditPanel::OnSaveUser);
    connect(m_removeUserButton, &QPushButton::clicked, this, &UserEditPanel::OnDeleteUser);
    connect(m_roleComboBox, QOverload<int>::of(&QComboBox::activated), this, &UserEditPanel::ToRoleToViewChanged);
    connect(m_kiosModeState, &QtMaterialToggle::clicked, this, &UserEditPanel::OnSetKioskMode);
}

void UserEditPanel::OnSaveUser()
{
    const QString FCS=m_FCSLineEdit->text();
    if (FCS.isEmpty() || m_userId.isEmpty())
    {
        m_messagBox->show();
    }
    else
    {
        const QString rank=m_rankComboBox->currentText();
        const int roleIndex=m_roleComboBox->currentIndex();
        Q_EMIT ToSaveUser(m_userId, FCS, rank, m_oldRoleIndex, roleIndex);
        m_oldRoleIndex=roleIndex;
        ShowSaveUserToast(m_userName);
    }
}

void UserEditPanel::OnDeleteUser()
{
    if (!m_userId.isEmpty())
    {
        m_rankComboBox->setCurrentIndex(0);
        m_FCSLineEdit->clear();
        m_roleComboBox->setCurrentIndex(0);
        Q_EMIT ToDeleteUser(m_userId);
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
    m_userName=user.name;
    m_userId=user.userId;
    m_oldRoleIndex=user.role;
    GetUserKioskState(m_userName);
    InsertUserData(user);
    if (m_currentUserName==m_userName)
    {
        m_kiosModeState->setDisabled(true);
        m_kiosModeState->setToolTip("Режим системного киоска для текущего пользователя установить нельзя");
    }
    else
    {
        m_kiosModeState->setEnabled(true);
        m_kiosModeState->setToolTip("Режим киоска -  пользователь не имеет возможности запустить ни одну системную программу, т.к. эти действия замаскированы ");
    }
}

void UserEditPanel::SetFontSize(const int fontSize)
{
    m_editPanelFont.setPointSize(fontSize);
    m_editFieldsLabel->setFont(m_editPanelFont);
    m_FCSFieldsLabel->setFont(m_editPanelFont);
    m_rankEditLabel->setFont(m_editPanelFont);
    m_roleEditLabel->setFont(m_editPanelFont);
    m_FCSLineEdit->setFont(m_editPanelFont);
    m_kioskModeIsEnabledLabel->setFont(m_editPanelFont);
    m_kioskModeIsDisabledLabel->setFont(m_editPanelFont);
    m_saveUserDataButton->setFont(m_editPanelFont);
    m_FCSLineEdit->setFont(m_editPanelFont);
    m_removeUserButton->setFont(m_editPanelFont);
    m_roleComboBox->setFont(m_editPanelFont);
    m_rankComboBox->setFont(m_editPanelFont);
}


void UserEditPanel::SetButtonSize(const int size)
{
    m_saveUserDataButton->setFixedHeight(size);
    m_FCSLineEdit->setFixedHeight(size);
    m_removeUserButton->setFixedHeight(size);
    m_roleComboBox->setFixedHeight(size);
    m_rankComboBox->setFixedHeight(size);
    m_rankComboBox->setIconSize(QSize(size*1.8, size*1.8));
}

void UserEditPanel::GetUserKioskState(const QString &userName)
{
    if (m_kioskService->IsUserInKiosk(userName))
    {
        m_kiosModeState->setChecked(true);
    }
    else
    {
        m_kiosModeState->setChecked(false);
    }
    m_kiosModeState->setEnabled(true);
}

void UserEditPanel::InsertUserData(const User &user)
{
    if (user.role==-1)
    {
        m_editFieldsLabel->setText(userEditFields.at(0));
        m_FCSLineEdit->clear();
        m_rankComboBox->setCurrentIndex(0);
        m_roleComboBox->setCurrentIndex(0);
    }
    else
    {
        m_editFieldsLabel->setText(userEditFields.at(1));
        m_FCSLineEdit->setText(user.FCS);
        m_rankComboBox->setCurrentText(user.rank);
        m_roleComboBox->setCurrentText(Roles.at(user.role));
    }
}

void UserEditPanel::ShowSaveUserToast(const QString &userName)
{
    QToast* pToast=QToast::CreateToast("Данные пользователя " + userName + " сохранены в базе",QToast::LENGTH_LONG, this);
    pToast->show();
}




