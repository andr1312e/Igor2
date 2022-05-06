#ifndef LOG4QT_XMLLAYOUT_H
#define LOG4QT_XMLLAYOUT_H

#include "layout.h"

namespace Log4Qt
{

class  LOG4QT_EXPORT XMLLayout : public Layout
{
    Q_OBJECT
public:
    explicit XMLLayout(QObject *parent = Q_NULLPTR);

    QString format(const LoggingEvent &event) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(XMLLayout)
};

}

#endif // LOG4QT_XMLLAYOUT_H

