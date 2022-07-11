#include "enviromentwizardpage.h"

EnviromentWizardPage::EnviromentWizardPage(EnviromentalVariablesService *service,QWidget *parent)
    : MyWizardPage(parent)
    , m_service(service)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    InitUI();
}

EnviromentWizardPage::~EnviromentWizardPage()
{
    delete m_topLabel;
    delete m_enviromentalValuesTable;
}

void EnviromentWizardPage::CreateUI()
{

    m_topLabel=new QLabel();
    m_enviromentalValuesTable=new QTableWidget();
}

void EnviromentWizardPage::InsertWidgetsIntoLayout()
{
    MainLayout()->addWidget(m_topLabel);
    MainLayout()->addWidget(m_enviromentalValuesTable);
}

void EnviromentWizardPage::InitUI()
{
    m_topLabel->setText("Переменные окружения");
    QFont font = m_topLabel->font();
    font.setPointSize(font.pointSize() + 2);
    m_topLabel->setFont(font);
    m_enviromentalValuesTable->setColumnCount(2);
    m_enviromentalValuesTable->setHorizontalHeaderLabels(QStringList()<< "Ключ" << "Значение");
    m_enviromentalValuesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

int EnviromentWizardPage::nextId() const
{
    return static_cast<int>(WizardPage::Page_Conclusion);
}

void EnviromentWizardPage::initializePage()
{
    for (int i=0; i<m_enviromentalValuesTable->rowCount(); ++i)
    {
        m_enviromentalValuesTable->removeRow(i);
    }
    const QList<QPair<QString, QString> > list=m_service->GetAllKeys();
    m_enviromentalValuesTable->setRowCount(list.count());
    for (int i=0; i<list.count(); ++i)
    {
        m_enviromentalValuesTable->setItem(i, 0, new QTableWidgetItem(list.at(i).first));
        m_enviromentalValuesTable->setItem(i, 1, new QTableWidgetItem(list.at(i).second));
    }
}

void EnviromentWizardPage::resizeEvent(QResizeEvent *event)
{
    const int halfWidth=event->size().width()/m_enviromentalValuesTable->columnCount();
    for (int i=0; i<m_enviromentalValuesTable->columnCount(); ++i)
    {
        m_enviromentalValuesTable->setColumnWidth(i, halfWidth);
    }
}
