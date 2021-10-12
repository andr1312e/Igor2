#include "fcspage.h"

FCSPage::FCSPage(QWidget *parent)
    : QWizardPage(parent)
{
    initUI();
    insertWidgetsIntoLayout();
    createConnections();
}

FCSPage::~FCSPage()
{

}

int FCSPage::nextId() const
{
    return 0;
}

void FCSPage::initUI()
{
    m_mainLayout=new QVBoxLayout();
    m_topLabel=new QLabel("Поле ввода данных администратора:");
    m_FCSLabel=new QLabel("Фамилия Имя Отчество");
    m_FCSlineEdit=new QLineEdit();
    m_RankLabel=new QLabel("Звание пользователя");
    m_RankComboBox=new QComboBox();
}

void FCSPage::insertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_topLabel);
    m_mainLayout->addWidget(m_FCSLabel);
    m_mainLayout->addWidget(m_FCSlineEdit);
    m_mainLayout->addWidget(m_RankLabel);
    m_mainLayout->addWidget(m_RankComboBox);
    setLayout(m_mainLayout);
}

void FCSPage::createConnections()
{

}
