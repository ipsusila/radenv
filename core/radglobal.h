#ifndef RADGLOBAL_H
#define RADGLOBAL_H

#include <QList>
#include <QSharedPointer>
#include <QtCore/qglobal.h>
#include <QPair>
#include <QDateTime>
#include "kgroup.h"
#include <limits>

#if defined(KCORE_LIBRARY)
#  define K_CORE_EXPORT Q_DECL_EXPORT
#else
#  define K_CORE_EXPORT Q_DECL_IMPORT
#endif

class KPort;
class KData;
class KDataArray;
class IModel;
class KOutput;
class KReport;
class KLocation;
class KModelInfo;
class KRadionuclide;
class IParamsEditor;
class IModelFactory;
class KGroupInfo;
class KStorage;
class KCase;
class KConnector;
class KCalculationInfo;
class KModelScene;

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

struct _tagPM {
    int left;
    int top;
    int right;
    int bottom;
};

/*!
 * Represent symbol to a variable or a numeric quantity
 */
typedef struct _tagSymbol
{
    const unsigned int id;      ///< Identifier given to this symbol
    const int type;             ///< Type of the symbol
    const int decimal;          ///< Number of decimal point
    const qreal minValue;       ///< Minimum value
    const qreal maxValue;       ///< Maximum value
    const qreal defaultValue;   ///< Default value
    const QString symbol;       ///< Represented symbol
    const QString rtSymbol;     ///< Symbol in rich-text format, for display purpose
    const QString text;         ///< Short text description
    const QString unit;         ///< Unit of the quantity
    const QString rtUnit;       ///< Unit in rich-text format
    const QString description;  ///< Description of the symbol

    /*!
     * \brief Compared two symbol.
     * \param o - other symbol to be compared
     * \return true if \a id and \a symbol is equal.
     */
    inline bool operator==(const struct _tagSymbol & o) const
    {
        return id == o.id && symbol == o.symbol;
    }
    inline bool operator!=(const struct _tagSymbol & o) const
    {
        return id != o.id || symbol != o.symbol;
    }
    inline bool isUnlimit() const
    {
        return maxValue < minValue;
    }
    inline QString defaultValueString(char fmt = 'g') const
    {
        return QString::number(defaultValue, fmt, decimal);
    }
    inline QString displayText() const
    {
        return QString("%1 (%2)").arg(text).arg(symbol);
    }
} Symbol;

typedef QList<KPort *>                  PortList;
typedef QList<KConnector *>             ConnectorList;
typedef QList<KData>                    DataList;
typedef QList<IModel *>                 ModelList;
typedef QList<IModelFactory *>          FactoryList;
typedef QList<KModelInfo>               ModelInfoList;
typedef QList<KGroupInfo>               GroupInfoList;
typedef QList<KRadionuclide>            RadionuclideList;
typedef QList<KLocation>                LocationList;
typedef QList<KCase>                    AssessmentList;
typedef KGroup<KData>                   DataGroup;
typedef QList<KCalculationInfo>         CalculationList;
typedef QList<const Symbol *>           SymbolList;

typedef struct _tagPM                   SMargin;
typedef struct _tagPM                   SPadding;

/**
 * Convert to (t & v) == v
 */
#define xHas(t,v)   (((t) & (v)) == (v))

/*!
 * Macro for accessing output.
 */
#define xTrace()    (KOutput::handler() << KOutput::EndLine << KOutput::Trace)
#define xInfo()     (KOutput::handler() << KOutput::EndLine << KOutput::Info)
#define xWarning()  (KOutput::handler() << KOutput::EndLine << KOutput::Warning)
#define xError()    (KOutput::handler() << KOutput::EndLine << KOutput::Error)

/**
 * Default storage name
 */
#define RAD_STORAGEID                 "_x_smea_storage_"
#define RAD_RADIONUCLIDE              "x_smea_radionuclide"
#define RAD_STORAGE                   "x_smea_storage"
#define RAD_LOCATION                  "x_smea_location"
#define RAD_ASSESSMENT                "x_smea_assessment"
#define RAD_NULL_FACTORY              "__null__"

#define TYPE_MODEL                      1
#define TYPE_PORT                       2
#define TYPE_LOCATION_PORT              3
#define TYPE_REPORT_PORT                4
#define TYPE_CONNECTOR                  5

#define DIGIT_DEF                       14
#define DIGIT_NONE                      0
#define MAX_REAL                        std::numeric_limits<qreal>::max()
#define MIN_REAL                        std::numeric_limits<qreal>::min()
#define MAX_INT                         (qreal)std::numeric_limits<int>::max()
#define MIN_INT                         (qreal)std::numeric_limits<int>::min()


namespace Rad {
extern const char LatinEndLine;
extern const Symbol EmptySymbol;
extern const Symbol ScalarInput;
extern const Symbol ScalarInput1;
extern const Symbol ScalarInput2;
extern const Symbol ScalarInput3;
extern const Symbol ScalarInput4;
extern const Symbol ScalarInput5;
extern const Symbol ScalarInput6;
extern const Symbol ScalarInput7;
extern const Symbol ScalarInput8;
extern const Symbol ScalarInput9;

extern const Symbol ScalarOutput;
extern const Symbol ScalarOutput1;
extern const Symbol ScalarOutput2;
extern const Symbol ScalarOutput3;
extern const Symbol ScalarOutput4;
extern const Symbol ScalarOutput5;

extern const Symbol CommentSymbol;
}

#endif // RADGLOBAL_H
