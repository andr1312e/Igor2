#include "globalfunctions.h"

static const QStringList badCharecters={
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

bool GlobalFunctions::stringContainsBadCharecters(const QString &fileName)
{
    for (QStringList::const_iterator it=badCharecters.cbegin(); it!=badCharecters.cend(); ++it)
    {
        if (fileName.contains(*it))
        {
            return true;
        }
    }
    return false;
}


