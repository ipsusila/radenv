#include<QtDebug>
#include <QPointF>
#include <QRectF>
#include "koutput.h"
#include "imodel.h"
#include "kport.h"

// default handler
KOutput KOutput::_defaultHandler;
static KOutput * __handlerPtr = 0;

////////////////////////////////////////////////////////////////////////////////////

KOutputProxy::KOutputProxy()
{
}

void KOutputProxy::errorNotSpecified(IModel * model, const Quantity& qty)
{
    xError() << *model << ": " << qty << QObject::tr(" not specified.");
}
void KOutputProxy::errorMessage(IModel * model, const QString& msg)
{
    xError() << *model << ": " << msg;
}
void KOutputProxy::errorMessage(IModel * model, const Quantity& qty, const QString& msg)
{
    xError() << *model << ": " << qty << ", " << msg;
}
void KOutputProxy::errorPortNotConnected(IModel *model, KPort * port)
{
    if (port)
        xError() << *model << QObject::tr(": Port ") << port->quantity()->symbol << QObject::tr(" not connected.");
    else
        xError() << *model << QObject::tr(": Port not connected.");
}
void KOutputProxy::errorReceptorNotSpecified(IModel * model)
{
    xError() << *model << QObject::tr(": Receptor location is not specified. Please double-click the port to specify location.");
}
void KOutputProxy::errorLoadFailed(IModel * model, const QString & val)
{
    xError() << *model << QString(QObject::tr("Can not load default %1 values from database.")).arg(val);
}
void KOutputProxy::errorLoadFailed(IModel * model, const Quantity & qty)
{
    xError() << *model << QObject::tr("Can not load default ") << qty
             << QObject::tr(" values from database.");
}

void KOutputProxy::warningLocationNotSpecified(IModel * model)
{
    xWarning() << *model << QObject::tr(": Location is not specified. Please double-click the port to specify location.");
}

void KOutputProxy::warningMessage(IModel * model, const QString& msg)
{
    xWarning() << *model << ": " << msg;
}

void KOutputProxy::warningNotProperlyDefined(IModel * model, const Quantity& qty)
{
    xWarning() << *model << ": " << qty << QObject::tr(" value not defined.");
}

void KOutputProxy::infoUseDefaultValue(IModel * model, const Quantity& qty)
{
    xInfo() << *model << QObject::tr("Site specific value for ") << qty
            << " not provided. Default/estimated value will be used.";
}

void KOutputProxy::infoVerificationResult(IModel * model, int err, int warn)
{
    xInfo() << *model << QString(QObject::tr(" -> %1 error(s), %2 warning(s)"))
               .arg(err).arg(warn);
}

////////////////////////////////////////////////////////////////////////////////////

KOutput::KOutput() : _outType(Trace), _level(KOutput::All)
{
}

KOutput::~KOutput()
{
}

void KOutput::clearContents()
{
}

void KOutput::trace(const QString& text)
{
    if ((_level & KOutput::Trace) == KOutput::Trace)
        traceOut(text);
}

void KOutput::info(const QString& text)
{
    if ((_level & KOutput::Info) == KOutput::Info)
        infoOut(text);
}

void KOutput::warning(const QString& text)
{
    if ((_level & KOutput::Warning) == KOutput::Warning)
        warningOut(text);
}

void KOutput::error(const QString& text)
{
    if ((_level & KOutput::Error) == KOutput::Error)
        errorOut(text);
}

void KOutput::traceOut(const QString& text)
{
    qDebug() << "Trace: " << text;
}

void KOutput::infoOut(const QString& text)
{
    qDebug() << "Info: " << text;
}

void KOutput::warningOut(const QString& text)
{
    qWarning() << "Warning: " << text;
}

void KOutput::errorOut(const QString& text)
{
    qCritical() << "Error:" << text;
}

void KOutput::newLine()
{
}
bool KOutput::isHtml() const
{
    return false;
}
void KOutput::setLevel(OutputTypes types)
{
    _level = types;
}

KOutput::OutputTypes KOutput::level() const
{
    return _level;
}
void KOutput::messageOutput(const QString& txt)
{
    switch(_outType)
    {
    case Trace:
        trace(txt);
        break;
    case Info:
        info(txt);
        break;
    case Warning:
        warning(txt);
        break;
    case Error:
        error(txt);
        break;
    default:
        break;
    }
}

KOutput & KOutput::operator<<(const QString& txt)
{
    messageOutput(txt);
    return *this;
}
KOutput & KOutput::operator<<(qreal v)
{
    messageOutput(QString::number(v));
    return *this;
}

KOutput & KOutput::operator<<(int v)
{
    messageOutput(QString::number(v));
    return *this;
}
KOutput & KOutput::operator<<(const QPoint& pt)
{
    messageOutput(QString("Point(%1, %2) ").arg(pt.x()).arg(pt.y()));
    return *this;
}

KOutput & KOutput::operator<<(const QPointF& pt)
{
    messageOutput(QString("PointF(%1, %2) ").arg(pt.x()).arg(pt.y()));
    return *this;
}

KOutput & KOutput::operator<<(const QRect& r)
{
    messageOutput(QString("Rect(%1, %2, %3, %4) ")
                  .arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height()));
    return *this;
}

KOutput & KOutput::operator<<(const QRectF& r)
{
    messageOutput(QString("RectF(%1, %2, %3, %4) ")
                  .arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height()));
    return *this;
}

KOutput & KOutput::operator<<(const IModel & model)
{
    messageOutput(QString(" Model[%1] ").arg(model.tagName()));
    return *this;
}

KOutput & KOutput::operator<<(const KPort & port)
{
    messageOutput(QString(" Port[%1] ").arg(port.quantity()->symbol));
    return *this;
}
KOutput & KOutput::operator<<(const Quantity & qty)
{
    messageOutput(qty.displayText());
    return *this;
}

KOutput & KOutput::operator<<(OutputType t)
{
    if (t == EndLine) {
        newLine();
    }
    else {
        _outType = t;
    }
    return *this;
}
KOutput::OutputType KOutput::currentOutputType() const
{
    return _outType;
}

KOutput & KOutput::handler()
{
    if (__handlerPtr)
        return *__handlerPtr;

    return _defaultHandler;
}

void KOutput::setupHandler(KOutput *h)
{
    __handlerPtr = h;
}
