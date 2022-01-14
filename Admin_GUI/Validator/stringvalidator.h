#ifndef STRINGVALIDATOR_H
#define STRINGVALIDATOR_H
#include <QValidator>

#include "functionswithstrings.h"

class StringValidator: public QValidator
{
public:
    StringValidator(QObject * parent);
    ~StringValidator();
    virtual State validate(QString &input, int &pos) const Q_DECL_OVERRIDE;
    virtual void fixup(QString &input) const Q_DECL_OVERRIDE;
};

#endif // STRINGVALIDATOR_H
