#ifndef STRINGVALIDATOR_H
#define STRINGVALIDATOR_H
#include <QValidator>

#include "functionswithstrings.h"

class StringValidator: public QValidator
{
public:
    StringValidator(QObject * parent);
    ~StringValidator();
    State validate(QString &input, int &pos) const;
    void fixup(QString &input) const;
};

#endif // STRINGVALIDATOR_H
