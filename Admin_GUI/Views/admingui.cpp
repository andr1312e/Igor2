#include "admingui.h"

#include <QFontDatabase>

Admin_GUI::Admin_GUI(DatabaseService *databaseService, LinuxUserService *userService, QWidget *parent)
    :  QWidget(parent)
    ,  m_databaseService(databaseService)
    ,  m_linuxUserService(userService)
{
    this->setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    initUI();
    applyStyle();
    insertWidgetsIntoLayout();
    createConnections();
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


    m_mainLayout=new QVBoxLayout();
    m_programLayout=new QHBoxLayout();

    initTopBar();

    m_userModel=new UserModel(m_linuxUserService, m_databaseService);

    m_linuxUsersListWidget=new LinuxUsersListWidget(m_userModel, this);
    m_linuxUsersListWidget->setMaximumWidth(350);
    m_settingsPanel=new SettingsPanel(m_linuxUserService->getCurrentUserName(), m_databaseService, this);
    m_settingsPanel->setMaximumWidth(450);
    m_additionalSettingsPanel=new AdditionalSettingsPanel(m_linuxUserService->getTerminal(), this);
}

void Admin_GUI::applyStyle()
{
    setStyleSheets();
    QPalette palette;
    palette.setColor(this->backgroundRole(), QColor(105,101,110));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

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
}

void Admin_GUI::setStyleSheets()
{
    QFile m_styleFile(":/Styles/style.qss");
    m_styleFile.open(QFile::ReadOnly);
    QString styleSheet=QString(m_styleFile.readAll());
    m_styleFile.close();
    this->setStyleSheet(styleSheet);
}

void Admin_GUI::roleToViewChanged(const QString &role)
{
    QStringList *users=m_userModel->getUserNamesByRole(role);
    m_additionalSettingsPanel->setRoleEditPanelData(role, users);
}

void Admin_GUI::onLinuxUserClick(User &user)
{
    m_settingsPanel->setUser(user);
    m_additionalSettingsPanel->setUserFilesEditPanelData(user.name, user.role);
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
    if (state)
        m_additionalSettingsPanel->hide();
    else
        m_additionalSettingsPanel->show();
    this->showNormal();
}
