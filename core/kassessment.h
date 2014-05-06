#ifndef KASSESSMENT_H
#define KASSESSMENT_H

#include <QRectF>
#include "iserializable.h"

class KCasePrivate;
class RADENV_API KAssessment : public ISerializable
{
public:
    KAssessment(const QDateTime & created = QDateTime());
    KAssessment(const KAssessment &other);
    KAssessment &operator=(const KAssessment &other);
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

    KModelScene * createScene(const QRectF & rect = QRectF());
    SceneList scenes() const;
    bool contains(KModelScene * scene) const;
    void remove(KModelScene * scene);
    void clear();

    void deserialize(const QByteArray & cont);
    void serialize(QByteArray &ba) const;
    virtual QDataStream & serialize(QDataStream & stream) const;
    virtual QDataStream & deserialize(QDataStream & stream);

private:
    QExplicitlySharedDataPointer<KCasePrivate> dptr;
};
K_DECLARE_SERIALIZABLE(KAssessment)

#endif // KASSESSMENT_H
