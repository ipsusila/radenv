#include <QtDebug>
#include "constantvalue.h"
#include "kport.h"
#include "koutput.h"
#include "widgetconstantvalue.h"
#include "quantity.h"
#include "imodelfactory.h"
#include <QThread>

ConstantValue::ConstantValue(IModelFactory * fact, const KModelInfo& inf)
    : IModel(fact, inf)
{
}

KData ConstantValue::modelData(const Quantity &qty) const
{
    return _dataList.find(qty);
}
void ConstantValue::copyDataTo(IModel * model) const
{
    if (model->info().serialId() == info().serialId()) {
        ConstantValue * dest = reinterpret_cast<ConstantValue *>(model);
        dest->_dataList = this->_dataList;
    }
}

KDataArray ConstantValue::result() const
{
    return _dataList;
}
bool ConstantValue::calculate(const KCalculationInfo& ci)
{
    qDebug() << Q_FUNC_INFO << ", thread id: " << (quint32)QThread::currentThreadId();
    Q_UNUSED(ci);
    return !_dataList.isEmpty();
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
    qDebug() << Q_FUNC_INFO << ", thread id: " << (quint32)QThread::currentThreadId();
    _outPorts << new KPort(this, &Srs19::ScalarOutput, KPort::Output);
    return true;
}
IUserInput * ConstantValue::createUserInputWidget(QWidget *parent)
{
     WidgetConstantValue * w =
             new WidgetConstantValue(factory()->storage(), &_dataList, parent);
    return w;
}

bool ConstantValue::verify(int * oerr, int * owarn)
{
    qDebug() << Q_FUNC_INFO << ", thread id: " << (quint32)QThread::currentThreadId();
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

QDataStream & ConstantValue::serialize(QDataStream &stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    IModel::serialize(stream);
    _dataList.serialize(stream);
    return stream;
}
QDataStream & ConstantValue::deserialize(QDataStream &stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    IModel::deserialize(stream);
    _dataList.deserialize(stream);
    return stream;
}

