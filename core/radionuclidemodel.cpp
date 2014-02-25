#include <QtDebug>
#include <QSize>
#include "radionuclidemodel.h"
#include "kstorage.h"

RadionuclideModel::RadionuclideModel(KStorage * stg, QObject *parent /*= 0*/)
    : QAbstractTableModel(parent), _storage(stg)
{
}
RadionuclideModel::~RadionuclideModel()
{
    qDebug() << "Radionuclide model removed";
}

int RadionuclideModel::rowCount(const QModelIndex & /*parent = QModelIndex()*/) const
{
   return _storage->radionuclides()->size();
}
int RadionuclideModel::columnCount(const QModelIndex & /*parent = QModelIndex()*/) const
{
   return 5;
}
void RadionuclideModel::setCurrent(const KRadionuclide& nuc)
{
    _curNuclide = nuc;
}

KRadionuclide RadionuclideModel::current() const
{
    return _curNuclide;
}

bool RadionuclideModel::contains(const KRadionuclide& nuc) const
{
    return _storage->radionuclides()->contains(nuc);
}

KRadionuclide RadionuclideModel::radionuclide(int idx) const
{
    int sz = _storage->radionuclides()->size();
    if (idx >= 0 && idx < sz)
        return _storage->radionuclides()->at(idx);
    return KRadionuclide();
}

void RadionuclideModel::save(const KRadionuclide& nuc)
{
    int pos = _storage->radionuclides()->indexOf(nuc);
    int sz = _storage->radionuclides()->size();
    if (_storage->save(nuc)) {
        //if newly added
        if (pos < 0) {
            beginInsertRows(QModelIndex(), sz, sz);
            endInsertRows();
        }
    }
}
void RadionuclideModel::remove(const KRadionuclide &nuc)
{
    int pos = _storage->radionuclides()->indexOf(nuc);
    if (pos >= 0) {
        beginRemoveRows(QModelIndex(), pos, pos);
        _storage->remove(nuc);
        endRemoveRows();
    }
}

QVariant RadionuclideModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
       return QVariant();

    int r = index.row();
    int c = index.column();
    KRadionuclide nuc = _storage->radionuclides()->at(r);
    switch(c) {
    case 0:
        return QVariant(nuc.nuclide());
    case 1:
        return QVariant(nuc.halfLife().decayConstant());
    case 2:
        return QVariant(nuc.halfLife().value());
    case 3:
        return QVariant(nuc.usage());
    case 4:
        return QVariant(nuc.attributes());
    }

    return QVariant();
}
QVariant RadionuclideModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        switch(section) {
        case 0:
            return tr("Radionuclide");
        case 1:
            return tr("Decay \nConstant(sec-1)");
        case 2:
            return tr("Half \nlife (sec)");
        case 3:
            return tr("Usage");
        case 4:
            return tr("Attributes");
        }
    }
    else if (orientation == Qt::Vertical) {
        return QVariant(section+1);
    }

    return QVariant();
}
