#ifndef FOODCHAIN_H
#define FOODCHAIN_H

#include "transport.h"

class FoodChain : public Transport
{
protected:
    bool needLocation() const;
    virtual QString displayText() const;
    FoodChain(IModelFactory * fact, const KModelInfo& inf);
};

#endif // FOODCHAIN_H
