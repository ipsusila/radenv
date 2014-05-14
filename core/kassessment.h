#ifndef KASSESSMENT_H
#define KASSESSMENT_H

#include <QRectF>
#include "iserializable.h"

class KAssessmentPrivate;
class RADENV_API KAssessment : public QObject, public ISerializable
{
    Q_OBJECT
public:
    explicit KAssessment(QObject * parent);
    KAssessment(const QDateTime & created, QObject * parent);
    ~KAssessment();

    bool operator==(const KAssessment &other) const;
    bool operator!=(const KAssessment &other) const;

    bool isValid() const;
    QDateTime created() const;
    QString name() const;
    void setName(const QString& name);
    QString author() const;
    void setAuthor(const QString& author);
    QString description() const;
    void setDescription(const QString& desc);
    QString remark() const;
    void setRemark(const QString& remark);
    QString docname() const;
    void setDocname(const QString& docname);
    QByteArray document() const;
    void setDocument(const QByteArray & doc);

    KScenario * createScenario(const QRectF & rect = QRectF());
    KScenario * firstScenario() const;
    ScenarioList scenarios() const;
    bool contains(KScenario * scene) const;
    void remove(KScenario * scene);
    void clear();

    void deserialize(const QByteArray & cont);
    void serialize(QByteArray &ba) const;
    virtual QDataStream & serialize(QDataStream & stream) const;
    virtual QDataStream & deserialize(QDataStream & stream);

private:
    Q_DISABLE_COPY(KAssessment)
    QExplicitlySharedDataPointer<KAssessmentPrivate> dptr;
};
K_DECLARE_SERIALIZABLE(KAssessment)

#endif // KASSESSMENT_H
