#ifndef DEFAULTVALUETABLE_H
#define DEFAULTVALUETABLE_H

#include <QMap>
#include <QDebug>
#include <QSharedDataPointer>
#include "kstorage.h"
#include "kstoragecontent.h"
#include "kradionuclide.h"

/**
 * Template for default value tables.
 * It's assumed that T is a struct/class which has the following
 * fields structure:
 *  - element/radionuclide : QString
 *  - val1
 *  - val2
 *  - val3
 *  - .... (type of real)
 */

template <class T>
class DefaultValueTable
{
private:
    class DefaultValuePrivate : public QSharedData {
    public:
        IModelFactory * factory;
        KStorage * storage;
        const T * defValues;
        int defValuesCnt;
        QString name;
        QString description;
        bool isElement;

        QMap<QString, T> valTable;
        QDateTime created;


        DefaultValuePrivate(IModelFactory *f, const T * defVals, int defValsCnt,
                            const QString &nm, const QString & desc, bool isE, KStorage *s)
            : factory(f), storage(s), defValues(defVals),
              defValuesCnt(defValsCnt), name(nm),
              description(desc), isElement(isE)
        {
            if (storage) {
                loadFrom(storage);
            }
            else {
                createDefault();
            }
        }
        void createDefault()
        {
            for (int k = 0; k < defValuesCnt; k++) {
                const T * val = (const T *)(defValues + k);
                valTable[*((QString *)val)] = *val;
            }
            created = QDateTime::currentDateTime();
        }

        bool loadFrom(KStorage * storage) {
            //save pointer to storage
            this->storage = storage;

            //load from storage
            valTable.clear();
            KStorageContent  content = storage->load(name, factory);
            if (!content.isEmpty()) {
                name = content.name();
                description = content.description();
                created = content.created();

                QDataStream stream(content);
                stream >> valTable;
            }
            return !valTable.isEmpty();
        }
        bool saveTo(KStorage * storage) {
            KStorageContent content(created);
            content.setFactory(factory);
            content.setName(name);
            content.setDescription(description);

            //append content
            QDataStream stream(&content, QIODevice::WriteOnly);
            stream << valTable;
            return storage->save(content);
        }

        T coeff(const QString& key) const
        {
            QString mapKey;
            if (isElement)
                mapKey = KRadionuclide::elementName(key);
            else
                mapKey = key;

            return valTable[mapKey];
        }
        qreal coeff(const QString& key, int idx) const
        {
            QString mapKey;
            if (isElement)
                mapKey = KRadionuclide::elementName(key);
            else
                mapKey = key;

            //use pointer aritmetic to calculate item field data
            T val = valTable[mapKey];
            int sz = sizeof(QString);
            quint8 * valPtr = (quint8 *)&val;
            qreal * fields = (qreal *)(valPtr+sz);

            //cast to real
            return fields[idx];
        }
    };

public:
    explicit DefaultValueTable(IModelFactory * factory, const T * defValues, int defValuesCnt,
                          const QString &name, const QString &desc, bool isElement, KStorage * storage)
        : data(new DefaultValuePrivate(factory, defValues, defValuesCnt, name, desc, isElement, storage))
    {
    }

    DefaultValueTable(const DefaultValueTable &rhs) : data(rhs.data)
    {
    }

    DefaultValueTable &operator=(const DefaultValueTable &rhs)
    {
        if (this != &rhs)
            data.operator=(rhs.data);
        return *this;
    }

    ~DefaultValueTable() {}

    inline bool save() {
        return data->saveTo(KStorage::storage());
    }

    inline bool saveDefault() {
        data->createDefault();
        return data->saveTo(KStorage::storage());
    }

    inline bool load(bool createOnEmpty = false) {
        bool res = data->loadFrom(KStorage::storage());
        if (!res && createOnEmpty) {
            data->createDefault();
            res = data->saveTo(KStorage::storage());
        }

        return res;
    }

    inline qreal value(const QString& key, int idx) const
    {
        return data->coeff(key, idx);
    }
    inline T value(const QString& key) const
    {
        return data->coeff(key);
    }

private:
    QSharedDataPointer<DefaultValuePrivate> data;
};

#endif // DEFAULTVALUETABLE_H
