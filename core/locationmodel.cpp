#include "locationmodel.h"
#include "kstorage.h"

LocationModel::LocationModel(KStorage * stg, QObject *parent) :
    QAbstractTableModel(parent), _storage(stg)
{
}
LocationModel::~LocationModel()
{
}

int LocationModel::rowCount(const QModelIndex & /*parent = QModelIndex()*/) const
{
   return _storage->locations()->size();
}
int LocationModel::columnCount(const QModelIndex & /*parent = QModelIndex()*/) const
{
   return 5;
}
void LocationModel::setCurrent(const KLocation& loc)
{
    _curLocation = loc;
}

KLocation LocationModel::current() const
{
    return _curLocation;
}

bool LocationModel::contains(const KLocation &loc) const
{
    return _storage->locations()->contains(loc);
}

KLocation LocationModel::location(int idx) const
{
    int sz = _storage->locations()->size();
    if (idx >= 0 && idx < sz)
        return _storage->locations()->at(idx);
    return KLocation();
}

void LocationModel::save(const KLocation& loc)
{
    int pos = _storage->locations()->indexOf(loc);
    int sz = _storage->locations()->size();
    if (_storage->save(loc)) {
        //if newly added
        if (pos < 0) {
            beginInsertRows(QModelIndex(), sz, sz);
            endInsertRows();
        }
    }
}
void LocationModel::remove(const KLocation &loc)
{
    int pos = _storage->locations()->indexOf(loc);
    if (pos >= 0) {
        beginRemoveRows(QModelIndex(), pos, pos);
        _storage->remove(loc);
        endRemoveRows();
    }
}

QVariant LocationModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
       return QVariant();

    int r = index.row();
    int c = index.column();
    KLocation loc = _storage->locations()->at(r);
    switch(c) {
    case 0:
        return QVariant(loc.code());
    case 1:
        return QVariant(loc.name());
    case 2:
        return QVariant(loc.description());
    case 3:
        return QVariant(loc.distance());
    case 4:
        return QVariant(loc.angle());
    }

    return QVariant();
}
QVariant LocationModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        switch(section) {
        case 0:
            return tr("Code");
        case 1:
            return tr("Name");
        case 2:
            return tr("Description");
        case 3:
            return tr("Distance (m)");
        case 4:
            return tr("Angle (deg)");
        }
    }
    else if (orientation == Qt::Vertical) {
        return QVariant(section+1);
    }

    return QVariant();
}
