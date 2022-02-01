#ifndef ADMIN_GUI_VIEWS_TOPBAR_H
#define ADMIN_GUI_VIEWS_TOPBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>

#include "Structs/userstruct.h"

class TopLeftBar : public QWidget
{
    Q_OBJECT

public:
    explicit TopLeftBar(QWidget *parent);
    ~TopLeftBar();

private:
    void CreateUI();
    void InsertWidgetsIntoLayouts();
    void InitUI();

public:
    void SetData(const QString &rank, const QString &FCS, const int &role);

protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    bool IsNeedToUpdateFontSize(int width);
    void UpdateFontSize();

private:
    QFont m_titleFont;
    int m_oldFontSize;

private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_currentUserNameLabel;
    QLabel *m_currentUseRoleLabel;
};

#endif // ADMIN_GUI_VIEWS_TOPBAR_H
