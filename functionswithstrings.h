#ifndef FUNCTIONSWITHSTRINGS_H
#define FUNCTIONSWITHSTRINGS_H

#include <QString>
#include <QStringList>

class FunctionsWithStrings
{

public:
   static bool StringContainsBadCharecters(const QString &fileName);
   static QStringList GetListofSubFoldersFromPath(const QString &path, QString calledFunc);
};

#endif // FUNCTIONSWITHSTRINGS_H
