#ifndef KGROUP_H
#define KGROUP_H

#include <QList>
#include <QVector>

template <class T>
struct KGroup
{
    typedef T value_type;
    typedef T * pointer_type;
    static const int DefaultId = 0;

    KGroup() : groupId(DefaultId), name(QString()), items(QList<T>()) {}

    KGroup(const QString& nm) : groupId(DefaultId), name(nm), items(QList<T>()) {}

    KGroup(int id, const QString& nm) : groupId(id), name(nm), items(QList<T>()) {}

    KGroup(const QString& nm, const QList<T>& it)
        : groupId(DefaultId), name(nm), items(it){}

    KGroup(int id, const QString& nm, const QList<T>& it)
        : groupId(id), name(nm), items(it){}

    KGroup &operator=(const KGroup& other) {
        this->groupId = other.groupId;
        this->name = other.name;
        this->items = other.items;
        return *this;
    }

    inline void add(const T& item)
    { items.append(item); }

    inline void remove(const T& item)
    { items.removeOne(item); }

    inline void removeAt(int idx)
    { items.removeAt(idx); }

    inline void clear()
    { items.clear(); }

    inline bool isEmpty() const
    { return items.isEmpty(); }

    inline int count() const
    { return items.count(); }

    inline const T&  itemAt(int idx) const
    { return items.at(idx); }

    inline const T * pointerAt(int idx) const
    { return &(items[idx]); }

    inline T * pointerAt(int idx)
    { return &(items[idx]); }

    inline void replaceAt(int idx, const T& v)
    { items[idx] = v; }

    bool operator==(const KGroup<T> & o) const
    {
        return (o.groupId == this->groupId && o.name == this->name && o.items == this->items);
    }

    bool operator!=(const KGroup<T> & o) const
    {
        return (o.groupId != this->groupId || o.name != this->name || o.items != this->items);
    }

    KGroup & operator<<(const T& item)
    { items.append(item); return *this; }

    static QList<T> flatten(const QVector<KGroup<T> > &groups)
    {
        QList<T> result;
        for(int k = 0; k < groups.size(); k++)
            result.append(groups.at(k).items);

        return result;
    }
    static QList<T> flatten(const QList<KGroup<T> > &groups)
    {
        QList<T> result;
        for(int k = 0; k < groups.size(); k++)
            result.append(groups.at(k).items);

        return result;
    }

    int groupId;
    QString name;
    QList<T> items;
};

template<class T>
KGroup<T> xMakeGroup(const QString& name, const QList<T> &items)
{
    return KGroup<T>(name, items);
}
template<class T>
KGroup<T> xMakeGroup(const QString &name, const QVector<T> &items)
{
    return KGroup<T>(name, QList<T>::fromVector(items));
}

template<class T>
inline QDataStream &operator<<(QDataStream & s, const KGroup<T> & g)
{
    s << g.groupId << g.name << g.items;
    return s;
}

template<class T>
inline QDataStream &operator>>(QDataStream &s, KGroup<T> &g)
{
    s >> g.groupId >> g.name >> g.items;
    return s;
}


#endif // KGROUP_H
