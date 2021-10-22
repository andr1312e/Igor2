#include "usersettingsrepository.h"

UsersDataWizardRepository::UsersDataWizardRepository(QString &curerntUserName)
   : m_curerntUserName(curerntUserName)
   , m_hasData(false)
   , m_userFCS(QStringLiteral(""))
   , m_userRank(QStringLiteral(""))
{

}

UsersDataWizardRepository::~UsersDataWizardRepository()
{

}

const QString &UsersDataWizardRepository::GetCurrentUserName() const
{
   return m_curerntUserName;
}

QString &UsersDataWizardRepository::GetCurrentUserFCS()
{
   return m_userFCS;
}

QString &UsersDataWizardRepository::GetCurrentUserRank()
{
   return m_userRank;
}

QVector<User> &UsersDataWizardRepository::GetUsersList()
{
   return m_usersList;
}

int UsersDataWizardRepository::GetUserCount()
{
   return m_usersList.count();
}

bool UsersDataWizardRepository::HasData() const
{
   return m_hasData;
}

void UsersDataWizardRepository::SetFCSAndRolesFromDb(QString &pathToUserDb)
{
   QFile file(pathToUserDb);

   if (file.open(QIODevice::ReadOnly)) {
      QByteArray arr = file.readAll();
      QByteArray decrypted = QByteArray::fromHex(arr);
      QDomDocument doc;
      doc.setContent(decrypted);
      QDomElement svg = doc.firstChildElement();
      GetFCSAndRolesFromXml(svg);
   }
}

void UsersDataWizardRepository::GetFCSAndRolesFromXml(QDomElement &usersNode)
{
   m_usersList.clear();

   if (usersNode.tagName() == "USERS") {
      QDomNodeList usersTags = usersNode.childNodes();
      int usersCount = usersTags.count();

      if (usersCount > 0) {
         m_usersList.resize(usersCount);

         for (int i = 0; i < usersCount; ++i) {
            QDomElement userElement = usersTags.at(i).toElement();

            if (userElement.attribute("name") == m_curerntUserName) {
               m_userFCS = userElement.attribute("FCS");
               m_userRank = userElement.attribute("rank");
            }

            User user;
            user.FCS = userElement.attribute("FCS");
            user.name = userElement.attribute("name");
            user.rank = userElement.attribute("rank");
            user.role = userElement.attribute("role");
            user.userId = userElement.attribute("userId");
            m_usersList[i] = user;
         }

         m_hasData = true;
      }
   }
}

void UsersDataWizardRepository::WriteToFile(Terminal *terminal, QString &pathToWritingDb)
{

}
