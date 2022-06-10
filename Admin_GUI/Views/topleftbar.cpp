#include "topleftbar.h"
#include <QDebug>

TopLeftBar::TopLeftBar(QWidget *parent)
    : QWidget(parent)
{
    CreateUI();
    InsertWidgetsIntoLayouts();
    InitUI();
}

TopLeftBar::~TopLeftBar()
{

    delete m_mainLayout;

    delete m_currentUserNameLabel;
    delete m_currentUseRoleLabel;
}

void TopLeftBar::SetData(const QString &FCS, int role)
{
    m_currentUserNameLabel->setText("Добро пожаловать товарищ: " + FCS);
    m_currentUseRoleLabel->setText("Ваша роль:  " + Roles.at(role));
}

void TopLeftBar::CreateUI()
{
    m_mainLayout = new QVBoxLayout();
    m_currentUserNameLabel = new QLabel();
    m_currentUseRoleLabel = new QLabel();
}

void TopLeftBar::InsertWidgetsIntoLayouts()
{
    m_mainLayout->addWidget(m_currentUserNameLabel);
    m_mainLayout->addWidget(m_currentUseRoleLabel);
    m_mainLayout->setMargin(1);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
}

void TopLeftBar::InitUI()
{
    m_oldFontSize = IsNeedToUpdateFontSize(this->width());
    m_titleFont = QFont(m_currentUserNameLabel->font());
    m_currentUserNameLabel->setFont(m_titleFont);
    m_currentUseRoleLabel->setFont(m_titleFont);
}


bool TopLeftBar::IsNeedToUpdateFontSize(int width)
{
    if (width > 1890)
    {
        if (m_oldFontSize != 20)
        {
            m_oldFontSize = 20;
            return true;
        }
    }
    else if (width > 1785)
    {
        if (m_oldFontSize != 19)
        {
            m_oldFontSize = 19;
            return true;
        }
    }
    else if (width > 1730)
    {
        if (m_oldFontSize != 18)
        {
            m_oldFontSize = 18;
            return true;
        }
    }
    else if (width > 1716)
    {
        if (m_oldFontSize != 17)
        {
            m_oldFontSize = 17;
            return true;
        }
    }
    else if (width > 1545)
    {
        if (m_oldFontSize != 16)
        {
            m_oldFontSize = 16;
            return true;
        }

    }
    else if (width > 1470)
    {
        if (m_oldFontSize != 15)
        {
            m_oldFontSize = 15;
            return true;
        }
    }
    else if (width > 1430)
    {
        if (m_oldFontSize != 14)
        {
            m_oldFontSize = 14;
            return true;
        }
    }
    else if (width > 1330)
    {
        if (m_oldFontSize != 13)
        {
            m_oldFontSize = 13;
            return true;
        }
    }
    else if (width > 1235)
    {
        if (m_oldFontSize != 12)
        {
            m_oldFontSize = 12;
            return true;
        }
    }
    else if (width > 1190)
    {
        if (m_oldFontSize != 11)
        {
            m_oldFontSize = 11;
            return true;
        }
    }
    else if (width > 1080)
    {
        if (m_oldFontSize != 10)
        {
            m_oldFontSize = 10;
            return true;
        }
    }
    else if (width > 990)
    {
        if (m_oldFontSize != 9)
        {
            m_oldFontSize = 9;
            return true;
        }
    }
    else
    {
        if (width > 903)
        {
            m_oldFontSize = 8;
            return true;
        }
    }

    return false;
}

void TopLeftBar::UpdateFontSize()
{
    m_titleFont.setPointSize(m_oldFontSize);
    m_currentUserNameLabel->setFont(m_titleFont);
    m_currentUseRoleLabel->setFont(m_titleFont);
}

void TopLeftBar::ChangeWidgetLayout()
{

}

void TopLeftBar::resizeEvent(QResizeEvent *event)
{
    qDebug() << "topBarSize" << event->size().width() << " fontSize " << m_oldFontSize;

    if (IsNeedToUpdateFontSize(event->size().width()))
    {
        UpdateFontSize();
    }
}
