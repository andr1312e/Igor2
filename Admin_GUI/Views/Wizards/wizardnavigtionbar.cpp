#include "wizardnavigtionbar.h"

WizardNavigtionBar::WizardNavigtionBar(QWidget *parent)
    : QWidget(parent)
{
    CreateUI();
    InsertWidgetsIntoLayouts();
    FillUI();
    ConnectObjects();
}

WizardNavigtionBar::~WizardNavigtionBar()
{
    delete m_mainLayout;
    QList<QAbstractButton*> buttolsList=m_buttonGroup->buttons();
    qDeleteAll(buttolsList);
    delete m_buttonGroup;
    delete m_label;
}

void WizardNavigtionBar::CreateUI()
{
    m_mainLayout=new QHBoxLayout();

    m_label=new QLabel();
    m_buttonGroup=new QButtonGroup();
    for (int i=0; i<=Page_Conclusion; i++)
    {
        QPushButton *button=new QPushButton(QString::number(i));
        button->setFlat(true);
        button->setDefault(false);
        button->setAutoDefault(false);
        button->setFocusPolicy(Qt::NoFocus);
        button->setFixedWidth(30);
        SetButtonToolTip(button, i);
        m_buttonGroup->addButton(button, i);
    }
}

void WizardNavigtionBar::InsertWidgetsIntoLayouts()
{
    QList <QAbstractButton*> buttonsList=m_buttonGroup->buttons();
    m_mainLayout->addWidget(m_label);
    for (int i=0; i<buttonsList.count(); ++i)
    {
        m_mainLayout->addWidget(buttonsList.at(i));
    }
    setLayout(m_mainLayout);

}

void WizardNavigtionBar::FillUI()
{
    m_label->setText("Перейти на страницу:");
}

void WizardNavigtionBar::ConnectObjects()
{
    connect(m_buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &WizardNavigtionBar::ToPageNumMove);
}

void WizardNavigtionBar::SetButtonToolTip(QPushButton *button, const int &pageIndex)
{
    switch (pageIndex)
    {
    case Page_Intro:
        button->setToolTip("Страница выбора файла настроек");
        break;
    case Page_UserData:
        button->setToolTip("Данные пользователей и администратора");
        break;
    case Page_FirstRole:
    case Page_SecondRole:
    case Page_ThirdRole:
    case Page_FourthRole:
        button->setToolTip("Программы для роли: " + Roles.at(pageIndex-2));
        break;
    case Page_Conclusion:
        button->setToolTip("Страница завершения настроек");
        break;
    default:
        break;
    }
}
