#ifndef ADMIN_GUI_VIEWS_DESKTOPPANEL_H
#define ADMIN_GUI_VIEWS_DESKTOPPANEL_H

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>

#include "Structs/programstruct.h"

#include "Services/fileexplorer.h"

#include "Admin_GUI/Delegates/filedelegate.h"
#include "Admin_GUI/Widgets/qtmaterialdialog.h"
#include "Admin_GUI/Views/filedialogwidget.h"

enum ICONS_PANEL_TYPE{
    USER_DESKTOP,
    ROLE_DESKTOP
};

class DesktopPanel: public QWidget
{
    Q_OBJECT

public:

    DesktopPanel(Terminal *terminal, ICONS_PANEL_TYPE type, QWidget *parent);
    ~DesktopPanel();

    void setParam(const QString &param, QStringList *users);//userName || role

public slots:

    void setDefaultRoleApps(const QString &role);

private:

    void initServices(Terminal *terminal);
    void initUI();
    void applyStyle();
    void initModel();
    void insertWidgetsIntoLayout();
    void createConnections();

private:

    ICONS_PANEL_TYPE m_type;

    FileExplorer *m_rootFileService;

    QStringList *m_usersList;

    QStandardItemModel *m_model;

    FileDelegate *m_fileDelegate;

    QString m_pararm;

    QString m_selectedItemName;

    QVBoxLayout *m_mainLayout;

    QLabel *m_programsToRun;

    QListView *m_allProgramsListView;

    QHBoxLayout *m_bottomLayout;

    QPushButton *m_addProgramButton;

    QPushButton *m_deleteProgramButton;


    QtMaterialDialog *m_dialog;

    QVBoxLayout *m_dialogLayout;

    FileDialogWidget *m_dialogWidget;

private:

    void updateAllUsersWithCurrentRole();

private slots:

    void addProgram(const QString &exec, const QString &iconPath, const QString &iconName);
    void deleteProgram();
    void onProgramSelect(const QModelIndex &index);

};

#endif // ADMIN_GUI_VIEWS_DESKTOPPANEL_H
