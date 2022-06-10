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
    QList<QAbstractButton *> buttolsList = m_buttonGroup->buttons();
    qDeleteAll(buttolsList);
    delete m_buttonGroup;
    delete m_descriptionLabel;
}

void WizardNavigtionBar::CreateUI()
{
    m_mainLayout = new QHBoxLayout();

    m_descriptionLabel = new QLabel();
    m_buttonGroup = new QButtonGroup();
    for (int i = 0; i <= static_cast<int>(WizardPage::Page_Conclusion); ++i)
    {
        QPushButton *const button = new QPushButton(QString::number(i + 1));
        button->setCheckable(true);
        button->setChecked(false);
        button->setFlat(true);
        button->setFocusPolicy(Qt::NoFocus);
        button->setFixedWidth(30);
        SetButtonToolTip(button, i);
        m_buttonGroup->addButton(button, i);
    }
}

void WizardNavigtionBar::InsertWidgetsIntoLayouts()
{
    const QList <QAbstractButton *> buttonsList = m_buttonGroup->buttons();
    m_mainLayout->addWidget(m_descriptionLabel);
    for (QAbstractButton *const button : buttonsList)
    {
        m_mainLayout->addWidget(button);
    }
    setLayout(m_mainLayout);

}

void WizardNavigtionBar::FillUI()
{
    m_descriptionLabel->setText(QStringLiteral("Перейти на страницу:"));
}

void WizardNavigtionBar::ConnectObjects()
{
    connect(m_buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &WizardNavigtionBar::ToPageNumMove);
}

void WizardNavigtionBar::SetButtonDown(int id) noexcept
{
    const QList <QAbstractButton *> buttonsList = m_buttonGroup->buttons();
    for (int i = 0; i < buttonsList.count(); ++i)
    {
        if (id == i)
        {
            if (!buttonsList.at(i)->isChecked())
            {
                buttonsList.at(i)->setChecked(true);
            }
        }
        else
        {
            if (buttonsList.at(i)->isChecked())
            {
                buttonsList.at(i)->setChecked(false);
            }
        }
    }
}

void WizardNavigtionBar::SetButtonToolTip(QPushButton *button, const int &pageIndex)
{
    switch (WizardPage(pageIndex))
    {
    case WizardPage::Page_Intro:
        button->setToolTip(QStringLiteral("Страница выбора файла настроек"));
        break;
    case WizardPage::Page_UserData:
        button->setToolTip(QStringLiteral("Данные пользователей и администратора"));
        break;
    case WizardPage::Page_FirstRole:
    case WizardPage::Page_SecondRole:
    case WizardPage::Page_ThirdRole:
    case WizardPage::Page_FourthRole:
        button->setToolTip(QStringLiteral("Программы для роли: ") + Roles.at(pageIndex - 2));
        break;
    case WizardPage::Page_Conclusion:
        button->setToolTip(QStringLiteral("Страница завершения настроек"));
        break;
    default:
        break;
    }
}
