#ifndef ADMIN_GUI_VIEWS_ADDITIONALSETTINGSPANEL_DIALOGWIDGET_FILEDIALOGWIDGET_H
#define ADMIN_GUI_VIEWS_ADDITIONALSETTINGSPANEL_DIALOGWIDGET_FILEDIALOGWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

#include "functionswithstrings.h"
#include "Admin_GUI/Widgets/qtmaterialtextfield.h"

class FileDialogWidget: public QWidget
{
   Q_OBJECT
public:

   FileDialogWidget(QWidget *parent);

   ~FileDialogWidget();

private:

   void CreateUI();

   void InsertWidgetsIntoLayout();

   void FillUI();

   void ConnectObject();

Q_SIGNALS:

   void ToDialogSignalHide();

   void ToAddFileToUserDesktop(const QString &exec, const QString &iconPath, const QString &iconName);

private Q_SLOTS:

   void OnHideDialog();

   void OnAddingEcexutePath();

   void OnAddIconPath();

   void OnAddIconToUserDesktop();

public:

   void SetTitleText(QString &text);

private:

   void ClearAllTextFiels();

private:

   QString m_userName;

   QVBoxLayout *m_mainLayout;

   QLabel *m_titleLabel;


   QHBoxLayout *m_execPathLayout;

   QtMaterialTextField *m_exec;

   QPushButton *m_execButton;


   QHBoxLayout *m_iconPathLayout;

   QtMaterialTextField *m_iconPath;

   QPushButton *m_iconPathButton;


   QtMaterialTextField *m_iconName;


   QHBoxLayout *m_dialogWidgetButtonsLayout;

   QPushButton *m_saveDialogButton;

   QPushButton *m_closeDialogButton;

   QMessageBox *m_messagBox;
};

#endif // ADMIN_GUI_VIEWS_ADDITIONALSETTINGSPANEL_DIALOGWIDGET_FILEDIALOGWIDGET_H
