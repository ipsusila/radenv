#ifndef DOSEESTIMATION_H
#define DOSEESTIMATION_H

#include "imodel.h"
#include "defaultvalues.h"

class DoseEstimation : public IModel
{
public:
    enum {
        Inhalation = 1,
        Immersion = 2,
        ExternalGround = 3,
        ExternalIrrigation = 4,
        Ingestion = 5,
        ExternalSediment = 6,
        ExternalSludge = 7,
        InhalationSludge = 8,
        Sludge = 9
    };

    KDataArray result() const;
    bool calculate(const KCalculationInfo& ci);
    const KPortList & inputs() const;

    bool verify(int * err = 0, int * warn = 0);
protected:
    KPortList        _inpPorts;
    KDataArray       _dataList;
    ExternalDoseCoefficient       _extDf;
    InhalationDoseCoefficient       _inhDf;
    IngestionDoseCoefficient       _ingDf;

    DoseEstimation(IModelFactory * fact, const KModelInfo & inf);
    virtual bool allocateIoPorts();
    virtual void defineParameters();
    virtual KData modelData(const Quantity & qty) const;
    virtual void connectionModified(KPort * port, KConnector * con, bool connected);
    virtual bool needLocation() const;

    virtual qreal exposedFraction(int type) const = 0;
    virtual qreal consumptionRate(const Quantity & qty, int type) const = 0;
    virtual qreal inhalationRate() const = 0;
    virtual int inhalationCoeffIndex() const = 0;
    virtual int ingestionCoeffIndex() const = 0;

    void addQuantity(KPort * port, KConnector * con);
    void addExposedFraction(const IModel * model, int type);
    void addConsumptionRate(const IModel * model, const Quantity & qty);
    void removeQuantity(KPort * port, KConnector * con);

    void calcImmersionDose(QMap<QString, qreal> * totalDoses);
    void calcIngestionDose(QMap<QString, qreal> * totalDoses);
    void calcInhalationDose(QMap<QString, qreal> * totalDoses);
    void calcSedimentDose(QMap<QString, qreal> * totalDoses);
    void calcGroundDose(QMap<QString, qreal> * totalDoses);
    void calcExtSludgeDose(QMap<QString, qreal> * totalDoses);
    void calcInhSludgeDose(QMap<QString, qreal> * totalDoses);
    void calcSkinDose(QMap<QString, qreal> * totalDoses);

};

class InfantDoseEstimation : public DoseEstimation
{
public:
    enum { SerialId = 27 };

    InfantDoseEstimation(IModelFactory * fact, const KModelInfo & inf);
    QString displayText() const;
protected:
    qreal exposedFraction(int type) const;
    qreal consumptionRate(const Quantity & qty, int type) const;
    qreal inhalationRate() const;
    int inhalationCoeffIndex() const;
    int ingestionCoeffIndex() const;
};

class AdultDoseEstimation : public DoseEstimation
{
public:
    enum { SerialId = 28 };

    AdultDoseEstimation(IModelFactory * fact, const KModelInfo & inf);
    QString displayText() const;
protected:
    qreal exposedFraction(int type) const;
    qreal consumptionRate(const Quantity & qty, int type) const;
    qreal inhalationRate() const;
    int inhalationCoeffIndex() const;
    int ingestionCoeffIndex() const;
};

#endif // DOSEESTIMATION_H
