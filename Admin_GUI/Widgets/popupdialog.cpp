#include "popupdialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>

PopupDialog::PopupDialog(QWidget *parent) : QWidget(parent)
{
    QLabel *pLabel = new QLabel("This is a popup widget...", this);
    QHBoxLayout *pMainLayout = new QHBoxLayout(this);
    pLabel->setStyleSheet("QLabel { color: black; }");

    pMainLayout->addWidget(pLabel);
}

void PopupDialog::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
