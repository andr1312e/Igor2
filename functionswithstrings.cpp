#include "functionswithstrings.h"

static const QStringList badCharecters = {
   "\\",
   "/",
   "`",
   ":",
   "&",
   "*",
   "?",
   "\"",
   "<",
   ">",
   "`",
   "(",
   ")",
   ":",
   ";",
   "\'",
};

bool FunctionsWithStrings::StringContainsBadCharecters(const QString &fileName)
{
   for (QStringList::const_iterator it = badCharecters.cbegin(); it != badCharecters.cend(); ++it) {
      if (fileName.contains(*it)) {
         return true;
      }
   }

   return false;
}

QStringList FunctionsWithStrings::GetListofSubFoldersFromPath(const QString &path, QString calledFunc)
{
   QStringList listofSubFoldersToPath = path.split('/');
   listofSubFoldersToPath.removeAll("");

   if (listofSubFoldersToPath.isEmpty()) {
      qFatal("Работа невозможна метод: %s выдал пуcтой путь. Путь %s", calledFunc.toLatin1().constData(), path.toLatin1().constData());
   } else {
      return listofSubFoldersToPath;
   }
}

