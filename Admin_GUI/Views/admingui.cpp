#include "admingui.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

Admin_GUI::Admin_GUI(DatabaseService *databaseService, LinuxUserService *userService, QWidget *parent)
    :  QWidget(parent)
    ,  m_databaseService(databaseService)
    ,  m_linuxUserService(userService)
{
    initUI();
    setWidgetSizes();
    insertWidgetsIntoLayout();
    createConnections();
    setMaximumWidgetSize();
}

Admin_GUI::~Admin_GUI()
{
    delete m_userModel;

    delete m_programLayout;
    delete m_mainLayout;

    delete m_topBar;
    delete m_linuxUsersListWidget;
    delete m_settingsPanel;
    delete m_additionalSettingsPanel;
}

void Admin_GUI::initUI()
{
    m_userModel=new UserModel(m_linuxUserService, m_databaseService, nullptr);

    m_mainLayout=new QVBoxLayout();
    m_programLayout=new QHBoxLayout();

    initTopBar();

    m_linuxUsersListWidget=new LinuxUsersListWidget(m_userModel, this);

    m_settingsPanel=new SettingsPanel(m_linuxUserService->getCurrentUserName(), m_userModel, m_databaseService->getTerminal(), this);

    m_additionalSettingsPanel=new AdditionalSettingsPanel(m_linuxUserService->getTerminal(), this);
}

void Admin_GUI::setWidgetSizes()
{
    m_linuxUsersListWidget->setMaximumWidth(336);
    m_settingsPanel->setMaximumWidth(450);
}

void Admin_GUI::insertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_topBar);

    m_programLayout->addWidget(m_linuxUsersListWidget);
    m_programLayout->addWidget(m_settingsPanel);
    m_programLayout->addWidget(m_additionalSettingsPanel);

    m_mainLayout->addLayout(m_programLayout);
    setLayout(m_mainLayout);
}

void Admin_GUI::createConnections()
{
    connect(m_linuxUsersListWidget, &LinuxUsersListWidget::onUserClick, this, &Admin_GUI::onLinuxUserClick);
    connect(m_settingsPanel, &SettingsPanel::setDefaultRoleApps, m_additionalSettingsPanel, &AdditionalSettingsPanel::setDefaultRoleApps);
    connect(m_settingsPanel, &SettingsPanel::roleToViewChanged, this, &Admin_GUI::roleToViewChanged);
    connect(m_topBar, &TopBar::hideAdditionalSettings, this, &Admin_GUI::hideAdditionalSettings);
    connect(m_topBar, &TopBar::setTheme, this, &Admin_GUI::setTheme);
}

void Admin_GUI::setMaximumWidgetSize()
{
    m_currentScreen = QGuiApplication::screenAt(mapToGlobal({width()/2,0}));
    m_maxWidth=m_currentScreen->availableSize().width();
    m_maxHeight=m_currentScreen->availableSize().height()-30;
    this->setMaximumWidth(m_maxWidth);
    this->setMaximumHeight(m_maxHeight);
}

void Admin_GUI::initTopBar()
{
    m_topBar=new TopBar(this);
    QString currentUserLinuxUserName=m_linuxUserService->getCurrentUserName();
    QString userFCS=m_databaseService->getUserAttributeByLinuxUserNameToList(currentUserLinuxUserName, "FCS");
    QString userRank=m_databaseService->getUserAttributeByLinuxUserNameToList(currentUserLinuxUserName, "rank");
    QString userRole=m_databaseService->getUserAttributeByLinuxUserNameToList(currentUserLinuxUserName, "role");
    m_topBar->setData(userRank, userFCS, userRole);
}

void Admin_GUI::hideAdditionalSettings(bool state)
{
    QScreen* pScreen = QGuiApplication::screenAt(mapToGlobal({width()/2,0}));
    if (m_currentScreen!=pScreen)
    {
        setMaximumWidgetSize();
    }
    if (state)
    {
        m_additionalSettingsPanel->hide();
        m_linuxUsersListWidget->setMaximumWidth(width()/2);
        m_settingsPanel->setMaximumWidth(width()/2);
        this->setMinimumSize(m_linuxUsersListWidget->minimumWidth()+m_settingsPanel->minimumWidth(), 600);
    }
    else
    {
        if(m_maxWidth>1713)
        {
            m_linuxUsersListWidget->setFixedWidth(350);
            m_settingsPanel->setFixedWidth(450);
            this->setMinimumSize(1700, 600);
        }
        else
        {
            m_linuxUsersListWidget->setFixedWidth(250);
            m_settingsPanel->setFixedWidth(350);
            this->setMinimumSize(500, 600);
        }
//        this->setFixedSize(this->size());
        this->setMaximumSize(m_maxWidth, m_maxHeight);
        m_additionalSettingsPanel->show();

    }
}

void Admin_GUI::roleToViewChanged(const QString &role)
{
    QStringList *users=m_userModel->getUsersNamesByRole(role);
    m_additionalSettingsPanel->setRoleEditPanelData(role, users);
}

void Admin_GUI::onLinuxUserClick(User &user)
{
    m_settingsPanel->setUser(user);
    m_additionalSettingsPanel->setUserFilesEditPanelData(user.name, user.role);
}
