#include "databaseservice.h"
#include <QDebug>

DatabaseService::DatabaseService(Terminal *terminal)
    : m_terminal(terminal)
    , m_file(new QFile())
    , m_accountsData(new QDomDocument())
{

}

void DatabaseService::loadFromFile(const QString &userDbPath)
{
    m_userDbPath=userDbPath;
    m_file->setFileName(userDbPath);
    if (m_file->open(QIODevice::ReadOnly))
    {
        m_array= m_file->readAll();
        m_file->close();
        m_array= QByteArray::fromHex(m_array);
        m_accountsData->setContent(m_array);
    }
}

DatabaseService::~DatabaseService()
{
    delete m_file;
    delete m_accountsData;
}


void DatabaseService::getRLSTIAdminsUserName(QStringList &adminsUserNames)
{
    QDomElement users=m_accountsData->elementsByTagName("USERS").at(0).toElement();
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

void DatabaseService::readUserFromFile(User &currentUser)
{
    QDomElement users=m_accountsData->elementsByTagName("USERS").at(0).toElement();
    QDomNodeList usersList = users.elementsByTagName("user");
    for (int i=0; i<usersList.count(); i++)
    {
        QDomElement user=usersList.at(i).toElement();
        if (user.attribute("userId")==currentUser.userId)
        {
            if (user.attribute("rank")=="")
            {
                currentUser.hasData=false;
            }
            else
            {
                currentUser.hasData=true;
                currentUser.FCS=user.attribute("FCS");
                currentUser.rank=user.attribute("rank");
                currentUser.role=user.attribute("role" );
            }
            return;
        }
    }
    currentUser.hasData=false;
}

QString DatabaseService::getUserAttributeByLinuxUserNameToList(const QString &userName, const QString &attribute)
{
    QDomElement users=m_accountsData->elementsByTagName("USERS").at(0).toElement();
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
}

void DatabaseService::writeToFile(QList<User> *users)
{
    m_accountsData->clear();
    QDomElement domElement=m_accountsData->createElement("USERS");
    m_accountsData->appendChild(domElement);
    for(QList<User>::iterator it=users->begin(); it!=users->end(); ++it)
    {
        QDomElement userDomElement=createUserElement(*m_accountsData, *it);
        domElement.appendChild(userDomElement);
    }
    //    qDebug()<< m_filePath;
    QString text=m_accountsData->toString();
    QByteArray encrypt=text.toLocal8Bit().toHex();
    m_file->setFileName(m_userDbPath);
    QTextStream stream(m_file);
    if(m_file->open(QIODevice::WriteOnly))
    {
        stream<<encrypt;
    }
    stream.flush();
    m_file->close();
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
