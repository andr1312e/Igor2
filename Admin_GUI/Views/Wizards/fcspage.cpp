#include "fcspage.h"

FCSPage::FCSPage(WizardService *service, QWidget *parent)
    : QWizardPage(parent)
    , m_wizardService(service)
    , m_oldDataUsageValue(false)
{
    initUI();
    insertWidgetsIntoLayout();
    createConnections();
}

void FCSPage::setUserFCSAndRole(bool isOld, QString &FCS, QString &rank, int &numOfUsers)
{
    if (isOld)
    {
        m_oldUserFCSLineEdit->setText(FCS);
        m_oldUserRankComboBox->setCurrentText(rank);
        m_oldNumOfUsersValue->setText(QString::number(numOfUsers));
    }
    else
    {
        m_backupFCSLineEdit->setText(FCS);
        m_backupRankComboBox->setCurrentText(rank);
        m_backupNumOfUsersValue->setText(QString::number(numOfUsers));
    }
}

FCSPage::~FCSPage()
{
    delete m_stringValidator;

    delete m_mainLayout;
    delete m_userFCSLayout;
    delete m_userRankLayout;
    delete m_numOfUsersLayout;
    delete m_oldUserFCSLayout;
    delete m_oldUserRankLayout;
    delete m_oldNumOfUsersLayout;

    delete m_topLabel;
    delete m_backupFCSLabel;
    delete m_backupFCSLineEdit;
    delete m_backupRankLabel;
    delete m_backupRankComboBox;
    delete m_backupNumOfUsersLabel;
    delete m_backupNumOfUsersValue;
    delete m_accountsData;

    delete m_oldUserDataLabel;
    delete m_oldUserFCSLabel;
    delete m_oldUserFCSLineEdit;
    delete m_oldUserRankLabel;
    delete m_oldUserRankComboBox;
    delete m_oldNumOfUsersLabel;
    delete m_oldNumOfUsersValue;
    delete m_oldDataUsageCheckBox;
}

int FCSPage::nextId() const
{
    return Page_Intro;
}

void FCSPage::initializePage()
{
    QString adminFCS, adminRank;
    int numOfUsers=0;
    if(m_wizardService->HasBackup())
    {
        m_topLabel->setText("Данные администратора хранящиеся в файле восстановления:");
        m_wizardService->getDataFromUserRepository(false, adminFCS, adminRank, numOfUsers);
        setUserFCSAndRole(false, adminFCS, adminRank, numOfUsers);
        if (m_wizardService->AdminAlreadyHasAnAccount())
        {
            setOldFieldsVisible(true);
            m_wizardService->getDataFromUserRepository(true, adminFCS, adminRank, numOfUsers);
            setUserFCSAndRole(true, adminFCS, adminRank, numOfUsers);
        }
        else
        {
            setOldFieldsVisible(false);
        }
    }
    else
    {
        if (m_wizardService->AdminAlreadyHasAnAccount())
        {
            m_topLabel=new QLabel("Данные администратора уже хранящиеся в локальной базе:");
            setOldFieldsVisible(false);
            m_wizardService->getDataFromUserRepository(true, adminFCS, adminRank, numOfUsers);
            setUserFCSAndRole(false, adminFCS, adminRank, numOfUsers);
        }
        else
        {
            m_topLabel->setText("Введите данные администратора: ФИО и Звание");
            setUserFCSAndRole(false, adminFCS, adminRank, numOfUsers);
        }
        setOldFieldsVisible(false);
    }
}

bool FCSPage::isComplete() const
{
    qDebug()<<"fff";
    return !m_backupFCSLineEdit->text().remove(' ').isEmpty();
}

void FCSPage::initUI()
{
    m_stringValidator=new StringValidator(Q_NULLPTR);
    m_mainLayout=new QVBoxLayout();
    m_topLabel=new QLabel("Поле ввода данных администратора:");

    m_userFCSLayout=new QHBoxLayout();
    m_backupFCSLabel=new QLabel("ФИО администратора:");
    m_backupFCSLineEdit=new QLineEdit();
    m_backupFCSLineEdit->setValidator(m_stringValidator);

    m_userRankLayout=new QHBoxLayout();
    m_backupRankLabel=new QLabel("Звание администратора:");
    m_backupRankComboBox=new QComboBox();
    m_backupRankComboBox->addItems(Ranks);

    m_numOfUsersLayout=new QHBoxLayout();
    m_backupNumOfUsersLabel=new QLabel("Колличество пользователей:");
    m_backupNumOfUsersValue=new QLabel();

    m_accountsData=new QListWidget();

    m_oldUserDataLabel=new QLabel("Данные администратора хранящиеся в прошлой БД:");

    m_oldUserFCSLayout=new QHBoxLayout();
    m_oldUserFCSLabel=new QLabel("ФИО администратора:");
    m_oldUserFCSLineEdit=new QLineEdit();

    m_oldUserRankLayout=new QHBoxLayout();
    m_oldUserRankLabel=new QLabel("Звание администратора:");
    m_oldUserRankComboBox=new QComboBox();
    m_oldUserRankComboBox->addItems(Ranks);

    m_oldNumOfUsersLayout=new QHBoxLayout();
    m_oldNumOfUsersLabel=new QLabel("Колличество пользователей:");
    m_oldNumOfUsersValue=new QLabel();

    m_oldAccountsData=new QListWidget();

    m_oldDataUsageCheckBox=new QCheckBox("Использовать старые данные");
}

void FCSPage::insertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_topLabel);

    m_userFCSLayout->addWidget(m_backupFCSLabel);
    m_userFCSLayout->addWidget(m_backupFCSLineEdit);

    m_userRankLayout->addWidget(m_backupRankLabel);
    m_userRankLayout->addWidget(m_backupRankComboBox);

    m_numOfUsersLayout->addWidget(m_backupNumOfUsersLabel);
    m_numOfUsersLayout->addWidget(m_backupNumOfUsersValue);

    m_mainLayout->addLayout(m_userFCSLayout);
    m_mainLayout->addLayout(m_userRankLayout);
    m_mainLayout->addLayout(m_numOfUsersLayout);
    m_mainLayout->addWidget(m_accountsData);

    m_mainLayout->addWidget(m_oldUserDataLabel);

    m_oldUserFCSLayout->addWidget(m_oldUserFCSLabel);
    m_oldUserFCSLayout->addWidget(m_oldUserFCSLineEdit);

    m_oldUserRankLayout->addWidget(m_oldUserRankLabel);
    m_oldUserRankLayout->addWidget(m_oldUserRankComboBox);

    m_oldNumOfUsersLayout->addWidget(m_oldNumOfUsersLabel);
    m_oldNumOfUsersLayout->addWidget(m_oldNumOfUsersValue);

    m_mainLayout->addLayout(m_oldUserFCSLayout);
    m_mainLayout->addLayout(m_oldUserRankLayout);
    m_mainLayout->addLayout(m_oldNumOfUsersLayout);
    m_mainLayout->addWidget(m_oldAccountsData);

    m_mainLayout->addWidget(m_oldDataUsageCheckBox);

    setLayout(m_mainLayout);
}

void FCSPage::createConnections()
{
    if (m_wizardService->AdminAlreadyHasAnAccount())
    {
        connect(m_oldDataUsageCheckBox, &QCheckBox::stateChanged, [&](int state){m_oldDataUsageValue=state/2.0;});//https://doc.qt.io/qt-5/qt.html#CheckState-enum
    }
    connect(m_backupFCSLineEdit, &QLineEdit::textEdited, this, &QWizardPage::completeChanged);
}

void FCSPage::setOldFieldsVisible(bool state)
{
    m_oldUserDataLabel->setVisible(state);
    m_oldUserFCSLabel->setVisible(state);
    m_oldUserFCSLineEdit->setVisible(state);
    m_oldUserRankLabel->setVisible(state);
    m_oldUserRankComboBox->setVisible(state);
    m_oldNumOfUsersLabel->setVisible(state);
    m_oldDataUsageCheckBox->setVisible(state);
    m_oldAccountsData->setVisible(state);
}
