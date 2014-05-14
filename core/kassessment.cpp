/******************************************************************************
 * Author   : I Putu Susila (putu [at] batan.go.id)
 * Created  : 19/02/2014
 *
 * History  :
 ******************************************************************************/

#include <QSharedData>
#include <QRectF>
#include <QtDebug>
#include "kassessment.h"
#include "kscenario.h"

//Token identifier
static const QString __assessmentToken = "@Assessment_v-1.0";

class KAssessmentPrivate : public QSharedData {
public:
    QDateTime created;
    QString name;
    QString author;
    QString description;
    QString remark;
    QString docname;
    QByteArray document;
    ScenarioList scenarios;

    KAssessmentPrivate(const QDateTime & c) : created(c) {}
    ~KAssessmentPrivate()
    {
        qDebug() << "Removing assessment private : " << name;
        this->clear();
    }

    inline bool isValid() const
    {
        return !name.isEmpty();
    }
    inline KScenario * firstScenario() const
    {
        if (scenarios.isEmpty())
            return 0;
        return scenarios.first();
    }

    inline KScenario * createScenario(KAssessment * aP, const QRectF & rect = QRectF())
    {
        KScenario * scenario;
        if (rect.isValid())
            scenario = new KScenario(rect.x(), rect.y(), rect.width(), rect.height(), aP);
        else
            scenario = new KScenario(-800, -400, 1600, 800, aP);
        scenarios.append(scenario);
        return scenario;
    }
    inline void remove(KScenario * scene)
    {
        scenarios.removeOne(scene);
        delete scene;
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

        qDebug() << "Delete all scenes:" << scenarios.size();
        while (!scenarios.isEmpty()) {
            KScenario * scene = scenarios.takeFirst();
            delete scene;
        }
    }

    void serialize(QByteArray &ba) const
    {
        ba.clear();
        quint32 sz = (quint32)scenarios.size();
        QDataStream stream(&ba, QIODevice::WriteOnly);
        stream << sz;
        for(quint32 k = 0; k < sz; k++)
            scenarios.at(k)->serialize(stream);
    }
    QDataStream & serialize(QDataStream & s) const
    {
        s << __assessmentToken << created << name << author
          << description << remark << docname << document;

        quint32 sz = (quint32)scenarios.size();
        s << sz;
        for(quint32 k = 0; k < sz; k++)
            scenarios.at(k)->serialize(s);
        return s;
    }
    void deserialize(KAssessment * aP, const QByteArray &ba)
    {
        quint32 sz;
        QDataStream stream(ba);
        stream >> sz;
        scenarios.clear();
        for(quint32 k = 0; k < sz; k++) {
            KScenario * scene = createScenario(aP);
            scene->deserialize(stream);
        }
    }
    QDataStream & deserialize(KAssessment * aP, QDataStream & s)
    {
        QString token;
        s >> token;
        if (token == __assessmentToken) {
            s >> created >> name >> author >> description
              >> remark >> docname >> document;
            quint32 sz;
            s >> sz;
            for(quint32 k = 0; k < sz; k++) {
                KScenario * scene = createScenario(aP);
                scene->deserialize(s);
            }
        }
        return s;
    }
};

KAssessment::KAssessment(QObject *parent)
    : QObject(parent), dptr(new KAssessmentPrivate(QDateTime()))
{

}

KAssessment::KAssessment(const QDateTime & c, QObject *parent)
    : QObject(parent), dptr(new KAssessmentPrivate(c))
{
}

KAssessment::~KAssessment()
{
}

bool KAssessment::operator==(const KAssessment &other) const
{
    return dptr->name == other.dptr->name;
}
bool KAssessment::operator!=(const KAssessment &other) const
{
    return dptr->name != other.dptr->name;
}

bool KAssessment::isValid() const
{
    return dptr->isValid();
}
QDateTime KAssessment::created() const
{
    return dptr->created;
}
QString KAssessment::name() const
{
    return dptr->name;
}
void KAssessment::setName(const QString &name)
{
    dptr->name = name;
}

QString KAssessment::author() const
{
    return dptr->author;
}
void KAssessment::setAuthor(const QString &author)
{
    dptr->author = author;
}

QString KAssessment::description() const
{
    return dptr->description;
}
void KAssessment::setDescription(const QString &desc)
{
    dptr->description = desc;
}

QString KAssessment::remark() const
{
    return dptr->remark;
}
void KAssessment::setRemark(const QString &remark)
{
    dptr->remark = remark;
}

QString KAssessment::docname() const
{
    return dptr->docname;
}
void KAssessment::setDocname(const QString& docname)
{
    dptr->docname = docname;
}

QByteArray KAssessment::document() const
{
    return dptr->document;
}
void KAssessment::setDocument(const QByteArray &doc)
{
    dptr->document = doc;
}

void KAssessment::deserialize(const QByteArray & cont)
{
    dptr->deserialize(this, cont);
}
KScenario * KAssessment::firstScenario() const
{
    return dptr->firstScenario();
}
KScenario * KAssessment::createScenario(const QRectF &rect)
{
    return dptr->createScenario(this, rect);
}
ScenarioList KAssessment::scenarios() const
{
    return dptr->scenarios;
}
bool KAssessment::contains(KScenario * scene) const
{
    return dptr->scenarios.contains(scene);
}
void KAssessment::remove(KScenario * scene)
{
    dptr->remove(scene);
}

void KAssessment::clear()
{
    dptr->clear();
}

void KAssessment::serialize(QByteArray &ba) const
{
    dptr->serialize(ba);
}

QDataStream & KAssessment::serialize(QDataStream & stream) const
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    return dptr->serialize(stream);
}
QDataStream & KAssessment::deserialize(QDataStream & stream)
{
    qDebug() << Q_FUNC_INFO << ", stream pos: " << stream.device()->pos();
    return dptr->deserialize(this, stream);
}
