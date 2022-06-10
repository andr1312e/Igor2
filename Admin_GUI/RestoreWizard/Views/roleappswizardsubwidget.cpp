#include "roleappswizardsubwidget.h"

RoleAppsWizardSubWidget::RoleAppsWizardSubWidget(const QString title, QWidget *parent)
    : QWidget(parent)
{
    CreateUI(title);
    InsertWidgetsIntoLayout();
}

RoleAppsWizardSubWidget::~RoleAppsWizardSubWidget()
{
    delete m_mainLayout;

    delete m_topLabel;
    delete m_execsLabel;
    delete m_execsList;
    delete m_desktopsLabel;
    delete m_desktopsTable;
}

void RoleAppsWizardSubWidget::CreateUI(const QString &title)
{
    m_mainLayout = new QVBoxLayout();
    m_topLabel = new QLabel(QStringLiteral("Данные полученны из: ") + title);
    m_execsLabel = new QLabel(QStringLiteral("Исполняемые файлы для перезапуска:"));
    m_execsList = new QListWidget();
    m_desktopsLabel = new QLabel(QStringLiteral("Ярлыки на рабочем столе:"));
    m_desktopsTable = new QTableWidget();
    m_desktopsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void RoleAppsWizardSubWidget::InsertWidgetsIntoLayout()
{
    m_mainLayout->addWidget(m_topLabel);
    m_mainLayout->addWidget(m_execsLabel);
    m_mainLayout->addWidget(m_execsList);
    m_mainLayout->addWidget(m_desktopsLabel);
    m_mainLayout->addWidget(m_desktopsTable);
    setLayout(m_mainLayout);
}

void RoleAppsWizardSubWidget::SetWizardWidgetFileds(QList<DesktopEntity> &desktops, QStringList &ecexs)
{
    m_execsList->clear();
    m_execsList->addItems(ecexs);
    m_desktopsTable->clear();
    m_desktopsTable->setRowCount(desktops.count());
    m_desktopsTable->setColumnCount(2);
    m_desktopsTable->setHorizontalHeaderLabels(m_headerLabels);
    m_desktopsTable->setColumnWidth(1, 280);
    m_desktopsTable->horizontalHeader()->setStretchLastSection(true);

    for (int r = 0; r < desktops.count(); ++r) {
        m_desktopsTable->setItem(r, 0, new QTableWidgetItem(desktops.at(r).GetName()));
        m_desktopsTable->setItem(r, 1, new QTableWidgetItem(desktops.at(r).GetExec()));
    }
}


