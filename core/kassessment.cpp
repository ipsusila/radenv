/******************************************************************************
 * Author   : I Putu Susila (putu [at] batan.go.id)
 * Created  : 19/02/2014
 *
 * History  :
 ******************************************************************************/

#include <QSharedData>
#include <QRectF>
#include <QDebug>
#include "kassessment.h"
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
        qDebug() << "Removing assessment private";
        this->clear();
    }

    inline bool isValid() const
    {
        return !name.isEmpty();
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
    inline void remove(KModelScene * scene)
    {
        scenes.removeOne(scene);
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

        qDebug() << "Delete all scenes";
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

KAssessment::KAssessment(const QDateTime & c) : data(new KCasePrivate)
{
    if (c.isValid())
        data->created = c;
}

KAssessment::KAssessment(const KAssessment &rhs) : data(rhs.data)
{
}

KAssessment &KAssessment::operator=(const KAssessment &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

KAssessment::~KAssessment()
{
}

bool KAssessment::operator==(const KAssessment &other) const
{
    return data->name == other.data->name;
}
bool KAssessment::operator!=(const KAssessment &other) const
{
    return data->name != other.data->name;
}

bool KAssessment::isValid() const
{
    return data->isValid();
}
QDateTime KAssessment::created() const
{
    return data->created;
}
QString KAssessment::name() const
{
    return data->name;
}
void KAssessment::setName(const QString &name)
{
    data->name = name;
}

QString KAssessment::author() const
{
    return data->author;
}
void KAssessment::setAuthor(const QString &author)
{
    data->author = author;
}

QString KAssessment::description() const
{
    return data->description;
}
void KAssessment::setDescription(const QString &desc)
{
    data->description = desc;
}

QString KAssessment::remark() const
{
    return data->remark;
}
void KAssessment::setRemark(const QString &remark)
{
    data->remark = remark;
}

QString KAssessment::docname() const
{
    return data->docname;
}
void KAssessment::setDocname(const QString& docname)
{
    data->docname = docname;
}

QByteArray KAssessment::document() const
{
    return data->document;
}
void KAssessment::setDocument(const QByteArray &doc)
{
    data->document = doc;
}

void KAssessment::deserialize(const QByteArray & cont)
{
    data->deserialize(cont);
}

KModelScene * KAssessment::createScene(const QRectF &rect)
{
    return data->createScene(rect);
}
SceneList KAssessment::scenes() const
{
    return data->scenes;
}
bool KAssessment::contains(KModelScene * scene) const
{
    return data->scenes.contains(scene);
}
void KAssessment::remove(KModelScene * scene)
{
    data->remove(scene);
}

void KAssessment::clear()
{
    data->clear();
}

void KAssessment::serialize(QByteArray &ba) const
{
    data->serialize(ba);
}

QDataStream & KAssessment::serialize(QDataStream & stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    return data->serialize(stream);
}
QDataStream & KAssessment::deserialize(QDataStream & stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    return data->deserialize(stream);
}
