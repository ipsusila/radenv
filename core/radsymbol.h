#ifndef RADSYMBOL_H
#define RADSYMBOL_H

#include <QDataStream>

/**
 * Default namespace
 */
namespace Rad {
    enum ValueType {
        Real = 0,
        NumText = 1,
        Integer = 2,
        Boolean = 3,
        Text = 4,
        Comment = 5
    };
}

/*!
 * Represent symbol to a variable or a numeric quantity
 */
typedef struct _tagQuantity
{
    const unsigned int id;      ///< Identifier given to this symbol
    const int type;             ///< Type of the symbol
    int decimal;                ///< Number of decimal point
    qreal minValue;             ///< Minimum value
    qreal maxValue;             ///< Maximum value
    qreal defaultValue;         ///< Default value
    QString symbol;             ///< Represented symbol
    QString rtSymbol;           ///< Symbol in rich-text format, for display purpose
    QString text;               ///< Short text description
    QString unit;               ///< Unit of the quantity
    QString rtUnit;             ///< Unit in rich-text format
    QString description;        ///< Description of the symbol

    /*!
     * \brief Compared two symbol.
     * \param o - other symbol to be compared
     * \return true if \a id and \a symbol is equal.
     */
    inline bool operator==(const struct _tagQuantity & o) const
    {
        return id == o.id && symbol == o.symbol;
    }
    inline bool operator!=(const struct _tagQuantity & o) const
    {
        return id != o.id || symbol != o.symbol;
    }
    inline bool isUnlimit() const
    {
        return maxValue < minValue;
    }
    inline bool isNumeric() const
    {
        return type == Rad::Integer || type == Rad::Real || type == Rad::NumText;
    }
    inline QString defaultValueString(char fmt = 'g') const
    {
        return QString::number(defaultValue, fmt, decimal);
    }
    inline QString displayText() const
    {
        if (symbol.isEmpty())
            return text;
        return QString("%1 (%2)").arg(text).arg(symbol);
    }
    inline QString displaySymbolWithUnit() const
    {
        if (unit.isEmpty())
            return symbol;
        return QString("%1 (%2)").arg(symbol).arg(unit);
    }
    inline QString displayTextWithUnit() const
    {
        if (unit.isEmpty())
            return text;
        return QString("%1 (%2)").arg(text).arg(unit);
    }
} Quantity;

inline QDataStream & operator<<(QDataStream &s, const Quantity & item)
{
    s << item.id << item.type << item.decimal << item.minValue
      << item.maxValue << item.defaultValue << item.symbol << item.rtSymbol
      << item.text << item.unit << item.rtUnit << item.description;
    return s;
}

inline QDataStream & operator>>(QDataStream &s, Quantity & item)
{
    unsigned int id;
    int type;

    s >> id >> type;
    if (id == item.id && type == item.type) {
        s >> item.decimal >> item.minValue >> item.maxValue
          >> item.defaultValue >> item.symbol >> item.rtSymbol >> item.text
          >> item.unit >> item.rtUnit >> item.description;
    }
    return s;
}

#endif // RADSYMBOL_H
