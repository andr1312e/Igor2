#include "linuxuserslistwidget.h"

#include <QDebug>

LinuxUsersListWidget::LinuxUsersListWidget(UserModel *userModel, QWidget *parent)
    : QWidget(parent)
    , m_oldFontSize(0)
{
    SetModel(userModel);
    CreateProxyModel();
    CreateUI();
    InsertWidgetsIntoLayout();
    SetModelToListView();
    CreateConnections();
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

void LinuxUsersListWidget::SetModel(UserModel *userModel)
{
    m_userModel = userModel;
}

void LinuxUsersListWidget::CreateProxyModel()
{
    m_model = m_userModel->GetModel();
    m_sortModel = new SortModel(this);
    m_sortModel->setSourceModel(m_model);
    m_sortModel->setDynamicSortFilter(true);
}

void LinuxUsersListWidget::SetModelToListView()
{
    m_allUsersListView->setModel(m_sortModel);
}

void LinuxUsersListWidget::CreateUI()
{
    m_mainLayout = new QVBoxLayout();

    m_linuxUsersLabel = new QLabel("Пользователи операционной системы Astra Linux: ");

    m_font =m_linuxUsersLabel->font();

    m_userDelegate = new UserDelegate(m_font, this);

    m_searchLayout = new QHBoxLayout();
    m_searchTypeComboBox = new QComboBox();
    m_searchTypeComboBox->addItems(comboBoxSearchAttributes);
    m_searchLineEdit = new QLineEdit();
    m_searchLineEdit->setMaximumWidth(195);

    m_allUsersListView = new QListView();
    m_allUsersListView->setItemDelegate(m_userDelegate);
    m_allUsersListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void LinuxUsersListWidget::InsertWidgetsIntoLayout()
{
    m_searchLayout->addWidget(m_searchLineEdit);
    m_searchLayout->addWidget(m_searchTypeComboBox);

    m_mainLayout->addWidget(m_linuxUsersLabel);
    m_mainLayout->addLayout(m_searchLayout);
    m_mainLayout->addWidget(m_allUsersListView);

    setLayout(m_mainLayout);
}

void LinuxUsersListWidget::CreateConnections()
{
    connect(m_allUsersListView, &QListView::clicked, this, &LinuxUsersListWidget::GetUserData);
    connect(m_searchLineEdit, &QLineEdit::textChanged, this, &LinuxUsersListWidget::OnLineEditChange);
    connect(m_searchTypeComboBox, &QComboBox::currentTextChanged, this, &LinuxUsersListWidget::OnComboBoxChange);
    connect(this, &LinuxUsersListWidget::ToSearch, m_sortModel, &SortModel::UpdateSeachTextAndSeachAttribute);
}

void LinuxUsersListWidget::UpdateFontSize()
{
    m_font.setPointSize(m_oldFontSize);
    m_linuxUsersLabel->setFont(m_font);
    m_searchLineEdit->setFont(m_font);
    m_searchTypeComboBox->setFont(m_font);
    m_allUsersListView->setFont(m_font);
}

void LinuxUsersListWidget::OnLineEditChange(const QString &text)
{
    const QString attribute = m_searchTypeComboBox->currentText();
    m_sortModel->UpdateSeachTextAndSeachAttribute(text, attribute);
}

void LinuxUsersListWidget::OnComboBoxChange(const QString &attribute)
{
    const QString text = m_searchLineEdit->text();
    m_sortModel->UpdateSeachTextAndSeachAttribute(text, attribute);
}

void LinuxUsersListWidget::GetUserData(const QModelIndex &index)
{
    QVariant indexData = index.data(Qt::UserRole + 1);
    User user = indexData.value<User>();
    Q_EMIT ToUserClick(user);
}

void LinuxUsersListWidget::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width();
//    qDebug() << width;

    if (width > 930) {
        if (m_oldFontSize != 20) {
            m_oldFontSize = 20;
            UpdateFontSize();
            m_searchTypeComboBox->setFixedHeight(44);
            m_searchLineEdit->setFixedHeight(44);
        }
    } else {
        if (width > 861) {
            if (m_oldFontSize != 19) {
                m_oldFontSize = 19;
                UpdateFontSize();
                m_searchTypeComboBox->setFixedHeight(42);
                m_searchLineEdit->setFixedHeight(42);
            }
        } else {
            if (width > 845) {
                if (m_oldFontSize != 18) {
                    m_oldFontSize = 18;
                    UpdateFontSize();
                    m_searchTypeComboBox->setFixedHeight(40);
                    m_searchLineEdit->setFixedHeight(40);
                }
            } else {
                if (width > 810) {
                    if (m_oldFontSize != 17) {
                        m_oldFontSize = 17;
                        UpdateFontSize();
                        m_searchTypeComboBox->setFixedHeight(38);
                        m_searchLineEdit->setFixedHeight(38);
                    }
                } else {
                    if (width > 726) {
                        if (m_oldFontSize != 16) {
                            m_oldFontSize = 16;
                            UpdateFontSize();
                            m_searchTypeComboBox->setFixedHeight(36);
                            m_searchLineEdit->setFixedHeight(36);
                        }
                    } else {
                        if (width > 700) {
                            if (m_oldFontSize != 15) {
                                m_oldFontSize = 15;
                                UpdateFontSize();
                                m_searchTypeComboBox->setFixedHeight(34);
                                m_searchLineEdit->setFixedHeight(34);
                            }
                        } else {
                            if (width > 666) {
                                if (m_oldFontSize != 14) {
                                    m_oldFontSize = 14;
                                    UpdateFontSize();
                                    m_searchTypeComboBox->setFixedHeight(33);
                                    m_searchLineEdit->setFixedHeight(33);
                                }
                            } else {
                                if (width > 600) {
                                    if (m_oldFontSize != 13) {
                                        m_oldFontSize = 13;
                                        UpdateFontSize();
                                        m_searchTypeComboBox->setFixedHeight(32);
                                        m_searchLineEdit->setFixedHeight(32);
                                    }
                                } else {
                                    if (width > 565) {
                                        if (m_oldFontSize != 12) {
                                            m_oldFontSize = 12;
                                            UpdateFontSize();
                                            m_searchTypeComboBox->setFixedHeight(31);
                                            m_searchLineEdit->setFixedHeight(31);
                                        }
                                    } else {
                                        if (width > 550) {
                                            if (m_oldFontSize != 11) {
                                                m_oldFontSize = 11;
                                                UpdateFontSize();
                                                m_searchTypeComboBox->setFixedHeight(30);
                                                m_searchLineEdit->setFixedHeight(30);
                                            }
                                        } else {
                                            if (m_oldFontSize != 10) {
                                                m_oldFontSize = 10;
                                                UpdateFontSize();
                                                m_searchTypeComboBox->setFixedHeight(29);
                                                m_searchLineEdit->setFixedHeight(29);
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
