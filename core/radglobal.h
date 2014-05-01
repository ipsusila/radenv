#ifndef RADGLOBAL_H
#define RADGLOBAL_H

#include <QList>
#include <QSharedPointer>
#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <QtCore/qglobal.h>
#include <QPair>
#include <QDateTime>
#include <limits>
#include "kgroup.h"
#include "radquantity.h"

#if defined(KCORE_LIBRARY)
#  define K_CORE_EXPORT Q_DECL_EXPORT
#else
#  define K_CORE_EXPORT Q_DECL_IMPORT
#endif

class KPort;
class KData;
class KDataItem;
class KDataArray;
class KDataTable;
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
class KSettingManager;
class KQuantityControl;
class KPluginManager;

struct _tagPM {
    int left;
    int top;
    int right;
    int bottom;
};

typedef QList<KConnector *>             ConnectorList;
typedef QList<KData>                    DataList;
typedef QVector<KDataItem>              DataItemArray;
typedef QList<IModel *>                 ModelList;
typedef QList<IModelFactory *>          FactoryList;
typedef QList<KModelInfo>               ModelInfoList;
typedef QList<KGroupInfo>               GroupInfoList;
typedef QList<KRadionuclide>            RadionuclideList;
typedef QList<KLocation>                LocationList;
typedef QList<KCase>                    AssessmentList;
typedef KGroup<KData>                   DataGroup;
typedef QList<KCalculationInfo>         CalculationList;
typedef QList<const Quantity *>         ConstQuantityList;
typedef QList<Quantity *>               QuantityList;
typedef QList<KDataArray>               DataArrayList;
typedef QList<KQuantityControl>           QuantityControlList;

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
    extern K_CORE_EXPORT const char LatinEndLine;
    extern K_CORE_EXPORT const Quantity EmptyQuantity;
    extern K_CORE_EXPORT const Quantity ScalarInput;
    extern K_CORE_EXPORT const Quantity ScalarInput1;
    extern K_CORE_EXPORT const Quantity ScalarInput2;
    extern K_CORE_EXPORT const Quantity ScalarInput3;
    extern K_CORE_EXPORT const Quantity ScalarInput4;
    extern K_CORE_EXPORT const Quantity ScalarInput5;
    extern K_CORE_EXPORT const Quantity ScalarInput6;
    extern K_CORE_EXPORT const Quantity ScalarInput7;
    extern K_CORE_EXPORT const Quantity ScalarInput8;
    extern K_CORE_EXPORT const Quantity ScalarInput9;

    extern K_CORE_EXPORT const Quantity ScalarOutput;
    extern K_CORE_EXPORT const Quantity ScalarOutput1;
    extern K_CORE_EXPORT const Quantity ScalarOutput2;
    extern K_CORE_EXPORT const Quantity ScalarOutput3;
    extern K_CORE_EXPORT const Quantity ScalarOutput4;
    extern K_CORE_EXPORT const Quantity ScalarOutput5;

    extern K_CORE_EXPORT const Quantity CommentQuantity;
    extern K_CORE_EXPORT const Quantity NameQuantity;
    extern K_CORE_EXPORT const Quantity UseDefaultValue;
    extern K_CORE_EXPORT const Quantity LongCommentQuantity;
    extern K_CORE_EXPORT const Quantity UseDefaultValue2;
}

#endif // RADGLOBAL_H
