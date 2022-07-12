#ifndef SERVICES_ENVIROMENTALVARIABLESSERVICE_H
#define SERVICES_ENVIROMENTALVARIABLESSERVICE_H

#include <QDomElement>
#include <QList>
#include <QPair>

#include "Logging/logger.h"
#include "Services/Terminals/terminal.h"

class ProfileVariablesService
{
public:
    explicit ProfileVariablesService();
    ~ProfileVariablesService();
public:
    void GetProfileDataFromBackUp(const QDomElement &profileElem);
    void ClearProFiles() noexcept;
    void AppendProfileDataInfo();
    QList <QPair<QString, QString>> GetAllKeys() const noexcept;
    bool SetNewItem(const QString &itemValue, int itemPos, int column) noexcept;
private:
    QPair<QString, QString> GetKeyAndVal(const QString &line) const noexcept;
    void RemoveDublicates(const QString &oldFileText);
    void CreateProFile();
    void WriteItemsToProFile();
    int HasItem(const QString &key) const noexcept;
    void ApplyEtc();
private:
    const QString m_proFilePath;
    QList <QPair<QString, QString>> m_profileKeyWithData;
    Terminal *const m_terminal;

};

#endif // SERVICES_ENVIROMENTALVARIABLESSERVICE_H
