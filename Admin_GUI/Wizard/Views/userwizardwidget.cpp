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
    delete m_stringValidator;
    delete m_userFCSLayout;
    delete m_numOfUsersLayout;
    delete m_mainLayout;

    delete m_topLabel;
    delete m_fcsLabel;
    delete m_fcsLineEdit;
    delete m_numOfUsersLabel;
    delete m_numOfUsersValue;
    delete m_accountsData;
}

void UserWizardWidget::InitUI()
{
    m_stringValidator = new StringValidator(this);
    m_mainLayout = new QVBoxLayout();
    m_topLabel = new QLabel(QStringLiteral("Поле ввода данных администратора:"));

    m_userFCSLayout = new QHBoxLayout();
    m_fcsLabel = new QLabel(QStringLiteral("ФИО администратора:"));
    m_fcsLineEdit = new QLineEdit();
    m_fcsLineEdit->setObjectName(QStringLiteral("wizard"));
    m_fcsLineEdit->setValidator(m_stringValidator);

    m_numOfUsersLayout = new QHBoxLayout();
    m_numOfUsersLabel = new QLabel(QStringLiteral("Колличество пользователей:"));
    m_numOfUsersValue = new QLabel();

    m_accountsData = new QTableWidget();


    m_accountsData->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void UserWizardWidget::InsertWidgetsIntoLayout()
{
    m_userFCSLayout->addWidget(m_fcsLabel);
    m_userFCSLayout->addWidget(m_fcsLineEdit);

    m_numOfUsersLayout->addWidget(m_numOfUsersLabel);
    m_numOfUsersLayout->addWidget(m_numOfUsersValue);

    m_mainLayout->addWidget(m_topLabel);
    m_mainLayout->addLayout(m_userFCSLayout);
    m_mainLayout->addLayout(m_numOfUsersLayout);
    m_mainLayout->addWidget(m_accountsData);
    setLayout(m_mainLayout);
}

void UserWizardWidget::FillWidget(const QString &title,const QString &FCS,const QList<User> &users)
{
    m_accountsData->setAutoFillBackground(true);

    m_topLabel->setText(title);
    m_fcsLineEdit->setText(FCS);

    if (users.isEmpty()) {
        m_numOfUsersValue->setText(QStringLiteral("Пользователей нет..."));
    } else {
        m_numOfUsersValue->setText(QString::number(users.count()));
        m_accountsData->clear();
        m_accountsData->setRowCount(users.count());
        m_accountsData->setColumnCount(5);
        m_accountsData->setHorizontalHeaderLabels(m_headerLabels);
        m_accountsData->setColumnWidth(1, 180);
        m_accountsData->setColumnWidth(2, 280);
        m_accountsData->horizontalHeader()->setStretchLastSection(true);

        int row=0;
        for (const User &user :users) {
            m_accountsData->setItem(row, 0, new QTableWidgetItem(user.userId));
            m_accountsData->setItem(row, 1, new QTableWidgetItem(user.name));
            m_accountsData->setItem(row, 2, new QTableWidgetItem(user.FCS));
            m_accountsData->setItem(row, 4, new QTableWidgetItem(Roles.at(user.role)));
            ++row;
        }
    }
}

QString UserWizardWidget::GetUserFCS() const
{
    return m_fcsLineEdit->text();
}


