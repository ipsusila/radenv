#include "kgroupinfo.h"

KGroupInfo::KGroupInfo()
{
}

KGroupInfo::KGroupInfo(const QString& n, const ModelInfoList &inf)
    : _name(n), _infos(inf)
{
}

ModelInfoList KGroupInfo::infos() const
{
    return _infos;
}
void KGroupInfo::addInfo(const KModelInfo& inf)
{
    _infos.append(inf);
}
void KGroupInfo::addInfos(const ModelInfoList& inf)
{
    _infos.append(inf);
}

void KGroupInfo::clear()
{
    _infos.clear();
    _children.clear();
}

QString KGroupInfo::name() const
{
    return _name;
}
void KGroupInfo::setName(const QString& nm)
{
    _name = nm;
}
GroupInfoList KGroupInfo::children() const
{
    return _children;
}
void KGroupInfo::add(const KGroupInfo &gi)
{
    _children.append(gi);
}
