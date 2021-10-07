#include "topbar.h"
#include <QDebug>

TopBar::TopBar(QWidget *parent)
    : QWidget(parent)
    , m_oldFontSize(0)
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
    delete m_iconsSize;

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

    m_iconsSize=new QSize(30, 30);
    m_settingsPushButtton=new QPushButton();
    m_settingsPushButtton->setCheckable(true);
    m_settingsPushButtton->setChecked(false);
    m_settingsPushButtton->setFlat(true);
    m_settingsPushButtton->setIcon(QIcon(":/images/settings2"));
    m_settingsPushButtton->setIconSize(*m_iconsSize);
    m_themePushButtton=new QPushButton();
    m_themePushButtton->setCheckable(true);
    m_themePushButtton->setChecked(false);
    m_themePushButtton->setFlat(true);
    m_themePushButtton->setIcon(QIcon(":/images/moon"));
    m_themePushButtton->setIconSize(*m_iconsSize);
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
    }
    else
    {
        m_settingsPushButtton->setIcon(QIcon(":/images/settings"));
    }
    m_settingsPushButtton->setIconSize(*m_iconsSize);
    emit hideAdditionalSettings(state);
}

void TopBar::onThemeButtonClick(bool state)
{
    if (state)
    {
        m_themePushButtton->setIcon(QIcon(":/images/sun"));
    }
    else
    {
        m_themePushButtton->setIcon(QIcon(":/images/moon"));
    }
    m_themePushButtton->setIconSize(*m_iconsSize);
    emit setTheme(state);
}

void TopBar::refreshDateTime()
{
    m_currentTimeLabel->setText(m_dateTime->currentDateTime().toString("Дата: dddd dd MMMM yyyy г.  Время: hh:mm:ss"));
}

bool TopBar::isNeedToUpdateFontSize(int width)
{
    if (width>1890)
    {
        if (m_oldFontSize!=20)
        {
            m_oldFontSize=20;
            return true;
        }
    }
    else
        if (width>1785)
        {
            if (m_oldFontSize!=19)
            {
                m_oldFontSize=19;
                return true;
            }
        }
        else
            if (width>1730)
            {
                if (m_oldFontSize!=18)
                {
                    m_oldFontSize=18;
                    return true;
                }
            }
            else
                if (width>1716)
                {
                    if (m_oldFontSize!=17)
                    {
                        m_oldFontSize=17;
                        return true;
                    }
                }
                else
                    if (width>1545)
                    {
                        if (m_oldFontSize!=16)
                        {
                            m_oldFontSize=16;
                            return true;
                        }
                    }
                    else
                        if (width>1470)
                        {
                            if (m_oldFontSize!=15)
                            {
                                m_oldFontSize=15;
                                return true;
                            }
                        }
                        else
                            if (width>1430)
                            {
                                if (m_oldFontSize!=14)
                                {
                                    m_oldFontSize=14;
                                    return true;
                                }
                            }
                            else
                                if (width>1330)
                                {
                                    if (m_oldFontSize!=13)
                                    {
                                        m_oldFontSize=13;
                                        return true;
                                    }
                                }
                                else
                                    if (width>1235)
                                    {
                                        if (m_oldFontSize!=12)
                                        {
                                            m_oldFontSize=12;
                                            return true;
                                        }
                                    }
                                    else
                                        if (width>1190)
                                        {
                                            if (m_oldFontSize!=11)
                                            {
                                                m_oldFontSize=11;
                                                return true;
                                            }
                                        }
                                        else
                                            if (width>1080)
                                            {
                                                if (m_oldFontSize!=10)
                                                {
                                                    m_oldFontSize=10;
                                                    return true;
                                                }
                                            }
                                            else
                                                if (width>990)
                                                {
                                                    if (m_oldFontSize!=9)
                                                    {
                                                        m_oldFontSize=9;
                                                        return true;
                                                    }
                                                }
                                                else
                                                {
                                                    if (m_oldFontSize!=8)
                                                    {
                                                        m_oldFontSize=8;
                                                        return true;
                                                    }
                                                }
    return false;
}

void TopBar::updateFontSize()
{
    m_titleFont->setPointSize(m_oldFontSize);
    m_currentUserNameLabel->setFont(*m_titleFont);
    m_currentTimeLabel->setFont(*m_titleFont);
    m_currentUseRoleLabel->setFont(*m_titleFont);
}

void TopBar::resizeEvent(QResizeEvent *event)
{
    if (isNeedToUpdateFontSize(event->size().width()))
        updateFontSize();
}
