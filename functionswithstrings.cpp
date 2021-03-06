#include "functionswithstrings.h"

static const std::array<char, 16> badCharecters =
{
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
/**
 * Проверка на наличие плохих символов для консоли
 */
bool FunctionsWithStrings::StringContainsBadCharecters(const QString &fileName)
{
    for (const char charecter : badCharecters)
    {
        if (fileName.contains(charecter))
        {
            return true;
        }
    }
    return false;
}
/**
 * Парсим путь папки на лист из имен папок
 */
QStringList FunctionsWithStrings::GetListofSubFoldersFromPath(const QString &path, const char *calledFunc)
{
    QStringList listofSubFoldersToPath = path.split('/');
    listofSubFoldersToPath.removeAll(QString());

    if (listofSubFoldersToPath.isEmpty())
    {
        qFatal("Работа невозможна метод: %s выдал пуcтой путь. Путь %s", calledFunc, path.toUtf8().constData());
    }
    else
    {
        return listofSubFoldersToPath;
    }
}

