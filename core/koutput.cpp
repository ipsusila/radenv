#include<QtDebug>
#include <QPointF>
#include <QRectF>
#include "koutput.h"
#include "imodel.h"
#include "kport.h"

// default handler
KOutput KOutput::_defaultHandler;
static KOutput * __handlerPtr = 0;

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
    messageOutput(QString(" Port[%1] ").arg(port.symbol()->symbol));
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
