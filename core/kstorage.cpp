#include <QtDebug>
#include <QtSql>
#include <QStringList>
#include "kstorage.h"
#include "koutput.h"
#include "imodelfactory.h"
#include "klocation.h"
#include "kradionuclide.h"
#include "kassessment.h"

#include "kradionuclide.hxx"

static const KRadionuclide __nullNuclide;
KStorage::KStorage(const QString& nm)
{
    xTrace() << QObject::tr("Create/open storage: ") << nm;
    open(nm);
}

//not used
KStorage::KStorage(const KStorage & /*rhs*/)
{
}
KStorage & KStorage::operator=(const KStorage &)
{
    return *this;
}
//not used

KStorage::~KStorage()
{
    qDebug() << "Storage object deleted: " << _stgName;
    close();
}

QString KStorage::storageName() const
{
    return _stgName;
}
bool KStorage::isNull() const
{
    return _stgName.isEmpty();
}
bool KStorage::isOpen() const
{
    return QSqlDatabase::database().isOpen();
}
bool KStorage::isValid() const
{
    //check tables
    QSqlDatabase db = QSqlDatabase::database();
    QStringList tables = db.tables();
    return !_stgName.isEmpty() && db.isValid() && !tables.isEmpty();
}

void KStorage::close()
{
    if (isNull())
        return;

    //first remove existing data
    QString dbName;
    {
        QSqlDatabase db = QSqlDatabase::database();
        if (db.isValid()) {
            dbName = db.connectionName();
            if (db.isOpen())
                db.close();
        }
    }

    //close the storage database
    if (!dbName.isEmpty())
        QSqlDatabase::removeDatabase(dbName);
}

bool KStorage::open(const QString& nm)
{
    if (nm == _stgName)
        return true;
    _stgName = nm;
    if (_stgName.isEmpty())
        return false;

    //first remove existing data
    close();

    //now open new database
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(_stgName);
    if (!db.open()) {
        xError() << Q_FUNC_INFO << QObject::tr(" [Storage]: Error when openning database ") << _stgName;
        return false;
    }

    bool result = initialize();
    result = loadAll() && result;

    return result;
}
bool KStorage::initialize()
{
    //check tables
    QSqlDatabase db = QSqlDatabase::database();
    QStringList tables = db.tables();

    //create tables from SQL
    xTrace() << Q_FUNC_INFO << QObject::tr(" [Storage]: Initializing storage");

    /*
     *#define RAD_STORAGEID                 "_x_smea_storage_"
        #define RAD_RADIONUCLIDE              "x_smea_radionuclide"
        #define RAD_STORAGE                   "x_smea_storage"
        #define RAD_LOCATION                  "x_smea_location"
        #define RAD_ASSESSMENT                "x_smea_assessment"
        #define RAD_NULL_FACTORY              "__null__"
        */


    bool result = true;
    if (!tables.contains(RAD_STORAGE))
        result = initStorageTable() && result;
    if (!tables.contains(RAD_RADIONUCLIDE))
        result = initNuclideTable() && result;
    if (!tables.contains(RAD_LOCATION))
        result = initLocationTable() && result;
    if (!tables.contains(RAD_ASSESSMENT))
        result = initAssessmentTable() && result;

    //if (tables.size() < 4)
        optimize();

    return result;
}

void KStorage::optimize()
{
    QSqlQuery query;
    query.exec("VACUUM");
}

bool KStorage::initStorageTable()
{
    //create storage tables
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1 (name VARCHAR(255) NOT NULL,"
                          "factory VARCHAR(255) NOT NULL, created DATETIME, "
                          "description TEXT, size INTEGER, data BLOB, "
                          "PRIMARY KEY(name,factory))")
            .arg(RAD_STORAGE);
    if (!query.exec(sql)) {
        xWarning() << Q_FUNC_INFO << QObject::tr(" [Storage]: Failed to execute query -> ") << sql;
        xWarning() << QObject::tr("[Storage]: ") << query.lastError().text();

        return false;
    }

    return true;
}

bool KStorage::initNuclideTable()
{
    QSqlQuery query;

    //create radionuclide tables
    QString sql = QString("CREATE TABLE %1 (nuclide VARCHAR(32) NOT NULL,decay DOUBLE,"
                          "usage INT NOT NULL DEFAULT 0, attr INT NOT NULL DEFAULT 0, "
                          "PRIMARY KEY(nuclide))").arg(RAD_RADIONUCLIDE);
    if (!query.exec(sql)) {
        xWarning() << Q_FUNC_INFO << QObject::tr(" [Storage]: Failed to execute query -> ") << sql;
        xWarning() << QObject::tr("[Storage]: ") << query.lastError().text();

        return false;
    }

    /*
     * inserting data directly is time consumming due to index generation.
     * first insert into temporary tables, without index, than copy to
     * real table which has PRIMARY KEY attribute.
     */

    QString tempTable = "__temp_nuclide__";
    sql = QString("CREATE TEMPORARY TABLE %1 (nuclide VARCHAR(32), decay DOUBLE,"
                  "usage INT NOT NULL DEFAULT 0, attr INT NOT NULL DEFAULT 0)")
            .arg(tempTable);
    query.exec(sql);

    sql = QString("INSERT INTO %1 (nuclide, decay, attr) VALUES (?,?,?)")
            .arg(tempTable);
    query.prepare(sql);

    QVariantList nuclides;
    QVariantList lambdas;
    QVariantList attributes;
    int nnuclide = sizeof(__radionuclide) / sizeof(RadionuclideItem);
    for(int k = 0; k < nnuclide; k++) {
        nuclides << __radionuclide[k].nuclide;
        lambdas << __radionuclide[k].lambda;
        attributes << __radionuclide[k].attribute;
    }

    query.addBindValue(nuclides);
    query.addBindValue(lambdas);
    query.addBindValue(attributes);
    if (!query.execBatch()) {
        xError() << Q_FUNC_INFO << QObject::tr(" [Storage]: Insert data error -> ")
                 << query.lastError().text();
    }

    //copy content
    sql = QString("INSERT INTO %1 SELECT nuclide,decay,usage,attr FROM %2")
            .arg(RAD_RADIONUCLIDE).arg(tempTable);
    query.exec(sql);

    sql = QString("DROP TABLE %1").arg(tempTable);
    query.exec(sql);

    return true;
}

bool KStorage::initLocationTable()
{
    /*
     - code
     - name
     - description
     - latitude
     - longitude
     - angle
     - distance
     - marker (blob)
    */
    //create location tables
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1 (code VARCHAR(32) NOT NULL, "
                          "name VARCHAR(255) NOT NULL, description TEXT, "
                          "latitude DOUBLE, longitude DOUBLE, angle DOUBLE, "
                          "distance TEXT, size INTEGER, marker BLOB, "
                          "PRIMARY KEY(code))")
            .arg(RAD_LOCATION);
    if (!query.exec(sql)) {
        xWarning() << Q_FUNC_INFO << QObject::tr(" [Storage]: Failed to execute query -> ") << sql;
        xWarning() << QObject::tr("[Storage]: ") << query.lastError().text();

        return false;
    }

    return true;
}

bool KStorage::initAssessmentTable()
{
    /*
     - name
     - created
     - author
     - description
     - remark
     - docname
     - document (blob)
    */
    //create assessment tables
    QSqlQuery query;
    QString sql = QString("CREATE TABLE %1 (name VARCHAR(255) NOT NULL, created DATETIME,"
                          "author VARCHAR(255) NOT NULL, description TEXT, "
                          "remark TEXT, docname VARCHAR(255), docsz INTEGER, "
                          "document BLOB, datasz INTEGER, data BLOB, PRIMARY KEY(name))")
            .arg(RAD_ASSESSMENT);
    if (!query.exec(sql)) {
        xWarning() << Q_FUNC_INFO << QObject::tr(" [Storage]: Failed to execute query -> ") << sql;
        xWarning() << QObject::tr("[Storage]: ") << query.lastError().text();

        return false;
    }

    return true;
}

bool KStorage::save(const KLocation& loc)
{
    //save to database
    QSqlQuery query;
    QString sql = QString("INSERT OR REPLACE INTO %1 (code, name, description,"
                          "latitude, longitude, angle, distance, size, marker) "
                          "VALUES(?,?,?,?,?,?,?,?,?)").arg(RAD_LOCATION);
    query.prepare(sql);
    query.bindValue(0, loc.code());
    query.bindValue(1, loc.name());
    query.bindValue(2, loc.description());
    query.bindValue(3, loc.latitude());
    query.bindValue(4, loc.longitude());
    query.bindValue(5, loc.angle());
    query.bindValue(6, loc.distances(KLocation::Delimiter));
    //query.bindValue(6, loc.distance());

    QImage marker = loc.marker();
    if (marker.isNull()) {
        query.bindValue(7, 0);
        query.bindValue(8, QVariant(QVariant::ByteArray));
    }
    else {
        QByteArray ba;
        QDataStream ds(&ba, QIODevice::WriteOnly | QIODevice::ReadOnly);
        ds << marker;

        query.bindValue(7, ba.size());
        query.bindValue(8, ba);
    }

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());

        return false;
    }

    int pos = _locations.indexOf(loc);
    if (pos < 0) {
        _locations.append(loc);
    }
    else {
        _locations[pos] = loc;
    }

    return true;
}

bool KStorage::save(const KRadionuclide& nuc)
{
    //save or update to database
    QSqlQuery query;
    QString sql;

    //update list
    int pos = _radionuclides.indexOf(nuc);
    if (pos < 0) {
        _radionuclides.append(nuc);
        sql = QString("INSERT INTO %1 (decay, usage, attr, nuclide) "
                      "VALUES(?,?,?,?)").arg(RAD_RADIONUCLIDE);
    }
    else {
        _radionuclides[pos] = nuc;
        sql = QString("UPDATE %1 SET decay=?, usage=?, attr=? WHERE nuclide=?")
            .arg(RAD_RADIONUCLIDE);
    }

    query.prepare(sql);
    query.bindValue(0, nuc.halfLife().decayConstant());
    query.bindValue(1, nuc.usage());
    query.bindValue(2, nuc.attributes());
    query.bindValue(3, nuc.nuclide());

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());

        return false;
    }

    return true;
}

bool KStorage::save(const KAssessment &a)
{
    //save to database
    QSqlQuery query;
    QString sql = QString("INSERT OR REPLACE INTO %1 (name, created, author,"
                          "description, remark, docname, docsz, document, datasz, data) "
                          "VALUES(?,?,?,?,?,?,?,?,?,?)").arg(RAD_ASSESSMENT);
    QByteArray ba;
    a.serialize(ba);

    query.prepare(sql);
    query.bindValue(0, a.name());
    query.bindValue(1, a.created());
    query.bindValue(2, a.author());
    query.bindValue(3, a.description());
    query.bindValue(4, a.remark());
    query.bindValue(5, a.docname());
    query.bindValue(6, a.document().size());
    query.bindValue(7, a.document());
    query.bindValue(8, ba.size());
    query.bindValue(9, ba);

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());

        return false;
    }
    return true;
}

void KStorage::remove(const KLocation& loc)
{
    //remove from database
    QSqlQuery query;
    QString sql = QString("DELETE FROM %1 WHERE code=?").arg(RAD_LOCATION);
    query.prepare(sql);
    query.bindValue(0, loc.code());

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());
    }
    else {
        _locations.removeOne(loc);
    }
}

void KStorage::remove(const KAssessment& a)
{
    //remove from database
    QSqlQuery query;
    QString sql = QString("DELETE FROM %1 WHERE name=?").arg(RAD_ASSESSMENT);
    query.prepare(sql);
    query.bindValue(0, a.name());

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());
    }
}
void KStorage::remove(const KRadionuclide &nuc)
{
    //remove from database
    QSqlQuery query;
    QString sql = QString("DELETE FROM %1 WHERE nuclide=?").arg(RAD_RADIONUCLIDE);
    query.prepare(sql);
    query.bindValue(0, nuc.nuclide());

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());
    }
    else {
        _radionuclides.removeOne(nuc);
    }
}

bool KStorage::save(const KStorageContent& content)
{
    //save to database
    QSqlQuery query;
    QString sql = QString("INSERT OR REPLACE INTO %1 (name,factory,created,"
                          "description,size,data) VALUES(?,?,?,?,?,?)")
            .arg(RAD_STORAGE);
    query.prepare(sql);
    query.bindValue(0, content.name());
    query.bindValue(1, content.factoryName());
    query.bindValue(2, content.created());
    query.bindValue(3, content.description());
    query.bindValue(4, content.size());
    query.bindValue(5, content);

    if (!query.exec()) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());

        return false;
    }

    return true;
}

KStorageContent KStorage::load(const QString& nm, const IModelFactory * f) const
{
    QSqlQuery query;
    QString sql = QString("SELECT name,factory,created,description,size,data "
                          "FROM %1 WHERE name=? AND factory=?").arg(RAD_STORAGE);

    query.prepare(sql);
    query.bindValue(0, nm);
    query.bindValue(1, f == 0 ? RAD_NULL_FACTORY : f->name());

    //load from database
    bool ok;
    if ((ok = query.exec()) && query.next()) {
        KStorageContent content(query.value(2).toDateTime());

        content.setName(query.value(0).toString());
        content.setFactory(f);
        content.setDescription(query.value(3).toString());
        content.append(query.value(5).toByteArray());

        int size = query.value(4).toInt();
        qDebug() << "[XStorage] DB size: " << size << ", stream size: " << content.size();

        return content;
    }
    else if (!ok) {
        qDebug() << Q_FUNC_INFO << " Error: " << query.lastError();
        qDebug() << "SQL: " << sql;
    }

    return KStorageContent();
}
const RadionuclideList * KStorage::radionuclides() const
{
    return &_radionuclides;
}
const KRadionuclide &  KStorage::radionuclide(const QString& nm) const
{
    for (int k = 0; k < _radionuclides.size(); k++) {
        const KRadionuclide & nuc = _radionuclides.at(k);
        if (nuc.nuclide() == nm)
            return nuc;
    }
    return __nullNuclide;
}

KLocation KStorage::location(const QString& code) const
{
    LocationList::const_iterator loc = _locations.constBegin();
    LocationList::const_iterator end = _locations.constEnd();
    while(loc != end) {
        if (loc->code() == code)
            return *loc;
        loc++;
    }
    return KLocation();
}

const LocationList * KStorage::locations() const
{
    return &_locations;
}

RadionuclideList KStorage::loadNuclides()
{
    QSqlQuery query;
    QString sql = QString("SELECT nuclide,decay,usage,attr FROM %1").arg(RAD_RADIONUCLIDE);

    RadionuclideList list;
    if (!query.exec(sql)) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());

        return list;
    }

    while (query.next()) {
        KRadionuclide nuc(
                    query.value(0).toString(),
                    KHalfLife(query.value(1).toDouble(), true),
                    query.value(2).toInt());
        nuc.addAttribute(query.value(3).toInt());
        list.append(nuc);
    }

    return list;
}

LocationList KStorage::loadLocations()
{
    //load from database
    QSqlQuery query;
    QString sql = QString("SELECT code, name, description,latitude, longitude, "
                          "angle, distance, size, marker FROM %1").arg(RAD_LOCATION);

    LocationList list;
    if (!query.exec(sql)) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());

        return list;
    }

    while (query.next()) {
        KLocation loc(query.value(0).toString());
        loc.setName(query.value(1).toString());
        loc.setDescription(query.value(2).toString());
        loc.setLatitude(query.value(3).toDouble());
        loc.setLongitude(query.value(4).toDouble());
        loc.setAngle(query.value(5).toDouble());
        loc.setDistance(query.value(6).toString(), KLocation::Delimiter);
        //loc.addDistance(query.value(6).toDouble());

        QByteArray ba = query.value(8).toByteArray();
        if (!ba.isEmpty()) {
            QImage img;
            QDataStream stream(ba);
            stream >> img;
            loc.setMarker(img);
        }

        //add location
        list.append(loc);
    }
    return list;
}
bool KStorage::assessmentExists(const QString& assessment) const
{
    /*
    //TODO
    //This parameter query give parameter count mismatch ????
    QSqlQuery query;
    QString sql = QString("SELECT name FROM %1 WHERE name=?").arg(RAD_ASSESSMENT);
    query.prepare(sql);
    query.bindValue(0, assessment);
    if (query.exec(sql) && query.next()) {
        return true;
    }
    else {
        qDebug() << (QObject::tr("[Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());
    }
    */

    QString val = assessment;
    val.replace('\'', "");

    QSqlQuery query;
    QString sql = QString("SELECT name FROM %1 WHERE name='%2'")
            .arg(RAD_ASSESSMENT)
            .arg(val);

    if (!query.exec(sql)) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());

        return false;
    }

    if (query.next()) {
        Q_ASSERT(query.value(0).toString() == assessment);
        return true;
    }

    return false;
}
AssessmentList KStorage::loadAssessmentPreviews(const QStringList & excludes, QObject *parent) const
{
    QSqlQuery query;
    AssessmentList list;
    QString sql = QString("SELECT name, created, author, description, remark, docname FROM %1")
            .arg(RAD_ASSESSMENT);

    if (!query.exec(sql)) {
        qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
        qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());

        return list;
    }

    while (query.next()) {
        QString name = query.value(0).toString();
        if (excludes.contains(name))
            continue;

        KAssessment *aP = new KAssessment(query.value(1).toDateTime(), parent);
        aP->setName(name);
        aP->setAuthor(query.value(2).toString());
        aP->setDescription(query.value(3).toString());
        aP->setRemark(query.value(4).toString());
        aP->setDocname(query.value(5).toString());

        list.append(aP);
    }

    return list;
}

AssessmentList KStorage::loadAssessments(const QStringList & names, QObject * parent) const
{
    QSqlQuery query;
    AssessmentList list;

    //TODO
    //Prepared statement is not working in SELECT
    //Error: parameter count mismatch
    QString sql = QString("SELECT name, created, author, description, remark,"
                          "docname, docsz, document, datasz, data FROM %1 WHERE name=")
            .arg(RAD_ASSESSMENT);

    //load designated assessments
    for(int k = 0; k < names.size(); k++) {
        QString name = names.at(k);
        name.replace('\'', "");
        name = "'" + name + "'";
        if (!query.exec(sql+name)) {
            qDebug() << Q_FUNC_INFO << (QObject::tr(" [Storage]: Failed to execute query -> ") + sql);
            qDebug() << (QObject::tr("[Storage]: ") + query.lastError().text());

            continue;
        }

        while (query.next()) {
            KAssessment *aP = new KAssessment(query.value(1).toDateTime(), parent);
            aP->setName(query.value(0).toString());
            aP->setAuthor(query.value(2).toString());
            aP->setDescription(query.value(3).toString());
            aP->setRemark(query.value(4).toString());
            aP->setDocname(query.value(5).toString());

            int docsz = query.value(6).toInt();
            QByteArray doc = query.value(7).toByteArray();
            if (docsz > 0 && docsz == doc.size())
                aP->setDocument(doc);

            int datasz = query.value(8).toInt();
            QByteArray content = query.value(9).toByteArray();
            if (datasz > 0 && content.size() == datasz)
                aP->deserialize(content);

            list.append(aP);
        }
    }

    return list;
}

bool KStorage::loadAll()
{
    _radionuclides = loadNuclides();
    _locations = loadLocations();

    return true;
}
