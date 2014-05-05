#ifndef KSTORAGE_H
#define KSTORAGE_H

#include "kstoragecontent.h"

class K_CORE_EXPORT KStorage
{
    RadionuclideList loadNuclides();
    LocationList loadLocations();

    bool initialize();
    bool initStorageTable();
    bool initNuclideTable();
    bool initLocationTable();
    bool initAssessmentTable();
public:
    explicit KStorage(const QString& nm);
    ~KStorage();

    bool loadAll();
    AssessmentList loadAssessments(const QStringList & names) const;
    AssessmentList loadAssessmentPreviews(const QStringList & excludes) const;
    bool assessmentExists(const QString& assessment) const;

    bool isNull() const;
    bool isOpen() const;
    bool isValid() const;
    QString storageName() const;
    bool open(const QString& nm);
    void close();
    void optimize();

    void remove(const KLocation& loc);
    void remove(const KAssessment& a);
    void remove(const KRadionuclide& nuc);

    bool save(const KLocation& loc);
    bool save(const KRadionuclide& nuc);
    bool save(const KAssessment& a);
    bool save(const KStorageContent& content);
    KStorageContent load(const QString& nm, const IModelFactory * f) const;

    const RadionuclideList * radionuclides() const;
    const KRadionuclide & radionuclide(const QString& nm) const;

    KLocation location(const QString& code) const;
    const LocationList * locations() const;

private:
    QString _stgName;
    RadionuclideList _radionuclides;
    LocationList _locations;

    KStorage(const KStorage &);
    KStorage &operator=(const KStorage &);
};

#endif // KSTORAGE_H
