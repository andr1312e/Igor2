#ifndef WIZARDNAVIGTIONBAR_H
#define WIZARDNAVIGTIONBAR_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include "Structs/user.h"

#include "Admin_GUI/RestoreWizard/Views/wizardpages.h"

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
public:
    void SetButtonDown(int id) noexcept;
private:
    void SetButtonToolTip(QPushButton *button, int pageIndex) noexcept;
private:
    QLabel *m_descriptionLabel;
    QButtonGroup *m_buttonGroup;
    QHBoxLayout *m_mainLayout;
};

#endif // WIZARDNAVIGTIONBAR_H
