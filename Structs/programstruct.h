#ifndef STRUCTS_DESKTOPENTITY_H
#define STRUCTS_DESKTOPENTITY_H

#include <QMetaType>

struct DesktopEntity{
    DesktopEntity(){}
    QString name;
    QString type;
    QString exec;
    QString icon;
};

Q_DECLARE_METATYPE(DesktopEntity)


#endif // STRUCTS_DESKTOPENTITY_H
