#ifndef ADMIN_GUI_VIEWS_WIZARD_PROFILEWIZARDPAGE_H
#define ADMIN_GUI_VIEWS_WIZARD_PROFILEWIZARDPAGE_H

#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QTableWidget>
#include <QHeaderView>
#include <QStack>

#include "mywizardpage.h"
#include "Admin_GUI/RestoreWizard/Services/enviromentalvariables.h"


class ProfileVariablesWizardPage : public MyWizardPage
{
public:
    explicit ProfileVariablesWizardPage(ProfileVariablesService *service, QWidget *parent);
    ~ProfileVariablesWizardPage();
private:
    void CreateUI();
    void InsertWidgetsIntoLayout();
    void InitUI();
    void ConnectObjects();
private Q_SLOTS:
    void OnItemChanged(QTableWidgetItem *item);
public:
    virtual int nextId() const Q_DECL_OVERRIDE;
    virtual void initializePage() Q_DECL_OVERRIDE;
protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
private:
    void RestoreLastDeletedItem();
    void DeleteSelectedItem();
private:
    ProfileVariablesService * const m_service;
    QStack<QPair<QString, QString>> m_deletedHistory;
private:

    QLabel *m_topLabel;
    QTableWidget *m_profileValuesTable;
};

#endif // ADMIN_GUI_VIEWS_WIZARD_PROFILEWIZARDPAGE_H
