#ifndef KGROUPINFO_H
#define KGROUPINFO_H

#include "kmodelinfo.h"

class K_CORE_EXPORT KGroupInfo
{
public:
    KGroupInfo();
    KGroupInfo(const QString& n, const ModelInfoList &inf);

    ModelInfoList infos() const;
    void addInfo(const KModelInfo& info);
    void addInfos(const ModelInfoList& inf);
    void clear();
    QString name() const;
    void setName(const QString& nm);
    GroupInfoList children() const;
    void add(const KGroupInfo& gi);
private:
    QString _name;
    ModelInfoList _infos;
    GroupInfoList _children;
};

#endif // KGROUPINFO_H
