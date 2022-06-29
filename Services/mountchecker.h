#ifndef SERVICES_MOUNTCHECKER_H
#define SERVICES_MOUNTCHECKER_H

#include "Terminals/terminal.h"
#include "Logging/logger.h"

enum class MountFilesState
{
    OriginalNames = 0,
    ShortInSubFolder = 1,
    ShortInOpt = 2,
    NoFiles = 3,
    Mounted = 4,
};

class MountChecker
{
public:
    MountChecker();
    ~MountChecker();
    bool HasMountFiles();
    bool IsReposiotoryMounted(const char *calledFunc);
    void MountRepository();
    void UnMountRepository();
    void RegisterMounting();
private:
    void CheckAndCreateMountFolder();
    void UpdateAptGet();
    void MountRepository(const QString &isoPath, const QString &path);
    void UnMountRepository(const QString &path);

private:
    Terminal *m_terminal;
    bool m_isBootMounted = false;
    bool m_isDevMounted = false;
    const QString m_bootFolder;
    const QString m_devFolder;

    const QStringList m_bootIsoPaths;
    const QStringList m_devIsoPaths;

    MountFilesState m_bootFileState, m_devFileState;

    const QString m_registerFilePath;

};

#endif // SERVICES_MOUNTCHECKER_H
