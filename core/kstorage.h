#ifndef KSTORAGE_H
#define KSTORAGE_H

#include "kstoragecontent.h"

class K_CORE_EXPORT KStorage
{
    RadionuclideList loadNuclides();
    LocationList loadLocations();
    AssessmentList loadAssessments();

    bool initialize();
    bool initStorageTable();
    bool initNuclideTable();
    bool initLocationTable();
    bool initAssessmentTable();
public:
    explicit KStorage(const QString& nm);
    ~KStorage();

    bool loadAll();

    bool isNull() const;
    bool isOpen() const;
    bool isValid() const;
    QString storageName() const;
    bool open(const QString& nm);
    void close();
    void optimize();

    void remove(const KLocation& loc);
    void remove(const KCase& a);
    void remove(const KRadionuclide& nuc);

    bool save(const KLocation& loc);
    bool save(const KRadionuclide& nuc);
    bool save(const KCase& a);
    bool save(const KStorageContent& content);
    KStorageContent load(const QString& nm, const IModelFactory * f) const;

    const RadionuclideList * radionuclides() const;
    const KRadionuclide & radionuclide(const QString& nm) const;

    KLocation location(const QString& code) const;
    const LocationList * locations() const;
    const AssessmentList * assessments() const;

    static KStorage * storage(const QString& name = QString());
    static KStorage * addStorage(const QString& nm);
    static void removeStorage(const QString& nm);
    static void removeStorages();
    
private:
    QString _stgName;
    RadionuclideList _radionuclides;
    LocationList _locations;
    AssessmentList _assessments;

    KStorage(const KStorage &);
    KStorage &operator=(const KStorage &);
};

#endif // KSTORAGE_H
