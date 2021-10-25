#ifndef ADMIN_GUI_VIEWS_FILEDIALOGWIDGET_H
#define ADMIN_GUI_VIEWS_FILEDIALOGWIDGET_H

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

class FileDialogWidget:public QWidget
{
    Q_OBJECT
public:

    FileDialogWidget(QWidget *parent);

    ~FileDialogWidget();

    void setTitleText(QString &text);

signals:

    void hideDialogSignal();

    void addFileToUserDesktop(const QString &exec, const QString &iconPath, const QString &iconName);

private:

    void initUI();

    void insertWidgetsIntoLayout();

    void createConnections();

private:

    void clearAllTextFiels();

private slots:

    void hideDialog();

    void addEcexPath();

    void addIconPath();

    void addIconToUserDesktop();

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

#endif // FILEDIALOGWIDGET_H
