#include "databaseservice.h"
#include <QDebug>

DatabaseService::DatabaseService(const QString &filePath, Terminal *terminal)
    : m_filePath(filePath)
    , m_terminal(terminal)
    , m_file(new QFile(m_filePath))
{
    if (m_file->open(QIODevice::ReadOnly))
    {
        m_array= m_file->readAll();
        m_file->close();
        m_array= QByteArray::fromHex(m_array);
        m_accountsData.setContent(m_array);
    }
}

DatabaseService::~DatabaseService()
{
    delete m_file;
}


void DatabaseService::getRLSTIAdminsUserName(QStringList &adminsUserNames)
{
    QDomElement users=m_accountsData.elementsByTagName("USERS").at(0).toElement();
    QDomNodeList userList = users.elementsByTagName("user");
    for (int i=0; i<userList.count(); i++)
    {
        QDomElement user=userList.at(i).toElement();
        if (user.attribute("role")==Roles.constLast())
        {
            adminsUserNames.append(user.attribute("name"));
        }
    }
}

void DatabaseService::fillUserFromList(User &currentUser)
{
    QDomElement users=m_accountsData.elementsByTagName("USERS").at(0).toElement();
    QDomNodeList usersList = users.elementsByTagName("user");
    for (int i=0; i<usersList.count(); i++)
    {
        QDomElement user=usersList.at(i).toElement();
        if (user.attribute("userId")==currentUser.userId)
        {
            currentUser.hasData=true;
            currentUser.FCS=user.attribute("FCS");
            currentUser.rank=user.attribute("rank");
            currentUser.role=user.attribute("role" );
            return;
        }
    }
    currentUser.hasData=false;
}

QString DatabaseService::getUserAttributeByLinuxUserNameToList(const QString &userName, const QString &attribute)
{
    QDomElement users=m_accountsData.elementsByTagName("USERS").at(0).toElement();
    QDomNodeList usersList = users.elementsByTagName("user");
    for (int i=0; i<usersList.count(); i++)
    {
        QDomElement user=usersList.at(i).toElement();
        if (user.attribute("name")==userName)
        {
            return user.attribute(attribute);
        }
    }
    return "";
//    qFatal("Нет пользователя DatabaseService::getUserAttributeByLinuxUserName");
}

QDomElement DatabaseService::createUserElement(QDomDocument &users,const User &user)
{
    QDomElement userElement=users.createElement("user");
    userElement.setAttribute("userId", user.userId);
    userElement.setAttribute("name", user.name);
    userElement.setAttribute("FCS", user.FCS);
    userElement.setAttribute("rank", user.rank);
    userElement.setAttribute("role", user.role);
    return userElement;
}

void DatabaseService::writeToFile(QList<User> *users)
{
    QDomDocument documentToWrite;
    QDomElement domElement=documentToWrite.createElement("USERS");
    documentToWrite.appendChild(domElement);
    for(QList<User>::ConstIterator it=users->begin(); it!=users->end(); ++it)
    {
        QDomElement userDomElement=createUserElement(documentToWrite, *it);
        domElement.appendChild(userDomElement);
    }
//    qDebug()<< m_filePath;
//    QFile file(m_filePath+"32");
//    if(file.open(QIODevice::WriteOnly))
//    {
//        QTextStream(&file)<<documentToWrite.toString();
//    }
}
