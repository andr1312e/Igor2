#ifndef ADMIN_GUI_VIEWS_USERWIZARDWIDGET_H
#define ADMIN_GUI_VIEWS_USERWIZARDWIDGET_H

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QHeaderView>

#include "Structs/userstruct.h"

#include "Admin_GUI/Validator/stringvalidator.h"

class UserWizardWidget : public QWidget
{
   Q_OBJECT
public:
   explicit UserWizardWidget(const QStringList &headerLabels, QWidget *parent);
   ~UserWizardWidget();
private:
   void InitUI();
   void InsertWidgetsIntoLayout();
public:
   void setWizardWidgetFileds(const QString &title, QString &FCS, QString &rank, QList<User> &users);
   QString getUserChoise();
private:
   const QStringList &m_headerLabels;
   StringValidator *m_stringValidator;
   QVBoxLayout *m_mainLayout;
   QLabel *m_topLabel;

   QHBoxLayout *m_userFCSLayout;
   QLabel *m_fcsLabel;
   QLineEdit *m_fcsLineEdit;

   QHBoxLayout *m_userRankLayout;
   QLabel *m_backupRankLabel;
   QComboBox *m_rankComboBox;

   QHBoxLayout *m_numOfUsersLayout;
   QLabel *m_numOfUsersLabel;
   QLabel *m_numOfUsersValue;

   QTableWidget *m_accountsData;

};

#endif // ADMIN_GUI_VIEWS_USERWIZARDWIDGET_H
