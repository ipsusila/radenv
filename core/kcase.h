#ifndef KCASE_H
#define KCASE_H

#include <QSharedDataPointer>
#include "radglobal.h"

class KCasePrivate;
class K_CORE_EXPORT KCase : public QByteArray
{
public:
    KCase(const QDateTime & created = QDateTime());
    KCase(const KCase &other);
    KCase &operator=(const KCase &other);
    ~KCase();

    bool operator==(const KCase &other) const;
    bool operator!=(const KCase &other) const;

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
    const QByteArray & content() const;
    
private:
    QSharedDataPointer<KCasePrivate> data;
};

#endif // KCASE_H
