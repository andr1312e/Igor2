#include "Admin_GUI/RestoreWizard/Services/enviromentalvariables.h"

EnviromentalVariablesService::EnviromentalVariablesService()
    : m_proFilePath(QStringLiteral("/etc/profile"))
    , m_terminal(Terminal::GetTerminal())
{

}


EnviromentalVariablesService::~EnviromentalVariablesService()
{

}
/**
 * Парсим данные из домэлеметна добавляя их в хэш контейнер
 */
void EnviromentalVariablesService::GetProfileDataFromBackUp(const QDomElement &profileElem)
{
    ClearProFiles();
    if (QLatin1Literal("PROFILES") == profileElem.tagName())
    {
        const QDomNodeList profileElems = profileElem.childNodes();
        for (int i = 0; i < profileElems.count(); ++i)
        {
            const QString line = profileElems.at(i).toElement().text();
            QPair<QString, QString> keyAndVal = GetKeyAndVal(line);
            m_profileKeyWithData.append(qMakePair(keyAndVal.first, keyAndVal.second));
        }
    }
    else
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Получили тэг с неверным именем. Должен быть PROFILE а имеем ") + profileElem.tagName()).toUtf8().constData());
    }
}
/**
 * Отчищаем данные профайла из парсера
 */
void EnviromentalVariablesService::ClearProFiles()
{
    m_profileKeyWithData.clear();
}
/**
 * Добавляем в профайл данные с заменой ключей уже существующих
 */
void EnviromentalVariablesService::AppendProfileDataInfo()
{
    if (!m_profileKeyWithData.isEmpty())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Есть переменные окружения "));
        if (m_terminal->IsFileExists(m_proFilePath, Q_FUNC_INFO, true))
        {

            const QString proFileData = m_terminal->GetFileText(m_proFilePath, Q_FUNC_INFO, true);
            if (proFileData.isEmpty())
            {
                Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Про файл пуст. Пишем все строки в него: ") + m_proFilePath);
                WriteItemsToProFile();
            }
            else
            {
                RemoveDublicates(proFileData);
                Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Про файл изменен. Пишем измененный текст в профайл: ") + m_proFilePath);
                for (const QPair<QString, QString> &newKeys : qAsConst(m_profileKeyWithData))
                {
                    const QString newProFileData = "export " + newKeys.first + '=' + newKeys.second;
                    m_terminal->AppendTextToFileSudo(newProFileData, m_proFilePath, Q_FUNC_INFO);
                }

            }
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Файла ") + m_proFilePath + QStringLiteral(" нет"));
            CreateProFile();
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Записываем все данные в файл: ") + m_proFilePath);
            WriteItemsToProFile();
        }
    }
    ApplyEtc();
}

const QList<QPair<QString, QString> > EnviromentalVariablesService::GetAllKeys() const noexcept
{
    return m_profileKeyWithData;
}
/**
 * Получаем из строки ключ значние по индексу оператора =
 */
QPair<QString, QString> EnviromentalVariablesService::GetKeyAndVal(const QString &line) const noexcept
{
    QPair<QString, QString> keyAndValue;
    const int index = line.indexOf('=');
    if (-1 == index)
    {
        keyAndValue.first = line;
    }
    else
    {
        keyAndValue.first = line.left(index);
        keyAndValue.second  = line.right(line.count() - index - 1);
    }
    return keyAndValue;
}
/**
 * Преобразуем текст профайла(весь) к новому типу, используя совпадение ключей в хэш таблице
 */
void EnviromentalVariablesService::RemoveDublicates(const QString &oldFileText)
{
    const QStringList lines = oldFileText.split('\n');
    for (const QString &line : qAsConst(lines))
    {
        const QPair<QString, QString> keyAndVal = GetKeyAndVal(line);
        const int index = HasItem(keyAndVal.first);
        if (index > -1)
        {
            m_profileKeyWithData.removeAt(index);
        }
    }
}
/**
 * создаем файл etc/profile
 */
void EnviromentalVariablesService::CreateProFile()
{
    m_terminal->CreateFile(m_proFilePath, Q_FUNC_INFO, true);
}
/**
 * записываем все имеющиеся данные в ect profile
 */
void EnviromentalVariablesService::WriteItemsToProFile()
{
    QString text;
    for (const QPair<QString, QString> &item : qAsConst(m_profileKeyWithData))
    {
        text += item.first + '=' + item.second + '\n';
    }
    m_terminal->AppendTextToFileSudo(text, m_proFilePath, Q_FUNC_INFO);
}

int EnviromentalVariablesService::HasItem(const QString &key) const noexcept
{
    const QString newKey=key.mid(7); //удаяем export
    for (int i = 0; i < m_profileKeyWithData.count(); ++i)
    {
        const QString backupKey=m_profileKeyWithData.at(i).first;
        if (newKey == backupKey)
        {
            return i;
        }
    }
    return -1;
}

void EnviromentalVariablesService::ApplyEtc()
{
    m_terminal->RunConsoleCommandSync(QLatin1Literal("source ")+m_proFilePath, Q_FUNC_INFO);
}
