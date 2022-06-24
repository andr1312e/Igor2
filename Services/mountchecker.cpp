#include "mountchecker.h"

MountChecker::MountChecker()
    : m_terminal(Terminal::GetTerminal())
    , m_bootIsoPaths{QLatin1Literal("/opt/smolensk-1.6.iso"), QLatin1Literal("/opt/boot/boot.iso")}
    , m_devIsoPaths{QLatin1Literal("/opt/devel-smolensk-1.6.iso"), QLatin1Literal("/opt/dev/dev.iso")}
    , m_registerFilePath(QLatin1Literal("/etc/apt/sources.list"))
{

}

MountChecker::~MountChecker()
{

}

bool MountChecker::HasMountFiles()
{
    if (m_terminal->IsFileExists(m_bootIsoPaths.front(), Q_FUNC_INFO, true))
    {
        m_bootFileName = MountFilesName::Full;
    }
    else
    {
        if (m_terminal->IsFileExists(m_bootIsoPaths.back(), Q_FUNC_INFO, true))
        {
            m_bootFileName = MountFilesName::Short;
        }
        else
        {
            if(m_terminal->IsFileExists("/opt/boot/smolensk-1.6.info", Q_FUNC_INFO, true))
            {
                m_bootFileName = MountFilesName::Mounted;
            }
            else
            {
                m_bootFileName = MountFilesName::NoFiles;
                return false;
            }
        }
    }

    if (m_terminal->IsFileExists(m_devIsoPaths.front(), Q_FUNC_INFO, true))
    {
        m_devFileName = MountFilesName::Full;
    }
    else
    {
        if (m_terminal->IsFileExists(m_devIsoPaths.back(), Q_FUNC_INFO, true))
        {
            m_devFileName = MountFilesName::Short;
        }
        else
        {
            if(m_terminal->IsFileExists("/opt/dev/smolensk-1.6.info", Q_FUNC_INFO, true))
            {
                m_devFileName = MountFilesName::Mounted;
            }
            else
            {
                m_devFileName = MountFilesName::NoFiles;
                return false;
            }
        }
    }
    return true;
}

bool MountChecker::IsReposiotoryMounted(const char *calledFunc)
{
    Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" Проверяем примонтированы ли репозитории"));
    if (m_bootFileName == MountFilesName::NoFiles || m_devFileName == MountFilesName::NoFiles)
    {
        qFatal("%s", QString(Q_FUNC_INFO + QStringLiteral(" Невозможно понять примонтированы ли репозитории так как файлы не найдены")).toUtf8().constData());
    }
    else
    {
        if(MountFilesName::Mounted==m_bootFileName)
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" boot примонтирован"));
            m_isBootMounted = true;
        }
        else
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" boot надо проверить"));
            const int firstFileNameIndex = static_cast<int>(m_bootFileName);
            Q_ASSERT(firstFileNameIndex<m_bootIsoPaths.count());
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
        if(MountFilesName::Mounted==m_devFileName)
        {
            Log4QtInfo(Q_FUNC_INFO + QStringLiteral(" dev примонтирован"));
            m_isDevMounted = true;
        }
        else
        {
            const int secondFileNameIndex = static_cast<int>(m_devFileName);
            Q_ASSERT(secondFileNameIndex<m_devIsoPaths.count());
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
    const int firstFileNameIndex = static_cast<int>(m_bootFileName);
    const int secondFileNameIndex = static_cast<int>(m_devFileName);
    if (!m_isBootMounted)
    {
        MountRepository(m_bootIsoPaths.at(firstFileNameIndex), QLatin1Literal("/opt/boot"));
    }
    if (!m_isDevMounted)
    {
        MountRepository(m_devIsoPaths.at(secondFileNameIndex), QLatin1Literal("/opt/dev"));
    }
    RegisterMounting();
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
    QStringList neededLinesRegister = {"deb file:///opt/boot smolensk contrib main non-free", "deb file:///opt/dev smolensk contrib main non-free"};
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
