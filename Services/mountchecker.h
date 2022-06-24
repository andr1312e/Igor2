#ifndef SERVICES_MOUNTCHECKER_H
#define SERVICES_MOUNTCHECKER_H

#include "Terminals/terminal.h"
#include "Logging/logger.h"

enum class MountFilesName
{
    Full = 0,
    Short = 1,
    NoFiles = 2,
    Mounted = 3,
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
    void MountRepository(const QString &isoPath, const QString &path);
    void UnMountRepository(const QString &path);

private:
    Terminal *m_terminal;
    bool m_isBootMounted = false;
    bool m_isDevMounted = false;

    const QStringList m_bootIsoPaths;
    const QStringList m_devIsoPaths;

    MountFilesName m_bootFileName, m_devFileName;

    const QString m_registerFilePath;

};

#endif // SERVICES_MOUNTCHECKER_H
