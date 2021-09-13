#include "usereditpanel.h"

UserEditPanel::UserEditPanel(const QString &userName, DatabaseService *repository, QWidget *parent)
    : QWidget(parent)
    , m_currentUserName(userName)
{
    initServices(repository);
    initUI();
    applyStyle();
    insertWidgetsIntoLayout();
    createConnections();
}

UserEditPanel::~UserEditPanel()
{
    delete m_actionLayout;
    delete m_kioskLayout;
    delete m_mainLayout;

    delete m_editFieldsLabel;
    delete m_FCSLineEdit;
    delete m_rankEditLabel;
    delete m_roleEditLabel;
    delete m_rankComboBox;
    delete m_roleComboBox;

    delete m_addUserButton;
    delete m_removeUserButton;

    delete m_kioskModeIsEnabledLabel;
    delete m_kioskModeIsDisabledLabel;
    delete m_kiosModeState;

    delete m_messagBox;

}

void UserEditPanel::setUser(User &user)
{
    m_userName=user.name;
    m_userId=user.userId;
    checkUserKiosk(m_userName);
    insertUserData(user);
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

void UserEditPanel::initServices(DatabaseService *repository)
{
    m_accountRepository=repository;
    m_kioskLockUnLockService=new KioskService(repository->getTerminal());
}

void UserEditPanel::initUI()
{

    m_mainLayout=new QVBoxLayout();

    m_editFieldsLabel=new QLabel(userEditFields.at(0));
    m_FCSLineEdit=new QtMaterialTextField();
    m_FCSLineEdit->setLabel("Фамилия имя отчество: ");
    m_FCSLineEdit->setMinimumWidth(250);
    m_rankEditLabel=new QLabel("Звание: ");
    m_rankComboBox=new QComboBox();
    QStringList::const_iterator it_first = Ranks.cbegin();
    QStringList::const_iterator it_second = RanksICO.cbegin();
    while (it_first!=Ranks.cend() || it_second!=RanksICO.cend())
    {
        m_rankComboBox->addItem(QIcon(*it_second), *it_first);
        ++it_first;
        ++it_second;
    }
    m_rankComboBox->setIconSize(QSize(70, 30));

    m_roleEditLabel=new QLabel("Роль в системе: ");
    m_roleComboBox=new QComboBox();


    m_roleComboBox->addItems(Roles);
    for (int i=0; i<Roles.length(); i++)
    {
        m_roleComboBox->setItemData(i, RolesToolTip.at(i), Qt::ToolTipRole);
    }

    m_actionLayout=new QHBoxLayout();
    m_addUserButton=new QPushButton("Записать пользователя в базу");
    m_removeUserButton=new QPushButton("Удалить пользователя из базы");

    m_kioskLayout=new QHBoxLayout();
    m_kioskLayout->setAlignment(Qt::AlignTop);
    m_kioskModeIsEnabledLabel=new QLabel("Режим киоска включен");
    m_kioskModeIsDisabledLabel=new QLabel("Режим киоска выключен");

    m_kiosModeState=new QtMaterialToggle();
    m_kiosModeState->setMinimumHeight(45);
    m_kiosModeState->setOrientation(Qt::Horizontal);
    m_kiosModeState->setDisabled(true);

    m_messagBox=new QMessageBox(this);
    m_messagBox->setText("Введите все параметры");

}

void UserEditPanel::applyStyle()
{
    m_addUserButton->setObjectName("add");
    m_removeUserButton->setObjectName("remove");
}

void UserEditPanel::insertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_editFieldsLabel);
    m_mainLayout->addWidget(m_FCSLineEdit);
    m_mainLayout->addWidget(m_rankEditLabel);
    m_mainLayout->addWidget(m_rankComboBox);
    m_mainLayout->addWidget(m_roleEditLabel);
    m_mainLayout->addWidget(m_roleComboBox);

    m_actionLayout->addWidget(m_addUserButton);
    m_actionLayout->addWidget(m_removeUserButton);

    m_kioskLayout->addWidget(m_kioskModeIsDisabledLabel);
    m_kioskLayout->addWidget(m_kiosModeState);
    m_kioskLayout->addWidget(m_kioskModeIsEnabledLabel);

    m_mainLayout->addLayout(m_actionLayout);
    m_mainLayout->addLayout(m_kioskLayout);

    setLayout(m_mainLayout);
}

void UserEditPanel::createConnections()
{
    connect(m_addUserButton, &QPushButton::clicked, this, &UserEditPanel::saveUser);
    connect(m_removeUserButton, &QPushButton::clicked, this, &UserEditPanel::deleteUser);
    connect(m_roleComboBox, &QComboBox::currentTextChanged, this, &UserEditPanel::roleToViewChanged);
    connect(m_kiosModeState, &QtMaterialToggle::clicked, this, &UserEditPanel::setKioskMode);
}

void UserEditPanel::setKioskMode(bool kioskModeState)
{
        if (kioskModeState)
        {
            m_kioskLockUnLockService->lockUser(m_userName);
        }
        else
        {
            m_kioskLockUnLockService->unLockUser(m_userName);
        }
}

void UserEditPanel::checkUserKiosk(QString &userName)
{
    if (m_kioskLockUnLockService->isUserInKiosk(userName))
    {
        m_kiosModeState->setChecked(true);
    }
    else
    {
        m_kiosModeState->setChecked(false);
    }
    m_kiosModeState->setEnabled(true);
}

void UserEditPanel::insertUserData(User &user)
{
    if (user.hasData)
    {
        m_editFieldsLabel->setText(userEditFields.at(1));
        m_FCSLineEdit->setText(user.FCS);
        m_rankComboBox->setCurrentText(user.rank);
        m_roleComboBox->setCurrentText(user.role);
    }
    else
    {
        m_editFieldsLabel->setText(userEditFields.at(0));
        m_FCSLineEdit->clear();
        m_rankComboBox->setCurrentIndex(0);
        m_roleComboBox->setCurrentIndex(0);
    }
}

void UserEditPanel::showToast(QString &userName)
{
    QToast* pToast=QToast::CreateToast("Данные пользователя " + userName + " сохранены в базе",QToast::LENGTH_LONG, this);
    pToast->show();
}

void UserEditPanel::saveUser()
{
    const QString FCS=m_FCSLineEdit->text();
    if (FCS=="" || m_userId=="")
    {

        m_messagBox->show();
    }
    else
    {
        const QString rank=m_rankComboBox->currentText();
        const QString role=m_roleComboBox->currentText();
        emit setDefaultRoleApps(role);
        emit fillUserAdditionalInfo(FCS, rank, role);
        showToast(m_userName);
    }
}

void UserEditPanel::deleteUser()
{
    if (m_userId!="")
    {
        m_rankComboBox->setCurrentIndex(0);
        m_FCSLineEdit->clear();
        m_roleComboBox->setCurrentIndex(0);
        emit clearUserAdditionalInfo();
    }
}

