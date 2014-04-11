#include "constantvalue.h"
#include "kport.h"
#include "koutput.h"
#include "widgetconstantvalue.h"

ConstantValue::ConstantValue(IModelFactory * fact, const KModelInfo& inf)
    : IModel(fact, inf)
{
}

KData ConstantValue::modelData(const Quantity &qty) const
{
    return _dataList.find(qty);
}

KDataArray ConstantValue::result() const
{
    return _dataList;
}
bool ConstantValue::calculate(const KCalculationInfo& ci)
{
    Q_UNUSED(ci);
    return !_dataList.isEmpty();
}

void ConstantValue::refresh()
{
    //todo
}

bool ConstantValue::isSource() const
{
    return true;
}

bool ConstantValue::needLocation() const
{
    return false;
}

QRectF ConstantValue::modelRect() const
{
    return QRectF(-50, -30, 100, 60);
}

const KPortList & ConstantValue::outputs() const
{
    return _outPorts;
}
QString ConstantValue::displayText() const
{
    return "";
}

bool ConstantValue::allocateIoPorts()
{
    _outPorts << new KPort(this, &Rad::ScalarOutput, KPort::Output);
    return true;
}
IUserInput * ConstantValue::createUserInputWidget(QWidget *parent)
{
     WidgetConstantValue * w =
             new WidgetConstantValue(&_dataList, parent);
    return w;
}

bool ConstantValue::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;

    if (_dataList.isEmpty()) {
        err++;
        KOutputProxy::errorMessage(this, QObject::tr("Value not defined"));
    }

    KOutputProxy::infoVerificationResult(this, err, warn);
    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool ConstantValue::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool ConstantValue::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
