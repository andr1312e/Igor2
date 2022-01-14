#include "User_GUI.h"

FakeUI::FakeUI(QWidget *parent)
    : QWidget(parent)
    , m_noApplicationExecBox(new QMessageBox())
{
}

FakeUI::~FakeUI()
{

}

void FakeUI::noExecApplication(const QString &execPath)
{
    m_noApplicationExecBox->show();
}
