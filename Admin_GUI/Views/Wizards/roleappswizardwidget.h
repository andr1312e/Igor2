#ifndef ROLEWIDGET_H
#define ROLEWIDGET_H

#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>
#include <Structs/programstruct.h>


class RoleAppsWizardWidget : public QWidget
{
   Q_OBJECT
public:
   RoleAppsWizardWidget(QString title, QWidget *parent);
   virtual ~RoleAppsWizardWidget();
   void setWizardWidgetFileds(QList<DesktopEntity> &desktops, QStringList &ecexs);
private:
   QStringList m_headerLabels = {"Имя ярлыка", "Путь к исполняемому файлу", "Тип"};
   QVBoxLayout *m_mainLayout;
   QLabel *m_topLabel;
   QLabel *m_execsLabel;
   QListWidget *m_execsList;
   QLabel *m_desktopsLabel;
   QTableWidget *m_desktopsTable;
private:
   void initUI(QString &title);
   void insertWidgetsIntoLayout();
   void createConnections();
};

#endif // ROLEWIDGET_H
