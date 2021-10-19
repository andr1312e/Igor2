#ifndef INTROPAGE_H
#define INTROPAGE_H
#include <QLabel>
#include <QWizardPage>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QStandardPaths>

#include <Services/Settings/wizardservice.h>

#include "Services/Settings/programfilesstate.h"
#include "Admin_GUI/Views/Wizards/wizardpages.h"

class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(ProgramFilesState &state, WizardService *service, QWidget *parent);
    ~IntroPage();
    int nextId() const override;
Q_SIGNALS:
    void checkBackupFile(QString &backupPath);
private:
    ProgramFilesState m_state;
    WizardService *m_wizardService;

    QVBoxLayout *m_mainLayout;
    QLabel *m_topLabel;
    QVBoxLayout *m_backupFileLayout;
    QHBoxLayout *m_backupFileLoadLayout;
    QPushButton *m_backupLoadButton;
    QLabel *m_backupLabel;
    QLineEdit *m_backupLineEdit;
    QLabel *m_faqLabel;

    QString m_backupFilePath;
private:
    void setWizardTitle();
    void initUI();
    void insertWidgetsIntoLayout();
    void createConnections();
private Q_SLOTS:
    void CheckBackupFile();
};

#endif // INTROPAGE_H
