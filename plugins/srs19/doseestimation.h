#ifndef DOSEESTIMATION_H
#define DOSEESTIMATION_H

#include "imodel.h"

class DoseEstimation : public IModel
{
protected:
    DoseEstimation(IModelFactory * fact, const KModelInfo & inf);

public:
    KDataArray result() const;
    bool calculate(const KCalculationInfo& ci);
    void refresh();
    bool isSource() const;
    KDataGroupArray * userInputs();

    const KPortList & inputs() const;
    QString displayText() const;

protected:
    KPortList        _inpPorts;
    KDataArray       _dataList;
    KDataGroupArray _userInputs;

    KData modelData(const Quantity & qty) const;

    qreal calcDose(qreal c, qreal hd, qreal df) const;
    qreal calcResDose(qreal c, qreal rinh, qreal dfinh, qreal of, qreal dl) const;

};

/*
    bool calculate(const KCalculationInfo& ci);
    bool verify(int * err = 0, int * warn = 0);
    bool load(QIODevice * io);
    bool save(QIODevice * io);
protected:
    bool allocateIoPorts();
    IUserInput * createUserInputWidget(QWidget *parent);
*/

#endif // DOSEESTIMATION_H
