#ifndef KAPPLICATION_H
#define KAPPLICATION_H

#include <QApplication>
#include "radglobal.h"

class KApplicationPrivate;
class RADENV_API KApplication : public QApplication
{
public:
    KApplication(int &argc, char ** argv);
    ~KApplication();

    int loadPlugins(const QString& path = QString());
    bool unloadPlugins();

    void toValidDialogGeometry(QRect * rGeometry, QSize * maxSize) const;
    void setupValidGeometry(const QString& settingName, QWidget * widget);
    bool isValid() const;
    int factoryCount() const;
    FactoryList factories() const;
    IModelFactory * factory(const Quantity * qty) const;
    IModelFactory * factory(const QString & name) const;
    IModel * createModel(IModelFactory * fact, const KModelInfo & mi) const;
    IModel * createModel(const QString& factName, int infoId) const;
    IModel * createModel(QDataStream &stream) const;
    void setStorage(const QString& name);
    void closeStorage();
    KStorage * storage() const;
    bool hasStorage() const;
    KOptions * options() const;

    const Quantity * findQuantity(IModelFactory * fact, unsigned int qid) const;
    const Quantity * findQuantity(const QString &factoryName, unsigned int qid) const;

    void serialize(QDataStream & stream, const Quantity * qty) const;
    const Quantity * deserialize(QDataStream & stream) const;

    static KApplication * selfInstance();
    
private:
    Q_DISABLE_COPY(KApplication)
    QSharedDataPointer<KApplicationPrivate> dptr;
};

#endif // KAPPLICATION_H
