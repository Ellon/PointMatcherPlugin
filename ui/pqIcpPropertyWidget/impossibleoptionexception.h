#ifndef IMPOSSIBLEOPTIONEXCEPTION_H
#define IMPOSSIBLEOPTIONEXCEPTION_H

#include <QException>

class ImpossibleOptionException : public QException
{
public:
    ImpossibleOptionException();

public:
    void raise() const { throw *this; }
    ImpossibleOptionException *clone() const { return new ImpossibleOptionException(*this); }
};

#endif // IMPOSSIBLEOPTIONEXCEPTION_H
