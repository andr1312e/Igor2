#include "settingspanel.h"
#include <QDebug>


SettingsPanel::SettingsPanel(const QString &userName, UserModel *model, Terminal *terminal, QWidget *parent)
    : QWidget(parent)
    , m_oldFontSize(0)
{
    initUI(userName, model, terminal);
    insertWidgetsIntoLayout();
    createConnections();
}

SettingsPanel::~SettingsPanel()
{
    delete m_mainLayout;

    delete m_userInfoPanel;
    delete m_userEditPanel;
}

void SettingsPanel::initUI(const QString &userName, UserModel *model, Terminal *terminal)
{

    m_mainLayout=new QVBoxLayout();
    m_userInfoPanel=new UserInfoPanel(terminal, this);
    m_userEditPanel=new UserEditPanel(userName, model, terminal, this);
}

void SettingsPanel::applyStyle()
{

}

void SettingsPanel::insertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_userInfoPanel);
    m_mainLayout->addWidget(m_userEditPanel);
    setLayout(m_mainLayout);
}

void SettingsPanel::createConnections()
{
    connect(m_userEditPanel, &UserEditPanel::clearUserAdditionalInfo, m_userInfoPanel, &UserInfoPanel::clearAdditionalInfo);
    connect(m_userEditPanel, &UserEditPanel::fillUserAdditionalInfo, m_userInfoPanel, &UserInfoPanel::fillUserAdditionalInfo);
    connect(m_userEditPanel, &UserEditPanel::setDefaultRoleApps, this, &SettingsPanel::setDefaultRoleApps);
    connect(m_userEditPanel, &UserEditPanel::roleToViewChanged, this, &SettingsPanel::roleToViewChanged);
}

void SettingsPanel::setUser(User &user)
{
    m_userInfoPanel->setUser(user);
    m_userEditPanel->setUser(user);
}

void SettingsPanel::resizeEvent(QResizeEvent *event)
{
    int width=event->size().width();
    if (width>900)
    {
        if (m_oldFontSize!=20)
        {
            m_oldFontSize=20;
            m_userInfoPanel->setFontSize(m_oldFontSize);
            m_userEditPanel->setFontSize(m_oldFontSize);
            m_userEditPanel->setButtonSize(44);
        }
    }
    else
    {
        if (width>861)
        {
            if (m_oldFontSize!=19)
            {
                m_oldFontSize=19;
                m_userInfoPanel->setFontSize(m_oldFontSize);
                m_userEditPanel->setFontSize(m_oldFontSize);
                m_userEditPanel->setButtonSize(42);
            }
        }
        else
        {
            if(width>845)
            {
                if (m_oldFontSize!=18)
                {
                    m_oldFontSize=18;
                    m_userInfoPanel->setFontSize(m_oldFontSize);
                    m_userEditPanel->setFontSize(m_oldFontSize);
                    m_userEditPanel->setButtonSize(40);
                }
            }
            else
            {
                if(width>810)
                {
                    if (m_oldFontSize!=17)
                    {
                        m_oldFontSize=17;
                        m_userInfoPanel->setFontSize(m_oldFontSize);
                        m_userEditPanel->setFontSize(m_oldFontSize);
                        m_userEditPanel->setButtonSize(38);
                    }
                }
                else
                {
                    if(width>726)
                    {
                        if (m_oldFontSize!=16)
                        {
                            m_oldFontSize=16;
                            m_userInfoPanel->setFontSize(m_oldFontSize);
                            m_userEditPanel->setFontSize(m_oldFontSize);
                            m_userEditPanel->setButtonSize(36);
                        }
                    }
                    else
                    {
                        if(width>700)
                        {
                            if (m_oldFontSize!=15)
                            {
                                m_oldFontSize=15;
                                m_userInfoPanel->setFontSize(m_oldFontSize);
                                m_userEditPanel->setFontSize(m_oldFontSize);
                                m_userEditPanel->setButtonSize(34);
                            }
                        }
                        else
                        {
                            if(width>666)
                            {
                                if (m_oldFontSize!=14)
                                {
                                    m_oldFontSize=14;
                                    m_userInfoPanel->setFontSize(m_oldFontSize);
                                    m_userEditPanel->setFontSize(m_oldFontSize);
                                    m_userEditPanel->setButtonSize(33);
                                }
                            }
                            else
                            {
                                if(width>600)
                                {
                                    if (m_oldFontSize!=13)
                                    {
                                        m_oldFontSize=13;
                                        m_userInfoPanel->setFontSize(m_oldFontSize);
                                        m_userEditPanel->setFontSize(m_oldFontSize);
                                        m_userEditPanel->setButtonSize(32);
                                    }
                                }
                                else
                                {
                                    if(width>565)
                                    {
                                        if (m_oldFontSize!=12)
                                        {
                                            m_oldFontSize=12;
                                            m_userInfoPanel->setFontSize(m_oldFontSize);
                                            m_userEditPanel->setFontSize(m_oldFontSize);
                                            m_userEditPanel->setButtonSize(31);
                                        }
                                    }
                                    else
                                    {
                                        if(width>550)
                                        {
                                            if (m_oldFontSize!=11)
                                            {
                                                m_oldFontSize=11;
                                                m_userInfoPanel->setFontSize(m_oldFontSize);
                                                m_userEditPanel->setFontSize(m_oldFontSize);
                                                m_userEditPanel->setButtonSize(30);
                                            }
                                        }
                                        else
                                        {
                                            if (m_oldFontSize!=10)
                                            {
                                                m_oldFontSize=10;
                                                m_userInfoPanel->setFontSize(m_oldFontSize);
                                                m_userEditPanel->setFontSize(m_oldFontSize);
                                                m_userEditPanel->setButtonSize(29);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

