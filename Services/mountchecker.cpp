#include "mountchecker.h"

MountChecker::MountChecker()
    : m_terminal(Terminal::GetTerminal())
    , m_bootFolder(QLatin1Literal("/opt/boot"))
    , m_devFolder(QLatin1Literal("/opt/dev"))
    , m_bootIsoPaths{QLatin1Literal("/opt/smolensk-1.6.iso"), QLatin1Literal("/opt/boot/boot.iso"), QLatin1Literal("/opt/boot.iso")}
    , m_devIsoPaths{QLatin1Literal("/opt/devel-smolensk-1.6.iso"), QLatin1Literal("/opt/dev/dev.iso"), QLatin1Literal("/opt/dev.iso")}
    , m_registerFilePath(QLatin1Literal("/etc/apt/sources.list"))
    , m_bootFileState(MountFilesState::NoFiles)
    , m_devFileState(MountFilesState::NoFiles)
{
    Q_STATIC_ASSERT(static_cast<int>(MountFilesState::NoFiles) == 3);
    Q_STATIC_ASSERT(static_cast<int>(MountFilesState::Mounted) == 4);
    Q_ASSERT(static_cast<int>(MountFilesState::NoFiles) == m_devIsoPaths.count());
    Q_ASSERT(static_cast<int>(MountFilesState::NoFiles) == m_bootIsoPaths.count());
}

MountChecker::~MountChecker()
{

}
/**
 * Проверяем наличие файлов монтировки и выдаем есть ли оба файла. В локальные енумы записываем стейт в зависимости от имени файла монтирования
 */
bool MountChecker::HasMountFiles()
{
    CheckAndCreateMountFolder();
    if (m_terminal->IsFileExists(m_bootIsoPaths.front(), Q_FUNC_INFO, true))
    {
        m_bootFileState = MountFilesState::OriginalNames;
    }
    else
    {
        if (m_terminal->IsFileExists(m_bootIsoPaths.at(1), Q_FUNC_INFO, true))
        {
            m_bootFileState = MountFilesState::ShortInSubFolder;
        }
        else
        {
            if (m_terminal->IsFileExists(m_bootIsoPaths.back(), Q_FUNC_INFO, true))
            {
                m_bootFileState = MountFilesState::ShortInOpt;
            }
            else
            {
                if (m_terminal->IsFileExists(QLatin1Literal("/opt/boot/smolensk-1.6.info"), Q_FUNC_INFO, true))
                {
                    m_bootFileState = MountFilesState::Mounted;
                }
                else
                {
                    m_bootFileState = MountFilesState::NoFiles;
                    return false;
                    //2 уже не ищем, так как первого нет              }
                }
            }
        }
    }

    if (m_terminal->IsFileExists(m_devIsoPaths.front(), Q_FUNC_INFO, true))
    {
        m_devFileState = MountFilesState::OriginalNames;
    }
    else
    {
        if (m_terminal->IsFileExists(m_devIsoPaths.at(1), Q_FUNC_INFO, true))
        {
            m_devFileState = MountFilesState::ShortInSubFolder;
        }
        else
        {
            if (m_terminal->IsFileExists(m_devIsoPaths.back(), Q_FUNC_INFO, true))
            {
                m_devFileState = MountFilesState::ShortInOpt;
            }
            else
            {
                if (m_terminal->IsFileExists("/opt/dev/smolensk-1.6.info", Q_FUNC_INFO, true))
                {
                    m_devFileState = MountFilesState::Mounted;
                }
                else
                {
                    m_devFileState = MountFilesState::NoFiles;
                    return false;
                }
            }
        }
    }
    return true;
}

bool MountChecker::IsReposiotoryMounted(const char *calledFunc)
{

    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Проверяем примонтированы ли репозитории"));
    if (m_bootFileState == MountFilesState::NoFiles || m_devFileState == MountFilesState::NoFiles)
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Невозможно понять примонтированы ли репозитории так как файлы не найдены")).toUtf8().constData());
    }
    else
    {
        if (MountFilesState::Mounted == m_bootFileState)
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" boot примонтирован"));
            m_isBootMounted = true;
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" boot надо проверить"));
            const int firstFileNameIndex = static_cast<int>(m_bootFileState);
            Q_ASSERT(firstFileNameIndex < m_bootIsoPaths.count());
            const QString bootOut = m_terminal->RunConsoleCommandSync("mount | grep /opt/boot", calledFunc);
            const QStringList moutedBootList = bootOut.split('\n');
            for (const QString &device : moutedBootList)
            {
                if (device.startsWith(m_bootIsoPaths.at(firstFileNameIndex)))
                {
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" boot примонтирован"));
                    m_isBootMounted = true;
                    break;
                }
            }
        }
        if (MountFilesState::Mounted == m_devFileState)
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" dev примонтирован"));
            m_isDevMounted = true;
        }
        else
        {
            const int secondFileNameIndex = static_cast<int>(m_devFileState);
            Q_ASSERT(secondFileNameIndex < m_devIsoPaths.count());
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" dev надо проверить"));
            const QString devOut = m_terminal->RunConsoleCommandSync("mount | grep /opt/dev", calledFunc);
            const QStringList mountedDevList = devOut.split('\n');
            for (const QString &device : mountedDevList)
            {
                if (device.startsWith(m_devIsoPaths.at(secondFileNameIndex)))
                {
                    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" dev примонтирован"));
                    m_isDevMounted = true;
                    break;
                }
            }
        }
        return m_isBootMounted && m_isDevMounted;
    }
}

void MountChecker::MountRepository()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" монтируем репозитории"));
    const int firstFileNameIndex = static_cast<int>(m_bootFileState);
    const int secondFileNameIndex = static_cast<int>(m_devFileState);
    if (!m_isBootMounted)
    {
        MountRepository(m_bootIsoPaths.at(firstFileNameIndex), QLatin1Literal("/opt/boot"));
    }
    if (!m_isDevMounted)
    {
        MountRepository(m_devIsoPaths.at(secondFileNameIndex), QLatin1Literal("/opt/dev"));
    }
    RegisterMounting();
    UpdateAptGet();
}

void MountChecker::UnMountRepository()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" размонтируем репозитории"));
    if (m_isBootMounted)
    {
        UnMountRepository(QLatin1Literal("/opt/boot"));
    }
    if (m_isDevMounted)
    {
        UnMountRepository(QLatin1Literal("/opt/dev"));
    }
}

void MountChecker::RegisterMounting()
{
    QStringList neededLinesRegister = {QLatin1Literal("deb file:///opt/boot smolensk contrib main non-free"),
                                       QLatin1Literal("deb file:///opt/dev smolensk contrib main non-free")
                                      };
    if (m_terminal->IsFileNotExists(m_registerFilePath, Q_FUNC_INFO, true))
    {
        m_terminal->CreateFile(m_registerFilePath, Q_FUNC_INFO, true);
    }
    const QString sourcesListData = m_terminal->GetFileText(m_registerFilePath, Q_FUNC_INFO, true);
    const QStringList sourcesFileLines = sourcesListData.split('\n');
    for (const QString &line : sourcesFileLines)
    {
        const int itemPosition = neededLinesRegister.indexOf(line);
        if (itemPosition > -1)
        {
            neededLinesRegister.removeAt(itemPosition);
        }
    }
    for (const QString &neededLine : neededLinesRegister)
    {
        m_terminal->AppendTextToFileSudo(neededLine, m_registerFilePath, Q_FUNC_INFO);
    }
}

void MountChecker::MountRepository(const QString &isoPath, const QString &path)
{
    QString terminalOut, terminalError;
    m_terminal->RunConsoleCommandSync(QLatin1Literal("sudo mount -r -o loop ") + isoPath + ' ' + path, terminalOut, terminalError);
    if (!terminalError.isEmpty())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не можем примонтировать ") + path + ' ' + terminalError + QStringLiteral(" путь: ") + isoPath);
    }
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Замонтирован ") + path + ' ' + terminalOut + QStringLiteral(" путь: ") + isoPath);
}

void MountChecker::CheckAndCreateMountFolder()
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Проверяем папки для монтировки "));
    if (m_terminal->IsDirNotExists(m_bootFolder, Q_FUNC_INFO, true))
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Папки boot нет. Создаем "));
        m_terminal->CreateFolder(m_bootFolder, Q_FUNC_INFO, true);
    }

    if (m_terminal->IsDirNotExists(m_devFolder, Q_FUNC_INFO, true))
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Папки dev нет. Создаем "));
        m_terminal->CreateFolder(m_devFolder, Q_FUNC_INFO, true);
    }
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Папки монтировки в наличии"));
}

void MountChecker::UpdateAptGet()
{
    QString terminalOut, terminalError;
    m_terminal->RunConsoleCommandSync(QLatin1Literal("sudo apt-get update"), terminalOut, terminalError);
    if (terminalError.isEmpty())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Обновили пакеты. Вывод ") + terminalOut);
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Обновили пакеты. Ошибка ") + terminalError);
    }
}

void MountChecker::UnMountRepository(const QString &path)
{
    QString terminalOut, terminalError;
    m_terminal->RunConsoleCommandSync(QLatin1Literal("sudo umount ") + path, terminalOut, terminalError);
    if (terminalError.isEmpty())
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Размонтировали ") + path + ' ' + terminalOut);
    }
    else
    {
        Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Не можем размонтировать ") + path + ' ' + terminalError);
    }
}
