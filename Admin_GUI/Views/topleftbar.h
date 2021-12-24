#ifndef ADMIN_GUI_VIEWS_TOPBAR_H
#define ADMIN_GUI_VIEWS_TOPBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QResizeEvent>

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
    void SetData(QString &rank, QString &FCS, QString &role);

protected:
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    bool IsNeedToUpdateFontSize(int width);
    void UpdateFontSize();
    void ChangeWidgetLayout();

private:
    QFont *m_titleFont;
    int m_oldFontSize;

private:
    QVBoxLayout *m_mainLayout;
    QLabel *m_currentUserNameLabel;
    QLabel *m_currentUseRoleLabel;
};

#endif // ADMIN_GUI_VIEWS_TOPBAR_H
