#include "topbar.h"
#include <QDebug>

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
    delete m_titleFont;
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
    m_titleFont=new QFont(m_currentUserNameLabel->font());
    m_currentTimeLabel=new QLabel();
    m_currentUseRoleLabel=new QLabel();
    m_currentUserNameLabel->setFont(*m_titleFont);
    m_currentTimeLabel->setFont(*m_titleFont);
    m_currentUseRoleLabel->setFont(*m_titleFont);
    m_settingsPushButtton=new QPushButton();
    m_settingsPushButtton->setCheckable(true);
    m_settingsPushButtton->setChecked(false);
    m_settingsPushButtton->setFlat(true);
    m_settingsPushButtton->setIcon(QIcon(":/images/settings2"));
    m_settingsPushButtton->setIconSize(QSize(30, 30));
    m_themePushButtton=new QPushButton();
    m_themePushButtton->setCheckable(true);
    m_themePushButtton->setChecked(false);
    m_themePushButtton->setFlat(true);
    m_themePushButtton->setIcon(QIcon(":/images/moon"));
    m_themePushButtton->setIconSize(QSize(30, 30));
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
    m_userInfoLayout->addWidget(m_themePushButtton);

    m_userInfoLayout->setAlignment(m_settingsPushButtton, Qt::AlignLeft);
    setLayout(m_userInfoLayout);
}

void TopBar::createConnections()
{
    connect(m_timer, &QTimer::timeout, this, &TopBar::refreshDateTime);
    connect(m_settingsPushButtton, &QPushButton::clicked, this, &TopBar::onSettingsButtonClick);
    connect(m_themePushButtton, &QPushButton::clicked, this, &TopBar::onThemeButtonClick);
}

void TopBar::onSettingsButtonClick(bool state)
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
    emit hideAdditionalSettings(state);
}

void TopBar::onThemeButtonClick(bool state)
{
    if (state)
    {
        m_themePushButtton->setIcon(QIcon(":/images/sun"));
        m_themePushButtton->setIconSize(QSize(30, 30));
    }
    else
    {
        m_themePushButtton->setIcon(QIcon(":/images/moon"));
        m_themePushButtton->setIconSize(QSize(30, 30));
    }
    //    changeTextSize(state);
    emit setTheme(state);
}

void TopBar::refreshDateTime()
{
    m_currentTimeLabel->setText(m_dateTime->currentDateTime().toString("Дата: dddd dd MMMM yyyy г.  Время: hh:mm:ss"));
}



void TopBar::calculateFontSize(int width)
{
    if (width>1870)
    {
        m_titleFont->setPointSize(20);
    }
    else
        if (width>1760)
        {
            m_titleFont->setPointSize(19);
        }
        else
            if (width>1720)
            {
                m_titleFont->setPointSize(18);
            }
            else
                if (width>1640)
                {
                    m_titleFont->setPointSize(17);
                }
                else
                    if (width>1530)
                    {
                        m_titleFont->setPointSize(16);
                    }
                    else
                        if (width>1450)
                        {
                            m_titleFont->setPointSize(15);
                        }
                        else
                            if (width>1410)
                            {
                                m_titleFont->setPointSize(14);
                            }
                            else
                                if (width>1330)
                                {
                                    m_titleFont->setPointSize(13);
                                }
                                else
                                    if (width>1220)
                                    {
                                        m_titleFont->setPointSize(12);
                                    }
                                    else
                                        if (width>1160)
                                        {
                                            m_titleFont->setPointSize(11);
                                        }
                                        else
                                            if (width>1080)
                                            {
                                                m_titleFont->setPointSize(10);
                                            }
                                            else
                                                if (width>980)
                                                {
                                                    m_titleFont->setPointSize(9);
                                                }
                                                else
                                                {
                                                    m_titleFont->setPointSize(8);
                                                }
}

void TopBar::resizeEvent(QResizeEvent *event)
{
    calculateFontSize(event->size().width());
    m_currentUserNameLabel->setFont(*m_titleFont);
    m_currentTimeLabel->setFont(*m_titleFont);
    m_currentUseRoleLabel->setFont(*m_titleFont);

}
