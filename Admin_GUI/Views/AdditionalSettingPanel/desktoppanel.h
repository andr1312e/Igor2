#ifndef ADMIN_GUI_VIEWS_DESKTOPPANEL_H
#define ADMIN_GUI_VIEWS_DESKTOPPANEL_H

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QListView>
#include <QPushButton>
#include <QSharedPointer>
#include <QLineEdit>
#include <QDebug>

#include "Structs/programstruct.h"
#include "Services/Sql/isqlservice.h"

#include "Services/userdesktopservice.h"
#include "Services/roledesktopservice.h"

#include "Admin_GUI/Delegates/filedelegate.h"
#include "Admin_GUI/Widgets/qtmaterialdialog.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/DialogWidgets/filedialogwidget.h"
#include "Admin_GUI/Views/AdditionalSettingPanel/mylistview.h"

enum ICONS_PANEL_TYPE {
    USER_ICONS,
    ROLE_ICONS
};

class DesktopPanel: public QWidget
{
    Q_OBJECT
public:
    explicit DesktopPanel(const ICONS_PANEL_TYPE type, UserDesktopService * userDesktopService, RoleDesktopService * roleDesktopService, QWidget *parent);
    ~DesktopPanel();

private:
    void CreateUI();
    void FillUI();
    void SetBackgroundColor();
    void SetPresenterModelToView();
    void InsertWidgetsIntoLayout();
    void ConnectObjects();

private Q_SLOTS:
    void OnAddProgram(const QString &exec, const QString &iconPath, const QString &iconName);
    void OnDeleteProgram();
    void OnProgramSelect(const QModelIndex &index);

public:
    void SetUser(const User &user);
    void SetRoleId(int roleId);//userName || role

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    virtual void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
private:
    void InsertDragItem(const DesktopEntity &entity);
    bool IsUserData() const;
private:
    int m_currentRoleId;
    const ICONS_PANEL_TYPE m_type;
    UserDesktopService* const m_userDesktopService;
    RoleDesktopService* const m_roleDesktopService;

    QStandardItemModel *m_model;
    FileDelegate *m_fileDelegate;
    QString m_userName;

private:

    QVBoxLayout *m_mainLayout;

    QLabel *m_programsToRun;
    QListView *m_allProgramsListView;

    QHBoxLayout *m_bottomLayout;
    QPushButton *m_addProgramButton;
    QPushButton *m_deleteProgramButton;

    QtMaterialDialog *m_dialog;
    QVBoxLayout *m_dialogLayout;
    DesktopUploadDialogWidget *m_dialogWidget;
};

#endif // ADMIN_GUI_VIEWS_DESKTOPPANEL_H
