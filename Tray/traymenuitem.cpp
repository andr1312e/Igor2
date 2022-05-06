#include "traymenuitem.h"

#include <QPainterPath>

TrayMenuItem::TrayMenuItem(QWidget *parent)
    : InteractiveButtonBase(parent)
{

}

TrayMenuItem::TrayMenuItem(const QString &text, QWidget *parent)
    : InteractiveButtonBase(text, parent)
{

}

TrayMenuItem::TrayMenuItem(const QIcon &pixmapPath, QWidget *parent)
    : InteractiveButtonBase(pixmapPath, parent)
{

}

TrayMenuItem::TrayMenuItem(const QIcon &pixmapPath, const QString &text, QWidget *parent)
    : InteractiveButtonBase(pixmapPath, text, parent)
{

}

TrayMenuItem::~TrayMenuItem()
{

}

void TrayMenuItem::SetIconPath(const QString &imagePath)
{
    m_pixmapPath=imagePath;
    setIcon(QIcon(imagePath));
}

void TrayMenuItem::SetTooltipText(const QString &tooltipText)
{
  QWidget::setToolTip(tooltipText);
}

bool TrayMenuItem::IsChecked()
{
    return InteractiveButtonBase::getState();
}
