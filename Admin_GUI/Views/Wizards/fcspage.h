#ifndef FCSPAGE_H
#define FCSPAGE_H
#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>
#include <QLineEdit>
#include <QComboBox>

class FCSPage: public QWizardPage
{
   Q_OBJECT
public:
    FCSPage(QWidget *parent);
    ~FCSPage();
    int nextId() const override;
private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_topLabel;
    QLabel *m_FCSLabel;
    QLineEdit *m_FCSlineEdit;
    QLabel *m_RankLabel;
    QComboBox *m_RankComboBox;
private:
    void initUI();
    void insertWidgetsIntoLayout();
    void createConnections();
};

#endif // FCSPAGE_H
