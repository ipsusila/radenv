#ifndef DEFAULTVALUETABLE_H
#define DEFAULTVALUETABLE_H

#include <QMap>
#include "kstorage.h"
#include "kstoragecontent.h"
#include "kradionuclide.h"
#include "imodelfactory.h"

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
        const T * defValues;
        int defValuesCnt;
        QString name;
        QString description;
        bool isElement;

        QMap<QString, T> valTable;
        QDateTime created;


        DefaultValuePrivate(IModelFactory *f, const T * defVals, int defValsCnt,
                            const QString &nm, const QString & desc, bool isE)
            : factory(f), defValues(defVals),
              defValuesCnt(defValsCnt), name(nm),
              description(desc), isElement(isE)
        {

            if (!load()) {
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

        bool load()
        {
            Q_ASSERT(factory != 0);
            KStorage * storage = factory->storage();

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
        bool save() {
            Q_ASSERT(factory != 0);
            KStorage * storage = factory->storage();

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

            struct _m { QString key; qreal f0; };
            Q_ASSERT(sizeof(struct _m) <= sizeof(T));
            if (sizeof(struct _m) > sizeof(T))
                return 0;

            T val = valTable[mapKey];
            struct _m * vt = (struct _m *)(&val);
            qreal * fields = &(vt->f0);
            if (vt->key == mapKey)
                return fields[idx];
            else
                return 0;

            /*
            //use pointer aritmetic to calculate item field data
            T val = valTable[mapKey];
            int sz = sizeof(QString);
            quint8 * valPtr = (quint8 *)&val;
            qreal * fields = (qreal *)(valPtr+sz);

            //cast to real
            return fields[idx];
            */
        }
    };

public:
    explicit DefaultValueTable(IModelFactory * factory, const T * defValues, int defValuesCnt,
                          const QString &name, const QString &desc, bool isElement)
        : data(new DefaultValuePrivate(factory, defValues, defValuesCnt, name, desc, isElement))
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
        return data->save();
    }

    inline bool saveDefault() {
        data->createDefault();
        return data->save();
    }

    inline bool load(bool createOnEmpty = false) {
        bool res = data->load();
        if (!res && createOnEmpty) {
            data->createDefault();
            res = data->save();
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
