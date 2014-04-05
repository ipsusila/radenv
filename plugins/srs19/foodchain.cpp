#include "foodchain.h"

FoodChain::FoodChain(IModelFactory * fact, const KModelInfo& inf)
    : Transport(fact, inf)
{
}
bool FoodChain::needLocation() const
{
    return false;
}
QString FoodChain::displayText() const
{
    KData data = this->modelData(Rad::NameSymbol);
    if (data.isValid())
        return data.value().toString();
    return "Food model";
}
