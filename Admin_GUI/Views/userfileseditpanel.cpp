#include "userfileseditpanel.h"

UserFilesEditPanel::UserFilesEditPanel(Terminal *terminal, QWidget *parent)
    : QWidget(parent)
{
    initUI(terminal);
    insertWidgetsIntoLayout();
    createConnections();
}

UserFilesEditPanel::~UserFilesEditPanel()
{
    delete m_topUserNameLayout;
    delete m_topRoleLayout;
    delete m_mainLayout;

    delete m_userNameLabel;
    delete m_userNameValue;
    delete m_userRoleLabel;
    delete m_userRoleValue;
    delete m_userDesktopPanel;
    delete m_userRunnubleApplicationPanel;
}

void UserFilesEditPanel::setUserNameAndRole(const QString &userName, const QString &role)
{
    m_userNameValue->setText(userName);
    if (role=="")
    {
        m_userRoleValue->setText("Пользователь не в базе");
    }
    else
    {
        m_userRoleValue->setText(role);
    }
    m_userDesktopPanel->setParam(userName, nullptr);
    m_userRunnubleApplicationPanel->setParam(userName, nullptr);
}

void UserFilesEditPanel::initUI(Terminal *terminal)
{

    m_mainLayout= new QVBoxLayout();

    m_topUserNameLayout= new QHBoxLayout();

    m_topRoleLayout=new QHBoxLayout();

    m_userNameLabel= new QLabel("Панель управления рабочем столом пользователя: ");

    m_userNameValue= new QLabel("Выберите пользователя слева");

    m_userRoleLabel= new QLabel("Роль выбранного пользователя:");

    m_userRoleValue= new QLabel("Выберите пользователя слева");

    m_userDesktopPanel= new DesktopPanel(terminal, ICONS_PANEL_TYPE::USER_DESKTOP ,this);

    m_userRunnubleApplicationPanel= new StartupPanel(terminal, STARTUP_PANEL_TYPE::USER_APPS,  this);


//    QPalette pal = this->palette();
//    pal.setColor(QPalette::Window, QColor(78, 75, 82));
//    this->setPalette(pal);
//    this->setAutoFillBackground(true);
}

void UserFilesEditPanel::insertWidgetsIntoLayout()
{

    m_topUserNameLayout->addWidget(m_userNameLabel);
    m_topUserNameLayout->addStretch(3);
    m_topUserNameLayout->addWidget(m_userNameValue);

    m_topRoleLayout->addWidget(m_userRoleLabel);
    m_topRoleLayout->addStretch(3);
    m_topRoleLayout->addWidget(m_userRoleValue);

    m_mainLayout->addLayout(m_topUserNameLayout);
    m_mainLayout->addLayout(m_topRoleLayout);

    m_mainLayout->addWidget(m_userDesktopPanel);
    m_mainLayout->addWidget(m_userRunnubleApplicationPanel);

    setLayout(m_mainLayout);
}

void UserFilesEditPanel::createConnections()
{
    connect(this, &UserFilesEditPanel::setDefaultRoleApps, m_userDesktopPanel, &DesktopPanel::setDefaultRoleApps);
    connect(this, &UserFilesEditPanel::setDefaultRoleApps, m_userRunnubleApplicationPanel, &StartupPanel::setDefaultRoleApps);
}
