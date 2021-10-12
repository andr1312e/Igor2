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
    IntroPage(QWidget *parent);
    ~IntroPage();
    int nextId() const override;
private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_topLabel;
    QVBoxLayout *m_settingsFileLayout;
    QHBoxLayout *m_settingsFileInputLayout;
    QPushButton *m_settingsFileButton;
    QLabel *m_settingsFileLabel;
    QLineEdit *m_settingsFilePath;
    QLabel *m_faqTitle;

private:
    QDomDocument *m_document;
    QFile *m_file;
private:
    void setWizardTitle();
    void initUI();
    void insertWidgetsIntoLayout();
    void createConnections();
private Q_SLOTS:
    void addSettingsFile();
private:
    bool checkFile(QString &file);
};

#endif // INTROPAGE_H
