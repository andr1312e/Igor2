#ifndef USER_GUI_VIEWS_USER_GUI_H
#define USER_GUI_VIEWS_USER_GUI_H

#include <QMessageBox>

class FakeUI : public QWidget
{
    Q_OBJECT
public:
    explicit FakeUI(QWidget *parent = nullptr);
    ~FakeUI();
private:

    QMessageBox *m_noApplicationExecBox;

public slots:

    void noExecApplication(const QString &execPath);
};

#endif // USER_GUI_VIEWS_USER_GUI_H
