#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <QString>

#include "boolmatrix.h"

namespace Serializer
{
    QString serialize(const IBoolMatrix& input);
    bool deserialize(const QString& input, IBoolMatrix& output);
}

#endif // SERIALIZER_H
