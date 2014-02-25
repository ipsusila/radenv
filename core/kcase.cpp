/******************************************************************************
 * Author   : I Putu Susila (putu [at] batan.go.id)
 * Created  : 19/02/2014
 *
 * History  :
 ******************************************************************************/

#include <QSharedData>
#include "kcase.h"

class KCasePrivate : public QSharedData {
public:
    QDateTime created;
    QString name;
    QString author;
    QString description;
    QString remark;
    QString docname;
    QByteArray document;

    KCasePrivate() : created(QDateTime::currentDateTime()) {}
};

KCase::KCase(const QDateTime & c) : data(new KCasePrivate)
{
    if (c.isValid())
        data->created = c;
}

KCase::KCase(const KCase &rhs) : QByteArray(rhs), data(rhs.data)
{
}

KCase &KCase::operator=(const KCase &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
        QByteArray::operator=(rhs);
    }
    return *this;
}

KCase::~KCase()
{
}

bool KCase::operator==(const KCase &other) const
{
    return data->name == other.data->name;
}
bool KCase::operator!=(const KCase &other) const
{
    return data->name != other.data->name;
}

QDateTime KCase::created() const
{
    return data->created;
}
QString KCase::name() const
{
    return data->name;
}
void KCase::setName(const QString &name)
{
    data->name = name;
}

QString KCase::author() const
{
    return data->author;
}
void KCase::setAuthor(const QString &author)
{
    data->author = author;
}

QString KCase::description() const
{
    return data->description;
}
void KCase::setDescription(const QString &desc)
{
    data->description = desc;
}

QString KCase::remark() const
{
    return data->remark;
}
void KCase::setRemark(const QString &remark)
{
    data->remark = remark;
}

QString KCase::docname() const
{
    return data->docname;
}
void KCase::setDocname(const QString& docname)
{
    data->docname = docname;
}

QByteArray KCase::document() const
{
    return data->document;
}
void KCase::setDocument(const QByteArray &doc)
{
    data->document = doc;
}

const QByteArray& KCase::content() const
{
    return *this;
}
