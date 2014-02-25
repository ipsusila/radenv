#ifndef LOCATIONMODEL_H
#define LOCATIONMODEL_H

#include <QAbstractTableModel>
#include "klocation.h"

class LocationModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LocationModel(KStorage * stg, QObject *parent = 0);
    virtual ~LocationModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool contains(const KLocation& loc) const;
    KLocation location(int idx) const;
    void save(const KLocation& loc);
    void remove(const KLocation& loc);
    void setCurrent(const KLocation& loc);
    KLocation current() const;
    
signals:
    
public slots:

private:
    KStorage * _storage;
    KLocation _curLocation;
    
};

#endif // LOCATIONMODEL_H
