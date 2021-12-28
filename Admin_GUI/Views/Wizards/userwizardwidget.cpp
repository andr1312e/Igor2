#include "userwizardwidget.h"

UserWizardWidget::UserWizardWidget(const QStringList &headerLabels, QWidget *parent)
    : QWidget(parent)
    , m_headerLabels(headerLabels)
{
    InitUI();
    InsertWidgetsIntoLayout();
}

UserWizardWidget::~UserWizardWidget()
{
    delete m_mainLayout;
    delete m_userFCSLayout;
    delete m_userRankLayout;
    delete m_numOfUsersLayout;

    delete m_topLabel;
    delete m_fcsLabel;
    delete m_fcsLineEdit;
    delete m_backupRankLabel;
    delete m_rankComboBox;
    delete m_numOfUsersLabel;
    delete m_numOfUsersValue;
    delete m_accountsData;
}

void UserWizardWidget::InitUI()
{
    m_stringValidator = new StringValidator(nullptr);
    m_mainLayout = new QVBoxLayout();
    m_topLabel = new QLabel("Поле ввода данных администратора:");

    m_userFCSLayout = new QHBoxLayout();
    m_fcsLabel = new QLabel("ФИО администратора:");
    m_fcsLineEdit = new QLineEdit();
    m_fcsLineEdit->setObjectName("wizard");
    m_fcsLineEdit->setValidator(m_stringValidator);

    m_userRankLayout = new QHBoxLayout();
    m_backupRankLabel = new QLabel("Звание администратора:");
    m_rankComboBox = new QComboBox();
    m_rankComboBox->addItems(Ranks);

    m_numOfUsersLayout = new QHBoxLayout();
    m_numOfUsersLabel = new QLabel("Колличество пользователей:");
    m_numOfUsersValue = new QLabel();

    m_accountsData = new QTableWidget();


    m_accountsData->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void UserWizardWidget::InsertWidgetsIntoLayout()
{
    m_userFCSLayout->addWidget(m_fcsLabel);
    m_userFCSLayout->addWidget(m_fcsLineEdit);

    m_userRankLayout->addWidget(m_backupRankLabel);
    m_userRankLayout->addWidget(m_rankComboBox);

    m_numOfUsersLayout->addWidget(m_numOfUsersLabel);
    m_numOfUsersLayout->addWidget(m_numOfUsersValue);

    m_mainLayout->addWidget(m_topLabel);
    m_mainLayout->addLayout(m_userFCSLayout);
    m_mainLayout->addLayout(m_userRankLayout);
    m_mainLayout->addLayout(m_numOfUsersLayout);
    m_mainLayout->addWidget(m_accountsData);
    setLayout(m_mainLayout);
}

void UserWizardWidget::setWizardWidgetFileds(const QString &title, QString &FCS, QString &rank, QList<User> &users)
{
    m_accountsData->setAutoFillBackground(true);

    m_topLabel->setText(title);
    m_fcsLineEdit->setText(FCS);
    m_rankComboBox->setCurrentText(rank);

    if (users.isEmpty()) {
        m_numOfUsersValue->setText("Пользователей нет...");
    } else {
        m_numOfUsersValue->setText(QString::number(users.count()));
        m_accountsData->clear();
        m_accountsData->setRowCount(users.count());
        m_accountsData->setColumnCount(5);
        m_accountsData->setHorizontalHeaderLabels(m_headerLabels);
        m_accountsData->setColumnWidth(1, 180);
        m_accountsData->setColumnWidth(2, 280);
        m_accountsData->horizontalHeader()->setStretchLastSection(true);

        for (int r = 0; r < users.count(); r++) {
            m_accountsData->setItem(r, 0, new QTableWidgetItem(users.at(r).userId));
            m_accountsData->setItem(r, 1, new QTableWidgetItem(users.at(r).name));
            m_accountsData->setItem(r, 2, new QTableWidgetItem(users.at(r).FCS));
            m_accountsData->setItem(r, 3, new QTableWidgetItem(users.at(r).rank));
            m_accountsData->setItem(r, 4, new QTableWidgetItem(Roles.at(users.at(r).role)));
        }
    }
}

QString UserWizardWidget::getUserChoise()
{
    return m_rankComboBox->currentText();
}


