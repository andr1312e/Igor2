#include <QDebug>
#include "usereditpanel.h"

UserEditPanel::UserEditPanel(const QString &userName, UserModel *model, Terminal *terminal, QWidget *parent)
    : QWidget(parent)
    , m_currentUserName(userName)
{
    initServicesAndModel(model, terminal);
    initUI();
    applyStyle();
    insertWidgetsIntoLayout();
    createConnections();
}

UserEditPanel::~UserEditPanel()
{
    delete m_kioskService;

    delete m_actionLayout;
    delete m_kioskLayout;
    delete m_mainLayout;

    delete m_editFieldsLabel;
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

void UserEditPanel::setFontSize(int fontSize)
{
    m_editPanelFont->setPointSize(fontSize);
    m_editFieldsLabel->setFont(*m_editPanelFont);
    m_FCSFieldsLabel->setFont(*m_editPanelFont);
    m_rankEditLabel->setFont(*m_editPanelFont);
    m_roleEditLabel->setFont(*m_editPanelFont);
    m_FCSLineEdit->setFont(*m_editPanelFont);
    m_kioskModeIsEnabledLabel->setFont(*m_editPanelFont);
    m_kioskModeIsDisabledLabel->setFont(*m_editPanelFont);
    m_saveUserDataButton->setFont(*m_editPanelFont);
    m_FCSLineEdit->setFont(*m_editPanelFont);
    m_removeUserButton->setFont(*m_editPanelFont);
    m_roleComboBox->setFont(*m_editPanelFont);
    m_rankComboBox->setFont(*m_editPanelFont);
}

void UserEditPanel::setButtonSize(int size)
{
    m_saveUserDataButton->setFixedHeight(size);
    m_FCSLineEdit->setFixedHeight(size);
    m_removeUserButton->setFixedHeight(size);
    m_roleComboBox->setFixedHeight(size);
    m_rankComboBox->setFixedHeight(size);
    m_rankComboBox->setIconSize(QSize(size*1.8, size*1.8));
}

void UserEditPanel::initServicesAndModel(UserModel *model, Terminal *terminal)
{
    m_model=model;
    m_kioskService=new KioskService(terminal);
}

void UserEditPanel::initUI()
{

    m_mainLayout=new QVBoxLayout();

    m_editFieldsLabel=new QLabel(userEditFields.front());
    m_FCSFieldsLabel=new QLabel("Фамилия имя отчество: ");
    m_editPanelFont=new QFont(m_FCSFieldsLabel->font());
    m_FCSLineEdit=new QLineEdit();
    m_rankEditLabel=new QLabel("Звание: ");
    m_rankComboBox=new QComboBox();
    QStringList::const_iterator it_first = Ranks.cbegin();
    std::array<QString, 18>::const_iterator it_second = RanksICO.cbegin();
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
    quint8 index=0;
    for (std::array<QString, 4>::const_iterator it=RolesToolTip.cbegin(); it!=RolesToolTip.cend(); ++it)
    {
        m_roleComboBox->setItemData(index, *it, Qt::ToolTipRole);
        index++;
    }

    m_actionLayout=new QHBoxLayout();
    m_saveUserDataButton=new QPushButton(QStringLiteral("Сохранить"));
    m_removeUserButton=new QPushButton(QStringLiteral("Удалить"));

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
    m_saveUserDataButton->setObjectName("add");
    m_removeUserButton->setObjectName("remove");
}

void UserEditPanel::insertWidgetsIntoLayout()
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
    m_kioskLayout->addWidget(m_kiosModeState);
    m_kioskLayout->addWidget(m_kioskModeIsEnabledLabel);

    m_mainLayout->addLayout(m_actionLayout);
    m_mainLayout->addLayout(m_kioskLayout);

    setLayout(m_mainLayout);
}

void UserEditPanel::createConnections()
{
    connect(m_saveUserDataButton, &QPushButton::clicked, this, &UserEditPanel::saveUser);
    connect(m_removeUserButton, &QPushButton::clicked, this, &UserEditPanel::deleteUser);
    connect(m_roleComboBox, &QComboBox::currentTextChanged, this, &UserEditPanel::roleToViewChanged);
    connect(m_kiosModeState, &QtMaterialToggle::clicked, this, &UserEditPanel::setKioskMode);
}

void UserEditPanel::setKioskMode(bool kioskModeState)
{
    if (kioskModeState)
    {
        m_kioskService->lockUser(m_userName);
    }
    else
    {
        m_kioskService->unLockUser(m_userName);
    }
}

void UserEditPanel::checkUserKiosk(QString &userName)
{
    if (m_kioskService->isUserInKiosk(userName))
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
        m_model->addUserToModel(m_userId, FCS, rank, role);
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
        m_model->deleteUserFromModel(m_userId);
        emit clearUserAdditionalInfo();
    }
}



