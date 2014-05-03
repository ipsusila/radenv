/******************************************************************************
 * Author   : I Putu Susila (putu [at] batan.go.id)
 * Created  : 19/02/2014
 *
 * History  :
 ******************************************************************************/

#include <QSharedData>
#include <QRectF>
#include <QDebug>
#include "kcase.h"
#include "kmodelscene.h"

//Token identifier
static const QString __assessmentToken = "@Assessment_v-1.0";

class KCasePrivate : public QSharedData {
public:
    QDateTime created;
    QString name;
    QString author;
    QString description;
    QString remark;
    QString docname;
    QByteArray document;
    SceneList scenes;

    KCasePrivate() : created(QDateTime::currentDateTime()) {}
    ~KCasePrivate()
    {
        qDebug() << "Removing case private";
        this->clear();
    }

    inline KModelScene * createScene(const QRectF & rect = QRectF())
    {
        KModelScene * scene;
        if (rect.isValid())
            scene = new KModelScene(rect.x(), rect.y(), rect.width(), rect.height());
        else
            scene = new KModelScene(-800, -400, 1600, 800);
        scenes.append(scene);
        return scene;
    }
    void clear()
    {
        created = QDateTime::currentDateTime();
        name.clear();
        author.clear();
        description.clear();
        remark.clear();
        docname.clear();
        document.clear();

        while (!scenes.isEmpty()) {
            KModelScene * scene = scenes.takeFirst();
            delete scene;
        }
    }

    void serialize(QByteArray &ba) const
    {
        ba.clear();
        quint32 sz = (quint32)scenes.size();
        QDataStream stream(&ba, QIODevice::WriteOnly);
        stream << sz;
        for(quint32 k = 0; k < sz; k++)
            scenes.at(k)->serialize(stream);
    }
    QDataStream & serialize(QDataStream & s) const
    {
        s << __assessmentToken << created << name << author
          << description << remark << docname << document;

        quint32 sz = (quint32)scenes.size();
        s << sz;
        for(quint32 k = 0; k < sz; k++)
            scenes.at(k)->serialize(s);
        return s;
    }
    void deserialize(const QByteArray &ba)
    {
        quint32 sz;
        QDataStream stream(ba);
        stream >> sz;
        scenes.clear();
        for(quint32 k = 0; k < sz; k++) {
            KModelScene * scene = createScene();
            scene->deserialize(stream);
            scenes.append(scene);
        }
    }
    QDataStream & deserialize(QDataStream & s)
    {
        QString token;
        s >> token;
        if (token == __assessmentToken) {
            s >> created >> name >> author >> description
              >> remark >> docname >> document;
            quint32 sz;
            s >> sz;
            for(quint32 k = 0; k < sz; k++) {
                KModelScene * scene = createScene();
                scene->deserialize(s);
                scenes.append(scene);
            }
        }
        return s;
    }
};

KCase::KCase(const QDateTime & c) : data(new KCasePrivate)
{
    if (c.isValid())
        data->created = c;
}

KCase::KCase(const KCase &rhs) : data(rhs.data)
{
}

KCase &KCase::operator=(const KCase &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
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

void KCase::deserialize(const QByteArray & cont)
{
    data->deserialize(cont);
}

KModelScene * KCase::createScene(const QRectF &rect)
{
    return data->createScene(rect);
}
SceneList KCase::scenes() const
{
    return data->scenes;
}
void KCase::clear()
{
    data->clear();
}

void KCase::serialize(QByteArray &ba) const
{
    data->serialize(ba);
}

QDataStream & KCase::serialize(QDataStream & stream) const
{
    return data->serialize(stream);
}
QDataStream & KCase::deserialize(QDataStream & stream)
{
    return data->deserialize(stream);
}
