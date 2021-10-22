#include "roleappswizardwidget.h"

RoleAppsWizardWidget::RoleAppsWizardWidget(QString title, QWidget *parent)
   : QWidget(parent)
{
   initUI(title);
   insertWidgetsIntoLayout();
   createConnections();
}

RoleAppsWizardWidget::~RoleAppsWizardWidget()
{
   delete m_mainLayout;

   delete m_topLabel;
   delete m_execsLabel;
   delete m_execsList;
   delete m_desktopsLabel;
   delete m_desktopsTable;
}

void RoleAppsWizardWidget::setWizardWidgetFileds(QList<DesktopEntity> &desktops, QStringList &ecexs)
{
   m_execsList->clear();
   m_execsList->addItems(ecexs);
   m_desktopsTable->clear();
   m_desktopsTable->setRowCount(desktops.count());
   m_desktopsTable->setColumnCount(3);
   m_desktopsTable->setHorizontalHeaderLabels(m_headerLabels);
   //   m_desktopsTable->horizontalHeader()->
   m_desktopsTable->setColumnWidth(1, 280);
   m_desktopsTable->horizontalHeader()->setStretchLastSection(true);

   for (int r = 0; r < desktops.count(); r++) {
      m_desktopsTable->setItem(r, 0, new QTableWidgetItem(desktops.at(r).name));
      m_desktopsTable->setItem(r, 1, new QTableWidgetItem(desktops.at(r).exec));
      m_desktopsTable->setItem(r, 2, new QTableWidgetItem(desktops.at(r).type == "Application" ? "Программа" : "Файл"));
   }
}

void RoleAppsWizardWidget::initUI(QString &title)
{
   m_mainLayout = new QVBoxLayout();
   m_topLabel = new QLabel("Данные полученны из: " + title);
   m_execsLabel = new QLabel("Исполняемые файлы:");
   m_execsList = new QListWidget();
   m_desktopsLabel = new QLabel("Ярлыки на рабочем столе:");
   m_desktopsTable = new QTableWidget();
   m_desktopsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void RoleAppsWizardWidget::insertWidgetsIntoLayout()
{
   m_mainLayout->addWidget(m_topLabel);
   m_mainLayout->addWidget(m_execsLabel);
   m_mainLayout->addWidget(m_execsList);
   m_mainLayout->addWidget(m_desktopsLabel);
   m_mainLayout->addWidget(m_desktopsTable);
   setLayout(m_mainLayout);
}

void RoleAppsWizardWidget::createConnections()
{

}
