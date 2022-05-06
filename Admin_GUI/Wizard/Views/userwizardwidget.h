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
   void FillWidget(const QString &title, const QString &FCS, const QList<User> &users);
   QString GetUserFCS() const;
private:
   const QStringList m_headerLabels;
   StringValidator *m_stringValidator;

private:
   QVBoxLayout *m_mainLayout;
   QLabel *m_topLabel;

   QHBoxLayout *m_userFCSLayout;
   QLabel *m_fcsLabel;
   QLineEdit *m_fcsLineEdit;

   QHBoxLayout *m_numOfUsersLayout;
   QLabel *m_numOfUsersLabel;
   QLabel *m_numOfUsersValue;

   QTableWidget *m_accountsData;

};

#endif // ADMIN_GUI_VIEWS_USERWIZARDWIDGET_H
