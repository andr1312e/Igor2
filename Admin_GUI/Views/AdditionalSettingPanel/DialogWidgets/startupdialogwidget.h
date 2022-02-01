#ifndef ADMIN_GUI_VIEWS_ADDITIONALSETTINGSPANEL_DIALOGWIDGET_EXECDIALOGWIDGET_H
#define ADMIN_GUI_VIEWS_ADDITIONALSETTINGSPANEL_DIALOGWIDGET_EXECDIALOGWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

#include <QWidget>

#include "Logging/logger.h"
#include "Admin_GUI/Widgets/qtmaterialtextfield.h"

class StartupDialogWidget : public QWidget
{
   Q_OBJECT

public:
   explicit StartupDialogWidget(QWidget *parent);
   ~StartupDialogWidget();

private:
   void CreateUI();
   void InsertWidgetsIntoLayouts();
   void InitUI();
   void ConnectObjects();

Q_SIGNALS:
   void ToHideDialog();
   void ToAddExecPathToFile(const QString &exec);


private Q_SLOTS:
   void OnHideAndClearDialog();
   void OnAddEcexPath();
   void OnCheckExec();

private:
   void CearAllTextFiels();

private:

   QVBoxLayout *m_mainLayout;
   QLabel *m_titleLabel;
   QHBoxLayout *m_execPathLayout;
   QtMaterialTextField *m_execTextField;
   QPushButton *m_execButton;

   QHBoxLayout *m_bottomButtonsLayout;
   QPushButton *m_saveDialogButton;
   QPushButton *m_closeDialogButton;
   QMessageBox *m_errorMessagBox;

private:
   const QString m_desktopPath;
};

#endif // ADMIN_GUI_VIEWS_ADDITIONALSETTINGSPANEL_DIALOGWIDGET_EXECDIALOGWIDGET_H
