#include "estuarinetransport.h"
#include "quantity.h"
#include "radcore.h"

EstuarineTransport::EstuarineTransport(IModelFactory * fact, const KModelInfo& inf)
    : RiverTransport(fact, inf)
{
}

void EstuarineTransport::defineParameters()
{
    //allocate parameters for user inputs
    DataGroup dg1(QObject::tr("River conditions (site specific)"));
    dg1 << KData(&Srs19::RiverEstuaryWidth, 0)
        << KData(&Srs19::FlowDepth, 0)
        << KData(&Srs19::LowRiverFlowRate, 0)
        << KData(&Srs19::NetFreshwaterVelocity, 0)
        << KData(&Rad::CommentQuantity, QObject::tr("If U not known, value will be calculated with U=qr/BD."));
    _userInputs << dg1;

    DataGroup dg2(QObject::tr("Parameter estimation"));
    dg2 << KData(&Srs19::EstimateParameters, false)
        << KData(&Srs19::EstimatedRiverWidth, 0);
    _userInputs << dg2;

    DataGroup dg3(QObject::tr("Estuary conditions"));
    dg3 << KData(&Srs19::TidalPeriod, 45000)
        << KData(&Srs19::MaximumEbbVelocity, 0.5)
        << KData(&Srs19::MaximumFloodVelocity, 0.5);
    _userInputs << dg3;

    DataGroup dg4(QObject::tr("Receptor position"));
    dg4 << KData(&Srs19::ReceptorOnOpposite, false)
        << KData(&Srs19::ReceptorAtUpstream, false);
    _userInputs << dg4;
}

bool EstuarineTransport::calculate(const KCalculationInfo& ci, const KLocation& loc, KDataArray * calcResult)
{
    //estimate parameter
    estimateParameters();

   //user input parameters
    qreal x = loc.distance(ci);
    qreal B = _userInputs.numericValueOf(Srs19::RiverEstuaryWidth);
    qreal U = _userInputs.numericValueOf(Srs19::NetFreshwaterVelocity);
    qreal Ue = _userInputs.numericValueOf(Srs19::MaximumEbbVelocity);
    qreal Uf = _userInputs.numericValueOf(Srs19::MaximumFloodVelocity);
    qreal D = _userInputs.numericValueOf(Srs19::FlowDepth);
    qreal Tp = _userInputs.numericValueOf(Srs19::TidalPeriod);
    qreal Lz = 7 * D;
    qreal Lu = 0.32 * qAbs(Uf) * Tp;
    qreal Ut = 0.32 * (qAbs(Ue) + qAbs(Uf));
    qreal qw = D * B * Ut;

    //check the value of net freshwater velocity
    if (U <= 0.0) {
        qreal qr = _userInputs.numericValueOf(Srs19::LowRiverFlowRate);
        U = qr / (B * D);
        _userInputs.replace(KData(&Srs19::NetFreshwaterVelocity, U));
        xTrace() << *this << "Estimating net freshwater velocity: " << U;
    }

    //add downind distance
    (*calcResult) << KData(&Srs19::LongitudinalDistance, x);
    (*calcResult) << KData(&Srs19::CompleteMixingDistance, Lz);
    (*calcResult) << KData(&Srs19::UpstreamTravelDistance, Lu);
    (*calcResult) << KData(&Srs19::MeanTidalFlowSpeed, Ut);
    (*calcResult) << KData(&Srs19::TidalFlowRate, qw);

    bool isOpposite = _userInputs.valueOf(Srs19::ReceptorOnOpposite).toBool();
    bool isUpstream = _userInputs.valueOf(Srs19::ReceptorAtUpstream).toBool();

    if (isOpposite) {
        calcualteConcentration(x, qw, U, calcResult);
    }
    else if (x <= Lz) {
        //should be calculated at release modul
        //CO already calculated.
        //const KData& C0 = inpQi.find(Srs19::EffluentRadionuclideConcentration);
        KData C0 = _inpPorts.data(Srs19::EffluentRadionuclideConcentration);
        if (C0.isValid()) {
            (*calcResult) << KData(&Srs19::TotalConcentrationInWater, C0);
        }
        else {
            xWarning() << *this << QObject::tr("Concentration in water C0 is not provided by discharge source.");
        }
    }
    else if ((isUpstream && x <= Lu) || !isUpstream) {
        qreal M = (0.3 * D * Ut * Tp) / (B*B);
        qreal N = KMath::ratioN(M);
        qreal A = (1.5 * D * x * U) / (N * B * B * Ut);
        qreal pe = KMath::pe(A, N);

        (*calcResult) << KData(&Srs19::TidalToTimescaleRatio, M);
        (*calcResult) << KData(&Srs19::LongitudinalCoeffRatio, N);
        (*calcResult) << KData(&Srs19::PartialMixingIndex, A);
        (*calcResult) << KData(&Srs19::MixingCorrectionFactor, pe);

        calcualteConcentration(x, qw, U, calcResult, pe);

        //calculate UCF
        qreal UCF = qExp((3*x*D*U) / (N * B * B * Ut));
        (*calcResult) << KData(&Srs19::UpstreamToDownstreamFactor, UCF);
    }
    else {
        //total concentration in water is zero
        (*calcResult) << KData(&Srs19::TotalConcentrationInWater, 0);
    }

    return true;
}
bool EstuarineTransport::verify(int * oerr, int * owarn)
{
    int err = 0, warn = 0;
    doVerification(&err, &warn);

    //set the parameters
    qreal Tp = _userInputs.numericValueOf(Srs19::TidalPeriod);
    qreal Ue = _userInputs.numericValueOf(Srs19::MaximumEbbVelocity);
    qreal Uf = _userInputs.numericValueOf(Srs19::MaximumFloodVelocity);

    if (Tp <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::TidalPeriod);
        err++;
    }

    if (Ue <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::MaximumEbbVelocity);
        err++;
    }

    if (Uf <= 0) {
        KOutputProxy::errorNotSpecified(this, Srs19::MaximumFloodVelocity);
        err++;
    }

    KOutputProxy::infoVerificationResult(this, err, warn);
    if (oerr)
        *oerr = err;
    if (owarn)
        *owarn = warn;

    return err == 0;
}

bool EstuarineTransport::load(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}
bool EstuarineTransport::save(QIODevice * io)
{
    Q_UNUSED(io);
    return true;
}

