#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include "radglobal.h"

class ISerializable
{
public:
    virtual ~ISerializable() {}
    virtual QDataStream & serialize(QDataStream & stream) const = 0;
    virtual QDataStream & deserialize(QDataStream & stream) = 0;
};

#define K_DECLARE_SERIALIZABLE(Clz) \
    inline QDataStream & operator<<(QDataStream &s, const Clz & obj) \
    { \
        return obj.serialize(s); \
    } \
    inline QDataStream & operator>>(QDataStream &s, Clz & obj)\
    {\
        return obj.deserialize(s);\
    }

#endif // ISERIALIZABLE_H
