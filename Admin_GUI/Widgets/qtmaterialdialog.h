#ifndef QTMATERIALDIALOG_H
#define QTMATERIALDIALOG_H

#include <QScopedPointer>
#include "Styles/MaterialWidgets/qtmaterialoverlaywidget.h"

class QLayout;
class QtMaterialDialogPrivate;

class QtMaterialDialog : public QtMaterialOverlayWidget
{
    Q_OBJECT

public:
    explicit QtMaterialDialog(QWidget *parent);
    ~QtMaterialDialog();

    QLayout *windowLayout() const;
    void setWindowLayout(QLayout *layout);

public Q_SLOTS:
    void OnShowDialog();
    void OnHideDialog();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtMaterialDialogPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtMaterialDialog)
    Q_DECLARE_PRIVATE(QtMaterialDialog)
};

#endif // QTMATERIALDIALOG_H
