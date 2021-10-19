#include "stringvalidator.h"

StringValidator::StringValidator(QObject *parent)
    :QValidator(parent)
{

}

StringValidator::~StringValidator()
{

}

QValidator::State StringValidator::validate(QString &input, int &pos) const
{
    if (GlobalFunctions::StringContainsBadCharecters(input))
    {
        return QValidator::Invalid;
    }
    else
    {
        return QValidator::Acceptable;
    }
}

void StringValidator::fixup(QString &input ) const
{
    input=input.trimmed();
}
