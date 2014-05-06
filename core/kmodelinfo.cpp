#include "kmodelinfo.h"
#include <QSharedData>

class KModelInfoPrivate : public QSharedData
{
    int _serId;
    QString _name;
    QString _text;
    QString _description;
    QIcon _icon;
public:
    KModelInfoPrivate() : _serId(0)
    {
    }
    KModelInfoPrivate(int serId) : _serId(serId)
    {
    }

    KModelInfoPrivate(int serId, const QString& n, const QString& t, const QString& d, const QIcon& i)
        : _serId(serId), _name(n), _text(t), _description(d), _icon(i)
    {
    }

    inline QString name() const { return _name; }
    inline void setName(const QString& n) { _name = n; }
    inline QString text() const { return _text; }
    inline void setText(const QString& t) { _text = t; }
    inline QString description() const { return _description; }
    inline void setDescription(const QString& d) { _description = d; }
    inline QIcon icon() const { return _icon; }
    inline void setIcon(const QIcon& i) { _icon = i; }
    inline int serialId() const { return _serId; }
};

KModelInfo::KModelInfo() : dptr(new KModelInfoPrivate)
{
}

KModelInfo::KModelInfo(int id) : dptr(new KModelInfoPrivate(id))
{
}
KModelInfo::KModelInfo(int id, const QString& n, const QString& t, const QString& d, const QIcon& i)
    : dptr(new KModelInfoPrivate(id, n,t,d,i))
{
}

KModelInfo::KModelInfo(const KModelInfo &rhs) : dptr(rhs.dptr)
{
}

KModelInfo &KModelInfo::operator=(const KModelInfo &rhs)
{
    if (this != &rhs)
        dptr.operator=(rhs.dptr);
    return *this;
}

KModelInfo::~KModelInfo()
{
}

QString KModelInfo::name() const
{
    return dptr->name();
}
void KModelInfo::setName(const QString &n)
{
    dptr->setName(n);
}

QString KModelInfo::text() const
{
    return dptr->text();
}
void KModelInfo::setText(const QString &t)
{
    dptr->setText(t);
}

QString KModelInfo::description() const
{
    return dptr->description();
}
void KModelInfo::setDescription(const QString &d)
{
    dptr->setDescription(d);
}

QIcon KModelInfo::icon() const
{
    return dptr->icon();
}
void KModelInfo::setIcon(const QIcon &i)
{
    dptr->setIcon(i);
}
int KModelInfo::serialId() const
{
    return dptr->serialId();
}
