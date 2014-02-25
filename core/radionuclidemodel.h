#ifndef RADIONUCLIDEMODEL_H
#define RADIONUCLIDEMODEL_H

#include <QAbstractTableModel>
#include "kradionuclide.h"

class RadionuclideModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RadionuclideModel(KStorage * stg, QObject *parent = 0);
    virtual ~RadionuclideModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool contains(const KRadionuclide& nuc) const;
    KRadionuclide radionuclide(int idx) const;
    void save(const KRadionuclide& nuc);
    void remove(const KRadionuclide& nuc);
    void setCurrent(const KRadionuclide& nuc);
    KRadionuclide current() const;

private:
    KStorage * _storage;
    KRadionuclide _curNuclide;
    
signals:
    
public slots:
    
};

#endif // RADIONUCLIDEMODEL_H
