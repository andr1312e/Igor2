#include "usersettingsrepository.h"

UsersDataWizardRepository::UsersDataWizardRepository(const QString &curerntUserName, const QString &curerntUserId, Terminal *terminal)
   : m_curerntUserName(curerntUserName)
   , m_curerntUserId(curerntUserId)
   , m_terminal(terminal)
   , m_hasData(false)
   , m_userFCS("")
   , m_userRank("")
   , m_accountsData(new QDomDocument())
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

int UsersDataWizardRepository::GetUserCount() const
{
   return m_usersList.count();
}

bool UsersDataWizardRepository::HasData() const
{
   return m_hasData;
}

void UsersDataWizardRepository::SetFCSAndRolesFromFile(QString &pathToUserDb)
{
   if (m_terminal->IsFileExists(pathToUserDb, "UsersDataWizardRepository::SetFCSAndRolesFromDb", true)) {
      QString text = m_terminal->GetFileText(pathToUserDb, "UsersDataWizardRepository::SetFCSAndRolesFromDb", true);
      QByteArray arr;
      arr += text;
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

void UsersDataWizardRepository::WriteUserRepositoryToFile(const QString &pathToWriteDb, bool adminOnly)
{
   if (m_terminal->IsFileNotExists(pathToWriteDb, "UsersDataWizardRepository::WriteToFile", true)) {
      m_terminal->CheckAndCreatePathToElement(pathToWriteDb, "UsersDataWizardRepository::WriteToFile", true);
      m_terminal->CreateFile(pathToWriteDb, "UsersDataWizardRepository::WriteToFile", true);
   } else {
      m_terminal->ClearFileSudo(pathToWriteDb, "UsersDataWizardRepository::WriteToFile");
   }

   CreateMainTag();

   if (adminOnly) {
      WriteAdminToDomDocument();
   } else {
      if (!FindAndUpdateAdminData()) {
         AppendAdminToList();
      }

      WriteAllUsersToDomDocument();
   }

   EncryptAndWriteToFile(pathToWriteDb);
}

bool UsersDataWizardRepository::FindAndUpdateAdminData()
{
   for (QVector<User>::iterator it = m_usersList.begin(); it != m_usersList.end(); ++it) {
      if (it->name == m_curerntUserName) {
         it->FCS = m_userFCS;
         it->rank = m_userRank;
         return true;
      }
   }

   return false;
}

void UsersDataWizardRepository::AppendAdminToList()
{
   User user;
   user.FCS = m_userFCS;
   user.rank = m_userRank;
   user.role = Roles.at(Roles.count() - 1);
   user.userId = m_curerntUserId;
   user.name = m_curerntUserName;
   m_usersList.append(user);
}

void UsersDataWizardRepository::CreateMainTag()
{
   QDomElement mainElement = m_accountsData->createElement("USERS");
   m_accountsData->appendChild(mainElement);
}

void UsersDataWizardRepository::WriteAllUsersToDomDocument()
{
   for (QVector<User>::const_iterator it = m_usersList.cbegin(); it != m_usersList.cend(); ++it) {
      AppendUserToDomDocument(*m_accountsData, *it);
   }
}

void UsersDataWizardRepository::WriteAdminToDomDocument()
{
   QDomElement userElement = m_accountsData->createElement("user");
   userElement.setAttribute("userId", m_curerntUserId);
   userElement.setAttribute("name", m_curerntUserName);
   userElement.setAttribute("FCS", m_userFCS);
   userElement.setAttribute("rank", m_userRank);
   userElement.setAttribute("role", Roles.at(Roles.count() - 1));
   m_accountsData->firstChildElement().appendChild(userElement);
}

void UsersDataWizardRepository::AppendUserToDomDocument(QDomDocument &document, const User &user)
{
   QDomElement userElement = document.createElement("user");
   userElement.setAttribute("userId", user.userId);
   userElement.setAttribute("name", user.name);
   userElement.setAttribute("FCS", user.FCS);
   userElement.setAttribute("rank", user.rank);
   userElement.setAttribute("role", user.role);
   document.firstChildElement().appendChild(userElement);
}

void UsersDataWizardRepository::EncryptAndWriteToFile(const QString &pathToWriteDb)
{
   QString text = m_accountsData->toString();
   QByteArray encrypt = text.toLatin1().toHex();
   m_terminal->WriteTextToFileSudo(encrypt, pathToWriteDb, "UsersDataWizardRepository::WriteToFile");
}
