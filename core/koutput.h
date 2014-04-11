#ifndef KOUTPUT_H
#define KOUTPUT_H

#include <QString>
#include "radglobal.h"

class QPoint;
class QPointF;
class QRect;
class QRectF;

class K_CORE_EXPORT KOutputProxy
{
public:
    KOutputProxy();

    static void errorNotSpecified(IModel * model, const Quantity& qty);
    static void errorMessage(IModel * model, const Quantity& qty, const QString& msg);
    static void errorMessage(IModel * model, const QString& msg);
    static void errorPortNotConnected(IModel * model, KPort * port = 0);
    static void errorReceptorNotSpecified(IModel * model);
    static void errorLoadFailed(IModel * model, const QString & val);
    static void errorLoadFailed(IModel * model, const Quantity & qty);
    static void warningLocationNotSpecified(IModel * model);
    static void warningNotProperlyDefined(IModel * model, const Quantity& qty);
    static void warningMessage(IModel * model, const QString& msg);
    static void infoVerificationResult(IModel * model, int err, int warn);
    static void infoUseDefaultValue(IModel * model, const Quantity& qty);
};

class K_CORE_EXPORT KOutput
{
public:
    enum OutputType {
        Trace = 0x01,
        Info = 0x02,
        Warning = 0x04,
        Error = 0x08,
        EndLine = 0x10,
        All = 0xFF
    };
    Q_DECLARE_FLAGS(OutputTypes, OutputType)

    virtual ~KOutput();

    void trace(const QString& text);
    void info(const QString& text);
    void warning(const QString& text);
    void error(const QString& text);

    virtual bool isHtml() const;
    virtual void clearContents();
    virtual void newLine();
    OutputType currentOutputType() const;
    void setLevel(OutputTypes types);
    OutputTypes level() const;

    KOutput & operator<<(const QString& txt);
    KOutput & operator<<(qreal v);
    KOutput & operator<<(int v);
    KOutput & operator<<(OutputType t);
    KOutput & operator<<(const QPoint& pt);
    KOutput & operator<<(const QPointF& pt);
    KOutput & operator<<(const QRect& r);
    KOutput & operator<<(const QRectF& r);
    KOutput & operator<<(const IModel & model);
    KOutput & operator<<(const KPort & port);
    KOutput & operator<<(const Quantity & qty);

    static KOutput & handler();
    static void setupHandler(KOutput * h);
protected:
    KOutput();

    void messageOutput(const QString& text);
    virtual void traceOut(const QString& text);
    virtual void infoOut(const QString& text);
    virtual void warningOut(const QString& text);
    virtual void errorOut(const QString& text);

    OutputType _outType;
    OutputTypes _level;

private:
    static KOutput _defaultHandler;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KOutput::OutputTypes)

#endif // KOUTPUT_H
