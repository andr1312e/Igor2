#ifndef ADMIN_GUI_VIEWS_WIZARD_ROLEWIDGET_H
#define ADMIN_GUI_VIEWS_WIZARD_ROLEWIDGET_H

#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>

#include "Structs/programstruct.h"

class RoleAppsWizardSubWidget : public QWidget
{
   Q_OBJECT
public:
   explicit RoleAppsWizardSubWidget(const QString title, QWidget *parent);
   ~RoleAppsWizardSubWidget();
private:
   void CreateUI(const QString &title);
   void InsertWidgetsIntoLayout();
public:
   void SetWizardWidgetFileds(QList<DesktopEntity> &desktops, QStringList &ecexs);
private:
   const QStringList m_headerLabels = {QStringLiteral("Имя ярлыка"), QStringLiteral("Путь к исполняемому файлу"), QStringLiteral("Тип")};
   QVBoxLayout *m_mainLayout;
   QLabel *m_topLabel;
   QLabel *m_execsLabel;
   QListWidget *m_execsList;
   QLabel *m_desktopsLabel;
   QTableWidget *m_desktopsTable;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_ROLEWIDGET_H
