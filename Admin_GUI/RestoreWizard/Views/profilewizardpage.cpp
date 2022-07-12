#include "profilewizardpage.h"

ProfileVariablesWizardPage::ProfileVariablesWizardPage(ProfileVariablesService *service,QWidget *parent)
    : MyWizardPage(parent)
    , m_service(service)
{
    CreateUI();
    InsertWidgetsIntoLayout();
    InitUI();
    ConnectObjects();
}

ProfileVariablesWizardPage::~ProfileVariablesWizardPage()
{
    delete m_topLabel;
    delete m_profileValuesTable;
}

void ProfileVariablesWizardPage::CreateUI()
{

    m_topLabel=new QLabel();
    m_profileValuesTable=new QTableWidget();
}

void ProfileVariablesWizardPage::InsertWidgetsIntoLayout()
{
    MainLayout()->addWidget(m_topLabel, 0, Qt::AlignHCenter);
    MainLayout()->addWidget(m_profileValuesTable);
}

void ProfileVariablesWizardPage::InitUI()
{
    m_topLabel->setText("Переменные окружения");
    QFont font = m_topLabel->font();
    font.setPointSize(font.pointSize() + 2);
    m_topLabel->setFont(font);
    m_profileValuesTable->setColumnCount(2);
    m_profileValuesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_profileValuesTable->setHorizontalHeaderLabels(QStringList()<< "Ключ" << "Значение");
    m_profileValuesTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_profileValuesTable->horizontalHeader()->setStretchLastSection(true);
}

void ProfileVariablesWizardPage::ConnectObjects()
{
    connect(m_profileValuesTable, &QTableWidget::itemChanged, this, &ProfileVariablesWizardPage::OnItemChanged);
}

void ProfileVariablesWizardPage::OnItemChanged(QTableWidgetItem *item)
{
    //тк колонки всегда 2 то либо 0 - это ключ или 1 это значение
    const int itemColumn=item->column();
    m_service->SetNewItem(item->text(), item->row(), itemColumn);
}

int ProfileVariablesWizardPage::nextId() const
{
    return static_cast<int>(WizardPage::Page_FilesAndFolders);
}

void ProfileVariablesWizardPage::initializePage()
{
    const bool oldBlocked = m_profileValuesTable->blockSignals(true);
    for (int i=0; i<m_profileValuesTable->rowCount(); ++i)
    {
        m_profileValuesTable->removeRow(i);
    }
    const QList<QPair<QString, QString> > list=m_service->GetAllKeys();
    m_profileValuesTable->setRowCount(list.count());
    for (int i=0; i<list.count(); ++i)
    {
        m_profileValuesTable->setItem(i, 0, new QTableWidgetItem(list.at(i).first));
        m_profileValuesTable->setItem(i, 1, new QTableWidgetItem(list.at(i).second));
    }
    m_profileValuesTable->blockSignals(oldBlocked);
}

void ProfileVariablesWizardPage::resizeEvent(QResizeEvent *event)
{
    const int halfWidth=(event->size().width()-51)/m_profileValuesTable->columnCount();
    for (int i=0; i<m_profileValuesTable->columnCount(); ++i)
    {
        m_profileValuesTable->setColumnWidth(i, halfWidth);
    }

}

void ProfileVariablesWizardPage::keyPressEvent(QKeyEvent *event)
{
    if (Qt::Key_Z == event->key() && (event->modifiers()&Qt::ControlModifier))
    {
        RestoreLastDeletedItem();
        return;
    }
    if(Qt::Key_Delete==event->key())
    {
        DeleteSelectedItem();
    }
}

void ProfileVariablesWizardPage::RestoreLastDeletedItem()
{
    if(!m_deletedHistory.isEmpty())
    {
        const QPair<QString, QString> item=m_deletedHistory.pop();
        for (int i=0; i<m_profileValuesTable->rowCount(); ++i)
        {
            const QString key=m_profileValuesTable->item(i, 0)->text();
            if(key==item.first)
            {
                const QString value=m_profileValuesTable->item(i, 1)->text();
                if(value!=item.second)
                {
                    m_profileValuesTable->item(i, 1)->setText(item.second);
                }
                return;
            }
        }
        m_profileValuesTable->insertRow(m_profileValuesTable->rowCount());
        m_profileValuesTable->setItem(m_profileValuesTable->rowCount()-1, 0, new QTableWidgetItem(item.first));
        m_profileValuesTable->setItem(m_profileValuesTable->rowCount()-1, 1, new QTableWidgetItem(item.second));
    }
}

void ProfileVariablesWizardPage::DeleteSelectedItem()
{
    const QList<QTableWidgetItem*> selectedItems=m_profileValuesTable->selectedItems();
    if(!selectedItems.isEmpty())
    {
        QTableWidgetItem *selectedItem=m_profileValuesTable->selectedItems().front();
        const int row=selectedItem->row();
        const QString key=m_profileValuesTable->item(row, 0)->text();
        const QString value=m_profileValuesTable->item(row, 1)->text();
        m_profileValuesTable->removeRow(row);
        const QPair<QString, QString> variable(key, value);
        m_deletedHistory.push_back(variable);
    }
}
