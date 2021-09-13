#ifndef ADMIN_GUI_MODEL_USERMODEL_H
#define ADMIN_GUI_MODEL_USERMODEL_H
#include <QList>

#include "Structs/userstruct.h"

#include "Services/linuxuserservice.h"
#include "Services/databaseservice.h"

class UserModel : public QObject
{
    Q_OBJECT
public:

    UserModel(LinuxUserService *linuxUserService, DatabaseService *databaseService);

    ~UserModel();

    QStandardItemModel* getModel(){return m_model;}

    QStringList* getUserNamesByRole(const QString &role);

public slots:

    void dataChanged();

private:

    QStandardItemModel *m_model;

    DatabaseService *m_databaseService;

    LinuxUserService *m_linuxUserService;

    QList<User> *m_users;

    QStringList *m_currentRoleUsers;

private:

    void FillListByLinuxUserService();

    void FillListByDatabaseService();

    void FillModelByList();

    void getUserImageToList(User &user);

};

#endif // ADMIN_GUI_MODEL_USERMODEL_H
