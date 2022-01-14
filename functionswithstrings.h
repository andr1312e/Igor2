#ifndef FUNCTIONSWITHSTRINGS_H
#define FUNCTIONSWITHSTRINGS_H

#include <QString>
#include <array>
#include <QStringList>

class FunctionsWithStrings
{

public:
   static bool StringContainsBadCharecters(const QString &fileName);
   static QStringList GetListofSubFoldersFromPath(const QString &path, const char *calledFunc);
};

#endif // FUNCTIONSWITHSTRINGS_H
