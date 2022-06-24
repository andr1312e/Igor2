#include "userwizardwidget.h"

UserWizardWidget::UserWizardWidget(const QStringList &headerLabels, WizardService *wizardService, QWidget *parent)
    : QWidget(parent)
    , m_headerLabels(headerLabels)
    , m_wizardService(wizardService)
{
    InitUI();
    InsertWidgetsIntoLayout();
    ConnectObjects();
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
    m_fcsLineEdit->setObjectName(QLatin1Literal("wizard"));
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

    m_numOfUsersLayout->addWidget(m_numOfUsersLabel, 0, Qt::AlignLeft);
    m_numOfUsersLayout->addWidget(m_numOfUsersValue, 0, Qt::AlignRight);

    m_mainLayout->addWidget(m_topLabel);
    m_mainLayout->addLayout(m_userFCSLayout);
    m_mainLayout->addLayout(m_numOfUsersLayout);
    m_mainLayout->addWidget(m_accountsData);
    setLayout(m_mainLayout);
}

void UserWizardWidget::ConnectObjects()
{
    connect(m_fcsLineEdit, &QLineEdit::textEdited, this, &UserWizardWidget::ToAdminFCSUpdated);
}

void UserWizardWidget::UpdateFCS(int currentRow, const QString &fcs)
{
    if (Q_NULLPTR == m_wizardService)
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Нарушена логика"));
    }
    else
    {
        const QString userId = m_accountsData->item(currentRow, 0)->text();
        const QString userName = m_accountsData->item(currentRow, 1)->text();
        m_wizardService->UpdateBackupUserFCS(userId, userName, fcs);
    }
}

void UserWizardWidget::UpdateRoleIndex(int currentRow, int roleIndex)
{
    if (Q_NULLPTR == m_wizardService)
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Нарушена логика"));
    }
    else
    {
        const QString userId = m_accountsData->item(currentRow, 0)->text();
        const QString userName = m_accountsData->item(currentRow, 1)->text();
        m_wizardService->UpdateBackupUserFCS(userId, userName, roleIndex);
    }

}

QLineEdit *UserWizardWidget::CreateUserFCSField(const QString &userFcs)
{
    QLineEdit *const userFcsField = new QLineEdit();
    userFcsField->setText(userFcs);
    return userFcsField;
}

QComboBox *UserWizardWidget::CreateUserRoleCB(int roleIndex) const
{
    QComboBox *const roleComboBox = new QComboBox();
    roleComboBox->addItems(Roles);
    if (User::RoleIsValid(roleIndex))
    {
        roleComboBox->setCurrentIndex(roleIndex);
    }
    else
    {
        roleComboBox->setCurrentIndex(0);
    }
    return roleComboBox;
}

void UserWizardWidget::FillWidget(const QString &title, const QString &FCS, const QList<User> &users)
{

    m_accountsData->setAutoFillBackground(true);

    m_topLabel->setText(title);
    m_fcsLineEdit->setText(FCS);

    if (users.isEmpty())
    {
        m_numOfUsersValue->setText(QStringLiteral("Пользователей нет..."));
    }
    else
    {
        m_numOfUsersValue->setText(QString::number(users.count()));
        m_accountsData->clear();
        m_accountsData->setRowCount(users.count());
        m_accountsData->setColumnCount(4);
        m_accountsData->setHorizontalHeaderLabels(m_headerLabels);
        m_accountsData->setColumnWidth(1, 180);
        m_accountsData->setColumnWidth(2, 280);
        m_accountsData->horizontalHeader()->setStretchLastSection(true);


        if (Q_NULLPTR == m_wizardService)
        {
            int row = 0;
            for (const User &user : users)
            {
                m_accountsData->setItem(row, 0, new QTableWidgetItem(user.GetUserId()));
                m_accountsData->setItem(row, 1, new QTableWidgetItem(user.GetUserName()));
                m_accountsData->setItem(row, 2, new QTableWidgetItem(user.GetUserFCS()));
                if (user.RoleIsValid())
                {
                    m_accountsData->setItem(row, 3, new QTableWidgetItem(Roles.at(user.GetUserRole())));
                }
                else
                {
                    m_accountsData->setItem(row, 3, new QTableWidgetItem(Roles.constFirst()));
                }
                ++row;
            }
        }
        else
        {
            int row = 0;
            for (const User &user : users)
            {
                m_accountsData->setItem(row, 0, new QTableWidgetItem(user.GetUserId()));
                m_accountsData->setItem(row, 1, new QTableWidgetItem(user.GetUserName()));
                QLineEdit *const userLineEdit = CreateUserFCSField(user.GetUserFCS());
                QComboBox *comboBox = Q_NULLPTR;
                if (m_wizardService->GetCurrentUserId() == user.GetUserId() && m_wizardService->GetCurrentUserName() == user.GetUserName())
                {
                    comboBox = new QComboBox();
                    comboBox->addItem(Roles.back());
                    comboBox->setEditable(false);
                    connect(userLineEdit, &QLineEdit::textEdited, m_fcsLineEdit, &QLineEdit::setText);
                    connect(m_fcsLineEdit, &QLineEdit::textEdited, userLineEdit, &QLineEdit::setText);
                }
                else
                {
                    comboBox = CreateUserRoleCB(user.GetUserRole());

                }
                m_accountsData->setCellWidget(row, 2, userLineEdit);
                m_accountsData->setCellWidget(row, 3, comboBox);
                connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,  [this, row](int newRoleIndex)
                {
                    UpdateRoleIndex(row, newRoleIndex);
                });
                connect(userLineEdit, &QLineEdit::textChanged, this,  [this, row](const QString & newFcs)
                {
                    UpdateFCS(row, newFcs);
                });
                ++row;
            }
        }
    }
}

