#include "fcspage.h"

FCSPage::FCSPage(CurrentUserWizardRepository *userWizardRepository, QDomDocument *backup, QWidget *parent)
    : QWizardPage(parent)
    , m_userWizardRepository(userWizardRepository)
    , m_currentUserName(m_userWizardRepository->getCurrentUserName())
    , m_backup(backup)
{
    initUI();
    insertWidgetsIntoLayout();
}

void FCSPage::setUserFCSAndRole(QString &FCS, QString &role)
{
    m_FCSlineEdit->setText(FCS);
    m_RankComboBox->setCurrentText(role);
}

FCSPage::~FCSPage()
{
    delete m_mainLayout;

    delete m_topLabel;
    delete m_FCSLabel;
    delete m_FCSlineEdit;
    delete m_RankLabel;
    delete m_RankComboBox;
}

int FCSPage::nextId() const
{
    QString fcs=m_FCSlineEdit->text();
    QString rank=m_RankComboBox->currentText();
    Q_EMIT sendUpdatedDataToParentWizard(fcs, rank);
    return 0;
}

void FCSPage::initializePage()
{
    QDomElement users=m_backup->firstChildElement().firstChildElement();
    const QDomNodeList usersList=users.childNodes();
    for (int i = 0; i < usersList.count(); i++)
    {
        QDomElement elem=usersList.at(i).toElement();
        QString userName=elem.attribute("name");
        if (userName==m_currentUserName)
        {
            m_FCSValue=elem.attribute("FCS");
            m_RankValue=elem.attribute("rank");
            return;
        }
    }
    createNewUser();
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

void FCSPage::createNewUser()
{
    QDomElement newElem=m_backup->createElement("user");
    newElem.setAttribute("userId", m_userWizardRepository->getCurrentUserId());
    newElem.setAttribute("name", m_currentUserName);
    newElem.setAttribute("rank", m_RankValue);
    newElem.setAttribute("role", "Администратор");
    newElem.setAttribute("FCS", m_FCSValue);
}