#ifndef ADMIN_GUI_VIEWS_WIZARD_ENVIROMENTWIZARDPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_ENVIROMENTWIZARDPAGE_H

#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QTableWidget>

#include "mywizardpage.h"
#include "Admin_GUI/RestoreWizard/Services/enviromentalvariables.h"


class EnviromentWizardPage : public MyWizardPage
{
public:
    explicit EnviromentWizardPage(EnviromentalVariablesService *service, QWidget *parent);
    ~EnviromentWizardPage();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void InitUI();
private Q_SLOTS:
    void OnItemChanged(QTableWidgetItem *item);
public:
    virtual int nextId() const Q_DECL_OVERRIDE;
    virtual void initializePage() Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private:
    EnviromentalVariablesService * const m_service;
private:

    QLabel *m_topLabel;
    QTableWidget *m_enviromentalValuesTable;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_ENVIROMENTWIZARDPAGE_H
