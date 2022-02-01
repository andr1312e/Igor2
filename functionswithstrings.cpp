#include "functionswithstrings.h"

static const QVarLengthArray<QChar, 16> badCharecterList = {
    '\\',
    '/',
    '`',
    ':',
    '&',
    '*',
    '?',
    '\'',
    '<',
    '>',
    '`',
    '(',
    ')',
    ':',
    ';',
    '\'',
};

bool FunctionsWithStrings::StringContainsBadCharecters(const QString &fileName)
{
    for (const QChar &charecter:badCharecterList)
    {
        for(const QChar &fileNameCharecter: fileName)
        {
            if (charecter==fileNameCharecter)
            {
                return true;
            }
        }
    }
    return false;
}

QStringList FunctionsWithStrings::GetListofSubFoldersFromPath(const QString &path,const char* calledFunc)
{
    QStringList listofSubFoldersToPath = path.split(QChar('/'));
    listofSubFoldersToPath.removeAll("");

    if (listofSubFoldersToPath.isEmpty()) {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Работа невозможна метод: ") + calledFunc + QStringLiteral(" выдал пустой путь ") + path ).toUtf8().constData());
    } else {
        return listofSubFoldersToPath;
    }
}

