#include "linuxuserslistwidget.h"

LinuxUsersListWidget::LinuxUsersListWidget(UserModel *userModel, QWidget *parent)
    : QWidget(parent)
{
    createProxyModel(userModel);
    initUI();
    insertWidgetsIntoLayout();
    setModelToView();
    createConnections();
}

LinuxUsersListWidget::~LinuxUsersListWidget()
{
    delete m_searchLayout;
    delete m_mainLayout;

    delete m_sortModel;
    delete m_userDelegate;

    delete m_linuxUsersLabel;
    delete m_searchLineEdit;
    delete m_searchTypeComboBox;
    delete m_allUsersListView;
}

void LinuxUsersListWidget::createProxyModel(UserModel *userModel)
{
    m_model=userModel->getModel();
    m_sortModel=new SortModel();
    m_sortModel->setSourceModel(m_model);
    m_sortModel->setDynamicSortFilter(true);
}

void LinuxUsersListWidget::setModelToView()
{
    m_allUsersListView->setModel(m_sortModel);
}

void LinuxUsersListWidget::initUI()
{
    m_mainLayout=new QVBoxLayout();
    m_userDelegate=new UserDelegate(this);
    m_linuxUsersLabel=new QLabel("Пользователи операционной системы Astra Linux: ");

    m_searchLayout=new QHBoxLayout();
    m_searchTypeComboBox=new QComboBox();
    m_searchTypeComboBox->addItems(comboBoxSearchAttributes);
    m_searchLineEdit=new QLineEdit();
    m_searchLineEdit->setMaximumWidth(195);

    m_allUsersListView=new QListView();
    m_allUsersListView->setItemDelegate(m_userDelegate);
}

void LinuxUsersListWidget::insertWidgetsIntoLayout()
{
    m_searchLayout->addWidget(m_searchLineEdit);
    m_searchLayout->addWidget(m_searchTypeComboBox);

    m_mainLayout->addWidget(m_linuxUsersLabel);
    m_mainLayout->addLayout(m_searchLayout);
    m_mainLayout->addWidget(m_allUsersListView);

    setLayout(m_mainLayout);
}

void LinuxUsersListWidget::createConnections()
{
    connect(m_allUsersListView, &QListView::clicked, this, &LinuxUsersListWidget::getUserData);
    connect(m_searchLineEdit, &QLineEdit::textChanged, this, &LinuxUsersListWidget::onLineEditChange);
    connect(m_searchTypeComboBox, &QComboBox::currentTextChanged, this, &LinuxUsersListWidget::onComboBoxChange);
    connect(this, &LinuxUsersListWidget::search, m_sortModel, &SortModel::UpdateSeachWordAndSeachAttribute);
}

void LinuxUsersListWidget::onLineEditChange(const QString &text)
{
    const QString attribute=m_searchTypeComboBox->currentText();
    m_sortModel->UpdateSeachWordAndSeachAttribute(text, attribute);
}

void LinuxUsersListWidget::onComboBoxChange(const QString &attribute)
{
    const QString text=m_searchLineEdit->text();
    m_sortModel->UpdateSeachWordAndSeachAttribute(text, attribute);
}

void LinuxUsersListWidget::getUserData(const QModelIndex &index)
{
    QVariant indexData=index.data(Qt::UserRole+1);
    User user=indexData.value<User>();
    emit onUserClick(user);
}
