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
#include <QDomDocument>

#include "Admin_GUI/Views/Wizards/wizardpages.h"

class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QDomDocument *backupXMLDocument, QWidget *parent);
    ~IntroPage();
    int nextId() const override;
private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_topLabel;
    QVBoxLayout *m_backupFileLayout;
    QHBoxLayout *m_backupFileLoadLayout;
    QPushButton *m_backupLoadButton;
    QLabel *m_backupLabel;
    QLineEdit *m_backupLineEdit;
    QLabel *m_faqLabel;

private:
    QDomDocument *m_backupXMLDocument;
    QFile *m_backupFile;
    QStringList *m_checkedList;
private:
    void setWizardTitle();
    void initUI();
    void insertWidgetsIntoLayout();
    void createConnections();
private Q_SLOTS:
    void addSettingsFile();
private:
    bool checkBackupFile(QString &backupPath);
    void setToBackupXmlDefaultStruct();
};

#endif // INTROPAGE_H
