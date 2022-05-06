#ifndef STRUCTS_DESKTOPENTITY_H
#define STRUCTS_DESKTOPENTITY_H

#include <QMetaType>
#include <QString>

struct DesktopEntity{
    QString name;
    QString type;
    QString exec;
    QString icon;
    bool operator ==(const DesktopEntity &entity) const{
        return (name==entity.name && exec==entity.exec);
    }
};

Q_DECLARE_TYPEINFO(DesktopEntity, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(DesktopEntity)


#endif // STRUCTS_DESKTOPENTITY_H
