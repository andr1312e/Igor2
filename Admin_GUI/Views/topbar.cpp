#include "topbar.h"

TopBar::TopBar(QWidget *parent)
    : QWidget(parent)
    , m_dateTime(new QDateTime())
    , m_timer(new QTimer())
{
    initUI();
    applyStyle();
    insertWidgetsIntoLayouts();
    createConnections();
    m_timer->start(1000);
}

TopBar::~TopBar()
{
    delete m_timer;
    delete m_dateTime;

    delete m_userInfoLayout;

    delete m_currentUserNameLabel;
    delete m_currentTimeLabel;
    delete m_currentUseRoleLabel;
    delete m_settingsPushButtton;

}

void TopBar::setData(QString &rank, QString &FCS, QString &role)
{
    m_currentUserNameLabel->setText("Добро пожаловать товарищ:  "+ rank + " " + FCS);
    m_currentUseRoleLabel->setText("Ваша роль:  " + role);
}

void TopBar::initUI()
{

    m_userInfoLayout=new QHBoxLayout();
    m_userInfoLayout->setSpacing(40);
    m_userInfoLayout->setContentsMargins(10, 10, 10, 10);
    m_currentUserNameLabel=new QLabel();
    QFont font=m_currentUserNameLabel->font();
    font.setPointSize(m_bigFontSize);
    m_currentTimeLabel=new QLabel();
    m_currentUseRoleLabel=new QLabel();
    m_currentUserNameLabel->setFont(font);
    m_currentTimeLabel->setFont(font);
    m_currentUseRoleLabel->setFont(font);
    m_settingsPushButtton=new QPushButton();
    m_settingsPushButtton->setCheckable(true);
    m_settingsPushButtton->setChecked(false);
    m_settingsPushButtton->setFlat(true);
    m_settingsPushButtton->setIcon(QIcon(":/images/settings2"));
    m_settingsPushButtton->setIconSize(QSize(30, 30));
}

void TopBar::applyStyle()
{
    m_settingsPushButtton->setStyleSheet("border: 0px;");
    m_settingsPushButtton->setAutoFillBackground(true);
    QPalette pal = m_settingsPushButtton->palette();
    pal.setColor(QPalette::Button, QColor(Qt::transparent));
    m_settingsPushButtton->setPalette(pal);
}

void TopBar::insertWidgetsIntoLayouts()
{
    m_userInfoLayout->setAlignment(Qt::AlignHCenter);
    m_userInfoLayout->addWidget(m_currentUserNameLabel);
    m_userInfoLayout->addWidget(m_currentTimeLabel);
    m_userInfoLayout->addWidget(m_currentUseRoleLabel);
    m_userInfoLayout->addWidget(m_settingsPushButtton);

    m_userInfoLayout->setAlignment(m_settingsPushButtton, Qt::AlignLeft);
    setLayout(m_userInfoLayout);
}

void TopBar::createConnections()
{
    connect(m_timer, &QTimer::timeout, this, &TopBar::refreshDateTime);
    connect(m_settingsPushButtton, &QPushButton::clicked, this, &TopBar::onButtonClick);
}

void TopBar::onButtonClick(bool state)
{
    if (state)
    {
        m_settingsPushButtton->setIcon(QIcon(":/images/settings2"));
        m_settingsPushButtton->setIconSize(QSize(30, 30));
    }
    else
    {
        m_settingsPushButtton->setIcon(QIcon(":/images/settings"));
        m_settingsPushButtton->setIconSize(QSize(30, 30));
    }
    changeTextSize(state);
    emit hideAdditionalSettings(state);
}

void TopBar::refreshDateTime()
{
    m_currentTimeLabel->setText(m_dateTime->currentDateTime().toString("Дата: dddd dd MMMM yyyy г.  Время: hh:mm:ss"));
}

void TopBar::changeTextSize(bool state)
{
    QFont font=m_currentUserNameLabel->font();
    if (state)
    {
        font.setPointSize(m_smallFontSize);
    }
    else
    {
        font.setPointSize(m_bigFontSize);
    }
    m_currentUserNameLabel->setFont(font);
    m_currentTimeLabel->setFont(font);
    m_currentUseRoleLabel->setFont(font);
}
