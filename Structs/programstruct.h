#ifndef STRUCTS_DESKTOPENTITY_H
#define STRUCTS_DESKTOPENTITY_H

#include <QMetaType>
#include <QString>

class DesktopEntity
{
public:
    explicit DesktopEntity();
    explicit DesktopEntity(const QString &name, const QString &exec, const QString &icon);
    DesktopEntity(const DesktopEntity &entity);
    ~DesktopEntity();
public:
    bool operator ==(const DesktopEntity &entity) const noexcept;
    DesktopEntity &operator =(const DesktopEntity &entity) noexcept;
public://Сеттеры
    void SetParams(const QString &name, const QString &exec, const QString &icon) noexcept;
    void SetParams(const QString &name, const QString &exec) noexcept;
    void SetName(const QString &name) noexcept;
    void SetExec(const QString &exec) noexcept;
    void SetIcon(const QString &icon) noexcept;
public://Геттеры
    const QString &GetName() const noexcept;
    const QString &GetExec() const noexcept;
    const QString &GetIcon() const noexcept;
private:
    QString m_name;
    QString m_exec;
    QString m_icon;

};

Q_DECLARE_METATYPE(DesktopEntity)


#endif // STRUCTS_DESKTOPENTITY_H
