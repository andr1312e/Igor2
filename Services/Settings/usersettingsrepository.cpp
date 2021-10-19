#include "usersettingsrepository.h"

CurrentUserWizardRepository::CurrentUserWizardRepository(QString &curerntUserName)
    : m_curerntUserName(curerntUserName)
    , m_hasData(false)
    , m_usersCount(0)
    , m_userFCS(QStringLiteral(""))
    , m_userRank(QStringLiteral(""))
{

}

CurrentUserWizardRepository::~CurrentUserWizardRepository()
{

}

const QString &CurrentUserWizardRepository::getCurrentUserName() const
{
    return m_curerntUserName;
}

QString &CurrentUserWizardRepository::GetCurrentUserFCS()
{
    return m_userFCS;
}

QString& CurrentUserWizardRepository::GetCurrentUserRank()
{
    return m_userRank;
}

int CurrentUserWizardRepository::GetUsersCount() const
{
    return m_usersCount;
}

bool CurrentUserWizardRepository::hasData() const
{
    return m_hasData;
}

void CurrentUserWizardRepository::setFCSAndRolesFromDb(QString &pathToUserDb)
{
    QFile file(pathToUserDb);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray arr=file.readAll();
        QByteArray decrypted=QByteArray::fromHex(arr);
        QDomDocument doc;
        doc.setContent(decrypted);
        QDomElement svg=doc.firstChildElement();
        getFCSAndRolesFromXml(svg);
    }
}

void CurrentUserWizardRepository::getFCSAndRolesFromXml(QDomElement &usersNode)
{
    if (usersNode.tagName()=="USERS")
    {
        QDomNodeList users=usersNode.childNodes();
        for (int i=0; i<users.count(); i++)
        {
            QDomElement user=users.at(i).toElement();
            if (user.tagName()=="user")
            {
                if (user.attribute("name")==m_curerntUserName)
                {
                    m_userFCS=user.attribute("FCS");
                    m_userRank=user.attribute("rank");
                }
                m_hasData=true;
            }
        }
        if (m_hasData==true)
        {
            m_usersCount=users.count();
        }
    }
}
