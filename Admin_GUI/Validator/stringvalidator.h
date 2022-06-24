#ifndef STRINGVALIDATOR_H
#define STRINGVALIDATOR_H
#include <QValidator>

#include "functionswithstrings.h"

class StringValidator: public QValidator
{
public:
    explicit StringValidator(QObject *parent);
    ~StringValidator();
public:
    virtual State validate(QString &input, int &pos) const Q_DECL_OVERRIDE;
    virtual void fixup(QString &input) const Q_DECL_OVERRIDE;
};

#endif // STRINGVALIDATOR_H
