#ifndef WIZARDNAVIGTIONBAR_H
#define WIZARDNAVIGTIONBAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include "Structs/userstruct.h"

#include "Admin_GUI/Wizard/Views/wizardpages.h"

class WizardNavigtionBar : public QWidget
{
    Q_OBJECT
public:
    explicit WizardNavigtionBar(QWidget *parent);
    ~WizardNavigtionBar();
private:
    void CreateUI();
    void InsertWidgetsIntoLayouts();
    void FillUI();
    void ConnectObjects();
Q_SIGNALS:
    void ToPageNumMove(int id);
private:
    void SetButtonToolTip(QPushButton *button, const int &pageIndex);
private:
    QLabel *m_label;
    QButtonGroup *m_buttonGroup;
    QHBoxLayout *m_mainLayout;
};

#endif // WIZARDNAVIGTIONBAR_H
