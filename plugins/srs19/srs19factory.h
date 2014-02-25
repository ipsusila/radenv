#ifndef SRS19FACTORY_H
#define SRS19FACTORY_H

#include "srs19.h"
#include "imodelfactory.h"

class SRS19_SHARED_EXPORT Srs19Factory : public QObject, public IModelFactory
{
    Q_OBJECT
    Q_INTERFACES(IModelFactory)
public:
    virtual ~Srs19Factory();

    bool isTopLevel() const;
    bool initialize();
    IModel * createModel(const KModelInfo & info);
    const GroupInfoList & groups() const;
    QString name() const;
    QString author() const;
    QString version() const;
    QString description() const;

    IParamsEditor * paramsEditor();
private:
    GroupInfoList _groupInfos;
    IParamsEditor * _paramsEditor;
};

#endif // SRS19FACTORY_H
