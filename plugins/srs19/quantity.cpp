#include <QObject>
#include <QtDebug>
#include "quantity.h"
#include "radcore.h"

namespace Srs19 {
    static ConstQuantityList __constQtyList;
    static QuantityList __qtyList;
    static const char * __qtyListName = "Quantity List";
    static const char * __qtyListDescription = "List of all available quantities";

    /*
     * For internal usage. Geenerate list of available quantities.
     */
    static void generateQuantityList()
    {
        //if already generated, do not add to list
        if (!__qtyList.isEmpty())
            return;

        __qtyList << &DischargePeriod
            << &Diameter
            << &NumOfServedPerson

            << &CrossSectionalArea
            << &LeeGaussianDiffusionFactor
            << &AirConcentrationIncrease
            << &ConcentrationInAir
            << &DailyDepositionRate
            << &SigmaFitParameterE
            << &SigmaFitParameterG
            << &ReductionFactor
            << &HeightGaussianDiffusionFactor
            << &ConstantValue1Metre
            << &ReleaseHeight
            << &BuildingHeight
            << &BuildingWidth
            << &FractionOfWind
            << &AtmosphericDischargeRate
            << &GeometricMeanOfWindSpeed
            << &AirFlowRate
            << &DryDepositionCoeff
            << &WetDepositionCoeff
            << &TotalDepositionCoeff
            << &DownwindDistance
            << &RadiactiveDecayConstant
            << &VerticalDiffusionParameter

            << &RiverEstuaryWidth
            << &EstimatedRiverWidth
            << &EffluentRadionuclideConcentration
            << &ConcentrationInBottomSediment
            << &ConcentrationInShoreSediment
            << &ConcentrationInSuspendedSediment
            << &OppositeBankMaxConcentration
            << &ConcentrationInDrySewage
            << &ConcentrationInFilteredWater
            << &TotalConcentrationInWater
            << &FlowDepth
            << &LiquidFlowRate
            << &SedimentDistributionCoeff
            << &MixingCorrectionFactor
            << &LowRiverFlowRate
            << &MeanRiverFlowRate
            << &WaterDischargeRate
            << &SewageDischargeRate
            << &AnnualSludgeProduction
            << &SuspendedSedimentConcentration
            << &EffShoreAccumulationTime
            << &TidalPeriod
            << &NetFreshwaterVelocity
            << &LongitudinalDistance
            << &ReleaseToBeachDistance

            << &BioaccumulationFactor
            << &ConcentrationFactor
            << &ConcentrationInAnimalFeed
            << &ConcentrationInAquaticFood
            << &ConcentrationInMeat
            << &ConcentrationInMilk
            << &ConcentrationInStoredAnimalFeed
            << &ConcentrationInDrySoil
            << &ConcentrationInVegetation
            << &ConcentrationInWater
            << &FractionIntakeAtSlaughter
            << &FractionIntakeAtMilking
            << &AnnualPastureFraction
            << &UptakeConcentrationFactor
            << &IrrigationRate
            << &MeatAnimalFeed
            << &MilkAnimalFeed
            << &WaterConsumption
            << &CropExposedPeriod
            << &IntervalAfterSlaughter
            << &IntervalAfterHarvest
            << &IntervalAfterMilking
            << &MassInterceptionFactor
            << &ReductionRateOnRoot
            << &ReductionRateOnVegetation
            << &ReductionOnSurface
            << &ReductionOnSoil
            << &SurfaceSoilDensity

            << &ConcentrationInSurfaceSoil
            << &ConcentrationInFoodstuff
            << &ConcentrationInSedimentSurface
            << &SurfaceSludgeConcentration
            << &SkinDoseCoeff
            << &GroundExposureDoseCoeff
            << &ImmersionDoseCoeff
            << &IngestionDoseCoeff
            << &InhalationDoseCoeff
            << &DustLoadingFactor
            << &GroundExposureDoseRate
            << &InhalationDoseRate
            << &ImmersionDoseRate
            << &SkinDoseRate
            << &IngestionDoseRate
            << &SedimentDoseRate
            << &ResuspensionDoseRate
            << &SewageDoseRate
            << &ConsumptionRate
            << &ExposedFractionTime
            << &InhalationRate
            << &Density

            << &OnSameBuilding
            << &CorrectedVerticalDiffusionParameter
            << &ReceptorOnOpposite
            << &EstimateParameters
            << &MaximumEbbVelocity
            << &MaximumFloodVelocity
            << &TidalFlowRate
            << &MeanTidalFlowSpeed
            << &ReceptorAtUpstream

            << &WaterDepth
            << &PartialMixingIndex
            << &CompleteMixingDistance
            << &UpstreamTravelDistance
            << &TidalToTimescaleRatio
            << &LongitudinalCoeffRatio
            << &ConcentrationInShoreline
            << &ConcentrationInWaterFish
            << &CoastalCurrent
            << &UpstreamToDownstreamFactor
            << &LateralDispersionCoeff
            << &DistanceFromShoreline
            << &UseOkuboDispersionExp
            << &LakeSurfaceArea
            << &LakeVolume
            << &LakeDepth

            << &CombinedReleaseRate
            << &LakeFlowVelocity
            << &IsSaltWater

            << &WaterDailyIrrigation
            << &AnnualIrrigationPeriod
            << &IrrigationDays
            << &AverageDepositionRate
            << &AnnualAverageDepositionRate
            << &AnnualIrrigationRate

            << &LambdaConstant

            << &ConsDueDirectContamination
            << &ConsDueIndirectProcess
            << &IsAnion
            << &DailyFeedingAmount
            << &ConsumptionDelay
            << &IsShortLiveNuclide
            << &IncrementValue
            << &SolidMaterialConcentration
            << &PersonSludgeProduction
            << &SewageSludgeDensity
            << &SludgeContainerDepth
            << &ConcentrationInWetSewage
            << &Concentration
            << &TotalDose
            << &AbsoluteAirHumidity
            << &AtmosphericTritiumConcentration
            << &WaterTritiumConcentration
            << &AtmosphericWaterIntakeFraction
            << &WaterIntakeFraction
            << &TritiumDoseFactor
            << &SpecificCarbonActivity
            << &DietaryCarbonFraction
            << &CarbonDoseFactor
            << &AirborneCarbonConcentration
            << &ScalarOutput
            << &CommentQuantity
            << &LongCommentQuantity
            << &NameQuantity
            << &UseDefaultValue
            << &UseDefaultValue2;

        //assign to const
        for (int k = 0; k < __qtyList.size(); k++)
            __constQtyList.append(__qtyList.at(k));
    }

    bool reloadQuantities(IModelFactory * factory)
    {
        //generate symbol list
        generateQuantityList();

        //load symbol from storage
        KStorageContent  content = factory->storage()->load(__qtyListName, factory);
        if (!content.isEmpty()) {
            QDataStream stream(content);
            int nsize = __qtyList.size();
            for(int k = 0; k < nsize && !stream.atEnd(); k++) {
                Quantity * qty = __qtyList.at(k);
                stream >> *qty;
            }
            return true;
        }
        return false;
    }

    bool saveQuantities(IModelFactory * factory)
    {
        KStorageContent content(QDateTime::currentDateTime());
        content.setFactory(factory);
        content.setName(__qtyListName);
        content.setDescription(__qtyListDescription);

        //append content
        QDataStream stream(&content, QIODevice::WriteOnly);
        for (int k = 0; k < __qtyList.size(); k++)
            stream << *__qtyList.at(k);

        return factory->storage()->save(content);
    }

    ConstQuantityList availableQuantities()
    {
        generateQuantityList();
        return __constQtyList;
    }

    QuantityList editableQuantities()
    {
        generateQuantityList();
        return __qtyList;
    }

//symbol definition
//ID, type, digit, min, max, default, symbol, symbol(rich-text), label, unit, unit(rich-text), detail

Quantity DischargePeriod =
    {1, Rad::Real, 1, 0, MAX_REAL, 30, "tb", "t<sub>b</sub>", QObject::tr("Discharge period"), "year(s)", "year(s)", QObject::tr("Duration of discharge")};
Quantity NumOfServedPerson =
    {2, Rad::Integer, DIGIT_NONE, 0, MAX_INT, 0, "Np", "N<sub>p</sub>", QObject::tr("Served person"), "", "", QObject::tr("Number of served person in the facility")};
Quantity Diameter =
    {3, Rad::Real, 2, 0, MAX_REAL, 0, "d", "d", QObject::tr("Diameter"), "m", "m", QObject::tr("Stack diameter")};


Quantity CrossSectionalArea =
    {4, Rad::Real, 2, 0, MAX_REAL, 0, "Ab", "A<sub>b</sub>", QObject::tr("Cross-sectional area"), "m2", "m<sup>2</sup>", QObject::tr("Projected Cross-sectional area of the building most influencing the flow of plume")};
Quantity LeeGaussianDiffusionFactor =
    {5, Rad::NumText, 2, 0, 100, 0, "B", "B", QObject::tr("Gaussian diffusion factor"), "m-2", "m<sup>-2</sup>", QObject::tr("Gaussian diffusion factor corrected for dispersion in the lee fo a building inside the wake zone (use instead of F)")};
Quantity AirConcentrationIncrease =
    {6, Rad::Real, 2, 0, MAX_REAL, 30, "B0", "B_0", QObject::tr("Air concentration increase"), "", "", QObject::tr("Unitless constant that accounts for the potential increase in air concentration along a vertical wall")};
Quantity ConcentrationInAir =
    {7, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ca", "C<sub>A</sub>", QObject::tr("Air concentration"), "Bq/m3", "Bq/m<sup>3</sup>", QObject::tr("Annual average concentration of radionuclide in air (at ground level)")};
Quantity DailyDepositionRate =
    {8, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "di", "d<sub>i</sub>", QObject::tr("Daily deposition rate"), "Bq.m-2.d-1", "Bq.m<sup>-2</sup>.d<sup>-1</sup>", QObject::tr("Daily average deposition rate to the ground surface of radionuclide i from both dry and wet processes")};
Quantity SigmaFitParameterE =
    {9, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "E", "E", QObject::tr("Sigma fit parameter E"), "", "", QObject::tr("Fit parameters for the equation that defines sigma-z")};
Quantity SigmaFitParameterG =
    {10, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "G", "G", QObject::tr("Sigma fit parameter G"), "", "", QObject::tr("Fit parameters for the equation that defines sigma-z")};
Quantity ReductionFactor =
    {11, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "f", "f", QObject::tr("Reduction factor"), "", "", QObject::tr("Unitless reduction factor accounting for radioactive decay during atmospheric transport")};
Quantity HeightGaussianDiffusionFactor =
    {12, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "F", "F", QObject::tr("Height Gaussian diffusion factor"), "m-2", "m<sup>-2</sup>", QObject::tr("Gaussian diffusion factor appropriate for the height of release H and downwind distance x")};
Quantity ConstantValue1Metre =
    {13, Rad::Real, 2, 0, MAX_REAL, 1, "K", "K", QObject::tr("Constant 1 m"), "m", "m", QObject::tr("Constant with a value of 1m")};
Quantity ReleaseHeight =
    {14, Rad::Real, 2, 0, MAX_REAL, 0, "H", "H", QObject::tr("Release height"), "m", "m", QObject::tr("Height at which the radionuclide is released")};
Quantity BuildingHeight =
    {15, Rad::Real, 2, 0, MAX_REAL, 0, "Hb", "H<sub>B</sub>", QObject::tr("Building height"), "m", "m", QObject::tr("Height of the building that dominates the air flow near the radionuclide release point")};
Quantity BuildingWidth =
    {16, Rad::Real, 2, 0, MAX_REAL, 0, "Wb", "W<sub>B</sub>", QObject::tr("Building width"), "m", "m", QObject::tr("Width of the building that dominates the air flow near the radionuclide release point")};
Quantity FractionOfWind =
    {17, Rad::Real, 4, 0, 10, 0.25, "Pp", "P<sub>p</sub>", QObject::tr("Fraction of wind"), "", "", QObject::tr("Fraction of time during the year that the wind blows towards the receptor of intereset in sector p")};
Quantity AtmosphericDischargeRate =
    {18, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qi", "Q<sub>i</sub>", QObject::tr("Atmospheric discharge"), "Bq/s", "Bq/s", QObject::tr("Annual average rate of radionuclide emission to the atmosphere")};
Quantity GeometricMeanOfWindSpeed =
    {19, Rad::Real, 4, 0, MAX_REAL, 0, "ua", "u<sub>a</sub>", QObject::tr("Geometric mean of wind speed"), "m/s", "m/s", QObject::tr("Annual geometric mean of the wind speed at the height of the release")};
Quantity AirFlowRate =
    {20, Rad::Real, 4, 0, MAX_REAL, 0, "V", "V", QObject::tr("Air flow rate"), "m3/s", "m<sup>3</sup>/s", QObject::tr("Volumetric flow rate of vent or stack athe point of release")};
Quantity DryDepositionCoeff =
    {21, Rad::Real, 2, 0, MAX_REAL, 500, "Vd", "V<sub>d</sub>", QObject::tr("Dry deposition coefficient"), "m/d", "m/d", QObject::tr("Dry deposition coefficient (deposition on the soil and vegetation)")};
Quantity WetDepositionCoeff =
    {22, Rad::Real, 2, 0, MAX_REAL, 500, "Vw", "V<sub>w</sub>", QObject::tr("Wet deposition coefficient"), "m/d", "m/d", QObject::tr("Wet deposition coefficient")};
Quantity TotalDepositionCoeff =
    {23, Rad::Real, 2, 0, MAX_REAL, 1000, "Vt", "V<sub>t</sub>", QObject::tr("Total deposition coefficient"), "m/d", "m/d", QObject::tr("Total deposition coefficient")};
Quantity DownwindDistance =
    {24, Rad::Real, 2, 0, MAX_REAL, 0, "x", "x", QObject::tr("Receptor distance"), "m", "m", QObject::tr("Location of the receptor (downwind distance)")};
Quantity RadiactiveDecayConstant =
    {25, Rad::NumText, 4, 0, MAX_REAL, 0, "li", "l<sub>i</sub>", QObject::tr("Decay constant"), "s-1", "s<sup>-1</sup>", QObject::tr("Radioactive decay constant for radionuclide i")};
Quantity VerticalDiffusionParameter =
    {26, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "sz", "s<sub>z</sub>", QObject::tr("Vertical diffusion parameter"), "m", "m", QObject::tr("Vertical diffusion parameter")};

//---
Quantity RiverEstuaryWidth =
    {27, Rad::Real, 2, 0, MAX_REAL, 0, "B", "B", QObject::tr("River/estuary width"), "m", "m", QObject::tr("River or estuary width")};
Quantity EstimatedRiverWidth =
    {28, Rad::Real, 2, 0, MAX_REAL, 0, "Bd", "Bd", QObject::tr("Estimated width"), "m", "m", QObject::tr("River width under a mean of annual river flow rate upstream of the tidal flow area")};
Quantity EffluentRadionuclideConcentration =
    {29, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "C0", "C<sub>0</sub>", QObject::tr("Concentration in effluent"), "Bq/m3", "Bq/m<sup>3</sup>", QObject::tr("Radionuclide concentration in water at the effluent discharge outfall")};
Quantity ConcentrationInBottomSediment =
    {30, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs,b", "C<sub>s,b</sub>", QObject::tr("Concentration in bottom sediment"), "Bq/kg", "Bq/kg", QObject::tr("Radionuclide concentration in bottom sediment")};
Quantity ConcentrationInShoreSediment =
    {31, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs,s", "C<sub>s,s</sub>", QObject::tr("Concentration in shore sediment"), "Bq/m2", "Bq/m<sup>2</sup>", QObject::tr("Radionuclide concentration in shore or beach sediment")};
Quantity ConcentrationInSuspendedSediment =
    {32, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs,w", "C<sub>s,w</sub>", QObject::tr("Concentration in suspended sediment"), "Bq/kg", "Bq/kg", QObject::tr("Radionuclide concentration in suspended sediment")};
Quantity OppositeBankMaxConcentration =
    {33, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ct", "C<sub>t</sub>", QObject::tr("Opposite bank max concentration"), "Bq/m3", "Bq/m<sup>3</sup>", QObject::tr("Maximum radionuclide concentration on the river bank opposite to the discharge point")};
Quantity ConcentrationInDrySewage =
    {34, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cdsl", "C<sub>sluldge</sub>", QObject::tr("Concentration in sludge (dry weight)"), "Bq/kg", "Bq/kg", QObject::tr("Annual average radionuclide concentration in sewage sludge")};
Quantity ConcentrationInFilteredWater =
    {35, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cw,s", "C<sub>w,s</sub>", QObject::tr("Concentration in filtered water"), "Bq/m3", "Bq/m<sup>3</sup>", QObject::tr("Radionuclide concentration in filtered water")};
Quantity TotalConcentrationInWater =
    {36, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cw,tot", "C<sub>w,tot</sub>", QObject::tr("Total concentration in water"), "Bq/m3", "Bq/m<sup>3</sup>", QObject::tr("Total radionuclide concentration in water")};
Quantity FlowDepth =
    {37, Rad::Real, 2, 0, MAX_REAL, 0, "D", "D", QObject::tr("Flow depth"), "m", "m", QObject::tr("Flow depth that correspondence to qr")};
Quantity LiquidFlowRate =
    {38, Rad::Real, 4, 0, MAX_REAL, 0, "F", "F", QObject::tr("Liquid flow rate"), "m3/s", "m<sup>3</sup>/s", QObject::tr("Flow rate of the liquid effluent")};
Quantity SedimentDistributionCoeff =
    {39, Rad::NumText, 0, 0, MAX_REAL, 0, "Kd", "K<sub>d</sub>", QObject::tr("Sediment distribution coefficient"), "L/kg", "L/kg", QObject::tr("Distribution coefficient between the radionuclide on sediment and that dissolved in water")};
Quantity MixingCorrectionFactor =
    {40, Rad::NumText, DIGIT_DEF, 0, MAX_REAL, 0, "P", "P", QObject::tr("Mixing correction factor"), "", "", QObject::tr("Correction factor for partial mixing")};
Quantity LowRiverFlowRate =
    {41, Rad::Real, 2, 0, MAX_REAL, 0, "qr", "q<sub>r</sub>", QObject::tr("Low river flow rate"), "m3/s", "m<sup>3</sup>/s", QObject::tr("30 year low annual river flow rate")};
Quantity MeanRiverFlowRate =
    {42, Rad::Real, 2, 0, MAX_REAL, 0, "qr", "q<sub>r</sub>", QObject::tr("Mean river flow rate"), "m3/s", "m<sup>3</sup>/s", QObject::tr("Mean annual river flow rate")};
Quantity WaterDischargeRate =
    {43, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qi", "Q<sub>i</sub>", QObject::tr("Surface water discharge"), "Bq/s", "Bq/s", QObject::tr("Average radionuclide discharge rate into river, estuary, lake or coastal water")};
Quantity SewageDischargeRate =
    {44, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qsl", "Q<sub>sludge</sub>", QObject::tr("Sewage discharge"), "Bq/a", "Bq/a", QObject::tr("Annual discharge of a radionuclide to a sewage plant")};
Quantity AnnualSludgeProduction =
    {45, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ssl", "S<sub>sludge</sub>", QObject::tr("Sludge production"), "kg/a", "kg/a", QObject::tr("Anual sewage sludge production at the relevant sewage treatment works")};
Quantity SuspendedSedimentConcentration =
    {46, Rad::Real, 8, 0, MAX_REAL, 0, "Ss", "S<sub>s</sub>", QObject::tr("Suspended sediment concentration"), "kg/m3", "kg/m<sup>3</sup>", QObject::tr("Suspended sediment concentration")};
Quantity EffShoreAccumulationTime =
    {47, Rad::NumText, 0, 0, MAX_REAL, 3.15e+7, "Te", "T<sub>e</sub>", QObject::tr("Effective accumulation time"), "s", "s", QObject::tr("Effective accumulation time for shore or beach sediment")};
Quantity TidalPeriod =
    {48, Rad::Real, 2, 0, MAX_REAL, 0, "Tp", "T<sub>p</sub>", QObject::tr("Tidal period"), "s", "s", QObject::tr("Tidal period")};
Quantity NetFreshwaterVelocity =
    {49, Rad::Real, 2, 0, MAX_REAL, 0, "U", "U", QObject::tr("Net freshwater velocity"), "m/s", "m/s", QObject::tr("Net freshwater velocity")};
Quantity LongitudinalDistance =
    {50, Rad::Real, 2, 0, MAX_REAL, 0, "x", "x", QObject::tr("Longitudinal distance"), "m", "m", QObject::tr("Longitudinal distance from the release point to a potential receptor locatoin")};
Quantity ReleaseToBeachDistance =
    {51, Rad::Real, 2, 0, MAX_REAL, 0, "y0", "y<sub>0</sub>", QObject::tr("Release to beach distance"), "m", "m", QObject::tr("Distance between the release point and the beach")};

Quantity BioaccumulationFactor =
    {52, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Bp", "B<sub>p</sub>", QObject::tr("Bioaccumulation factor"), "L/kg", "L/kg", QObject::tr("Equilibrium ratio of the concentration of a radionuclide in aquatic food p to its dissolved concentration in water")};
Quantity ConcentrationFactor =
    {53, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Bv", "B<sub>v</sub>", QObject::tr("Uptake concentration factor"), "Bq/kg", "Bq/kg", QObject::tr("Concentration factor for the uptake of radionuclide from soil by edible parts of crops; adhesion of soil to the vegetation is not taken into account. For pasture grass the unit of mass is dry matter; for vegetables consumed by humans it is fresh weight")};
Quantity ConcentrationInAnimalFeed =
    {54, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ca,i", "C<sub>a,i</sub>", QObject::tr("Concentration in animal feed"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in animal feed")};
Quantity ConcentrationInAquaticFood =
    {55, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Caf,i", "C<sub>af,i</sub>", QObject::tr("Concentration in aquatic food"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in aquatic foods")};
Quantity ConcentrationInMeat =
    {56, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cf,i", "C<sub>f,i</sub>", QObject::tr("Concentration in meat"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in meat")};
Quantity ConcentrationInMilk =
    {57, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cm,i", "C<sub>m,i</sub>", QObject::tr("Concentration in milk"), "Bq/L", "Bq/L", QObject::tr("Annual average concentration of radionuclide in milk")};
Quantity ConcentrationInStoredAnimalFeed =
    {58, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cp,i", "C<sub>p,i</sub>", QObject::tr("Concentration in stored animal feed"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in stored animal feed")};
Quantity ConcentrationInDrySoil =
    {59, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs", "C<sub>s</sub>", QObject::tr("Concentration in dry soil"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in dry soil")};
Quantity ConcentrationInVegetation =
    {60, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cv,i", "C<sub>v,i</sub>", QObject::tr("Concentration in vegetation"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in food crops (express in fresh matter for vegetation consumed by humans and in dry matter for vegetation consumed by animals)")};
Quantity ConcentrationInWater =
    {61, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cw,i", "C<sub>w,i</sub>", QObject::tr("Concentration in water"), "Bq/L", "Bq/L", QObject::tr("Annual average concentration in water")};
Quantity FractionIntakeAtSlaughter =
    {62, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ff", "F<sub>f</sub>", QObject::tr("Daily intake at slaughter"), "d/kg", "d/kg", QObject::tr("Fraction of the animal daily intake that appears in a kilogram of flesh at the time of slaughter")};
Quantity FractionIntakeAtMilking =
    {63, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Fm", "F<sub>m</sub>", QObject::tr("Daily intake at milking"), "d/L", "d/L", QObject::tr("Fraction of animal daily intake that appears in a litre of milke at the time of milking")};
Quantity AnnualPastureFraction =
    {64, Rad::Real, 4, 0, 1, 0, "fp", "f<sub>p</sub>", QObject::tr("Annual pasture fraction"), "", "", QObject::tr("Fraction of time during a year that an animal consumes fresh pasture")};
Quantity UptakeConcentrationFactor =
    {65, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Fv", "F<sub>v</sub>", QObject::tr("Uptake concentration factor"), "Bq/kg", "Bq/kg", QObject::tr("Concentration factor for the uptake of radionuclide from soil by edible parts of crops, adjusted implicitly to account for adhesion of soil to the vegetation")};
Quantity IrrigationRate =
    {66, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Iw", "I<sub>w</sub>", QObject::tr("Irrigation rate"), "m3.m-2.d-1", "m<sup>3</sup>.m<sup>-2</sup>.d<sup>-1</sup>", QObject::tr("Average irrigation rate")};
Quantity MeatAnimalFeed =
    {67, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qf", "Q<sub>f</sub>", QObject::tr("Meat animal feed"), "kg/d", "kg/d", QObject::tr("Amount of feed consumed per dy by a meat producing animal")};
Quantity MilkAnimalFeed =
    {68, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qm", "Q<sub>m</sub>", QObject::tr("Milk animal feed"), "kg/d", "kg/d", QObject::tr("Amount of feed consumed per da by a milk producing animal")};
Quantity WaterConsumption =
    {69, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qw", "Q<sub>w</sub>", QObject::tr("Water consumption"), "m3/d", "m<sup>3</sup>/d", QObject::tr("Amount of water consumed per day by an animal")};
Quantity CropExposedPeriod =
    {70, Rad::Real, 2, 0, MAX_REAL, 0, "te", "t<sub>e</sub>", QObject::tr("Exposed period"), "d", "d", QObject::tr("Time period that crops are exposed to contamination during the growing season")};
Quantity IntervalAfterSlaughter =
    {71, Rad::Real, 2, 0, MAX_REAL, 0, "tf", "f<sub>f</sub>", QObject::tr("Period after slaughter"), "d", "d", QObject::tr("Time interval between slaughter and consumption of the meat")};
Quantity IntervalAfterHarvest =
    {72, Rad::Real, 2, 0, MAX_REAL, 0, "th", "t<sub>h</sub>", QObject::tr("Hold up time"), "d", "d", QObject::tr("Delay (hold up) time that represents the time interval between harvest and consumption of the food")};
Quantity IntervalAfterMilking =
    {73, Rad::Real, 2, 0, MAX_REAL, 0, "tm", "t<sub>m</sub>", QObject::tr("Period after milking"), "d", "d", QObject::tr("Time interval between milking and consumption of the milk")};
Quantity MassInterceptionFactor =
    {74, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "a", "a", QObject::tr("Mass interception factor"), "m2/kg", "m<sup>2</sup>/kg", QObject::tr("Mass interception fraction for a given food crop")};
Quantity ReductionRateOnRoot =
    {75, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "lEis", "l<sub>E<sup>i</sup>s</sub>", QObject::tr("Reduction rate on root"), "d-1", "d<sup>-1</sup>", QObject::tr("Effective rate constant responsible for reduction of radionuclide concentration in the root zone")};
Quantity ReductionRateOnVegetation =
    {76, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "lEiv", "l<sub>E<sup>i</sup>v</sub>", QObject::tr("Reduction rate on vegetation"), "d-1", "d<sup>-1</sup>", QObject::tr("Effective rate constant responsible for reduction fo radionuclide concentration on vegetation")};
Quantity ReductionOnSurface =
    {77, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "lw", "l<sub>w</sub>", QObject::tr("Reduction rate on plant surface"), "d-1", "d<sup>-1</sup>", QObject::tr("Effective rate constant responsible for reduction of the concentration of material deposited on plant surfaces due to process other than radioactive decay")};
Quantity ReductionOnSoil =
    {78, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "ls", "l<sub>s</sub>", QObject::tr("Reduction rate on soil"), "d-1", "d<sup>-1</sup>", QObject::tr("Effective rate constant responsible for reduction of the concentration of material deposited on the root zone of soils due to processes other than radioactive decay")};
Quantity SurfaceSoilDensity =
    {79, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "p", "p", QObject::tr("Surface soil density"), "kg/m2", "kg/m<sup>2</sup>", QObject::tr("Effective surface soil density for the root zone")};

Quantity ConcentrationInSurfaceSoil =
    {80, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cgr", "C<sub>gr</sub>", QObject::tr("Ground surface concentration"), "Bq/m2", "Bq/m<sup>2</sup>", QObject::tr("Ground surface concentration resulting from a 30 year discharge to the atmosphere")};
Quantity ConcentrationInFoodstuff =
    {81, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cp,i", "C<sub>p,i</sub>", QObject::tr("Concentration in foodstuff"), "Bq/kg", "Bq/kg", QObject::tr("Concentration of radionuclide i in foodstuff p at the time of consumption")};
Quantity ConcentrationInSedimentSurface =
    {82, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs,s", "C<sub>s,s</sub>", QObject::tr("Sediment surface concentration"), "Bq/m2", "Bq/m<sup>2</sup>", QObject::tr("Annual average sediment surface concentration")};
Quantity SurfaceSludgeConcentration =
    {83, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "C'sl", "C'<sub>sludge</sub>", QObject::tr("Surface concentration in sludge"), "Bq/m2", "Bq/m<sup>2</sup>", QObject::tr("Surface concentration deposited in sewage sludge")};
Quantity SkinDoseCoeff =
    {84, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFs", "DF_s", QObject::tr("Skin dose coefficient"), "Sv/a per Bq/m3", "Sv/a per Bq/m3m", QObject::tr("Dose coefficient for skin from air immersion")};
Quantity GroundExposureDoseCoeff =
    {85, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFgr", "DF<sub>gr</sub>", QObject::tr("Ground exposure dose coefficient"), "Sv/a per Bq/m2", "Sv/a per Bq/m<sup>2</sup>", QObject::tr("Dose coefficient for exposure to contaminated ground or sediment surfaces")};
Quantity ImmersionDoseCoeff =
    {86, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFim", "DF<sub>im</sub>", QObject::tr("Immersion dose coefficient"), "Sv/a per Bq/m3", "Sv/a per Bq/m<sup>3</sup>", QObject::tr("Immersion dose coefficient")};
Quantity IngestionDoseCoeff =
    {87, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFing", "DF<sub>ing</sub>", QObject::tr("Ingestion dose coefficient"), "Sv/Bq", "Sv/Bq", QObject::tr("Dose coefficient for ingestion of radionuclides")};
Quantity InhalationDoseCoeff =
    {88, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFinh", "DF<sub>inh</sub>", QObject::tr("Inhalation dose coefficient"), "Sv/Bq", "Sv/Bq", QObject::tr("Dose coefficient for inhalation of radionuclides")};
Quantity DustLoadingFactor =
    {89, Rad::NumText, DIGIT_DEF, 0, MAX_REAL, 0, "DL", "D<sub>L</sub>", QObject::tr("Dust loading factor"), "kg/m3", "kg/m<sup>3</sup>", QObject::tr("Dust loading factor")};
Quantity GroundExposureDoseRate=
    {90, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Egr", "E<sub>gr</sub>", QObject::tr("Ground exposure dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate from ground deposits after a 30 year discharge to the atmosphere")};
Quantity InhalationDoseRate =
    {91, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Einh", "E<sub>inh</sub>", QObject::tr("Inhalation dose rate"), "Sv/a", "Sv/a", QObject::tr("Annual inhalation dose from concentration in air")};
Quantity ImmersionDoseRate =
    {92, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Eim", "E<sub>im</sub>", QObject::tr("Immersion dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate from immersion in a discharge cloud")};
Quantity SkinDoseRate =
    {93, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Eim,s", "E<sub>im,s</sub>", QObject::tr("Skin immersion dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate to skin from immersion in a discharge cloud")};
Quantity IngestionDoseRate =
    {94, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Eing,p", "E<sub>ing,p</sub>", QObject::tr("Ingestion dose rate"), "Sv/a", "Sv/a", QObject::tr("Annual dose from ingestion of foodstuff p")};
Quantity SedimentDoseRate =
    {95, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Em", "E<sub>m</sub>", QObject::tr("Sediment exposure dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate from exposure to contaminated sediment")};
Quantity ResuspensionDoseRate =
    {96, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Eres", "E<sub>res</sub>", QObject::tr("Inhalation of resuspended sediment"), "Sv/a", "Sv/a", QObject::tr("Annual inhalation dose from resuspension of radionuclides deposited in sewage sludge")};
Quantity SewageDoseRate =
    {97, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Es", "E<sub>s</sub>", QObject::tr("Sewage sludge dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate from concentration in sewage sludge")};
Quantity ConsumptionRate =
    {98, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Hp", "H<sub>p</sub>", QObject::tr("Consumption rate"), "kg/a", "kg/a", QObject::tr("Average consumption rate for foodstuff p")};
Quantity ExposedFractionTime =
    {99, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Of", "O<sub>f</sub>", QObject::tr("Exposed time fraction"), "", "", QObject::tr("Fractoin of time in a year when an individual is exposed to a particular exposure pathway")};
Quantity InhalationRate =
    {100, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Rinh", "R<sub>inh</sub>", QObject::tr("Inhalation rate"), "m3/a", "m<sup>3</sup>/a", QObject::tr("Annual average inhalation rate")};
Quantity Density =
    {101, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "p", "p", QObject::tr("Density"), "kg/m3", "kg/m<sup>3</sup>", QObject::tr("Density")};

Quantity OnSameBuilding =
    {102, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "sb", "sb", QObject::tr("Source and receptor on same building"), "", "", QObject::tr("Source and receptor on same building surface (dispersion in the lee of a building inside the cavity zone).")};
Quantity CorrectedVerticalDiffusionParameter  =
    {103, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Sz", "S<sub>z</sub>", QObject::tr("Corrected vertical diffusion parameter"), "m", "m", QObject::tr("Corrected vertical diffusion parameter")};
Quantity ReceptorOnOpposite =
    {104, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "ro", "ro", QObject::tr("Receptor located at the opposite to discharge point?"), "", "", QObject::tr("Receptor x on the same bank side or in opposite side of the discharge point.")};
Quantity EstimateParameters =
    {105, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "ep", "ep", QObject::tr("Estimate parameter(s)"), "", "", QObject::tr("Estimate needed parameters.")};

Quantity MaximumEbbVelocity =
    {106, Rad::Real, 2, 0, MAX_REAL, 0, "Ue", "U<sub>e</sub>", QObject::tr("Maximum ebb velocity"), "m/s", "m/s", QObject::tr("Maximum ebb (seaward) velocity")};
Quantity MaximumFloodVelocity =
    {107, Rad::Real, 2, 0, MAX_REAL, 0, "Uf", "U<sub>f</sub>", QObject::tr("Maximum flood velocity"), "m/s", "m/s", QObject::tr("Maximum flood (upstream) velocity")};
Quantity TidalFlowRate =
    {108, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "qw", "q<sub>w</sub>", QObject::tr("Tidal flow rate"), "m3/s", "m<sup>3</sup>/s", QObject::tr("Tidal flow rate")};
Quantity MeanTidalFlowSpeed =
    {109, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ut", "U<sub>t</sub>", QObject::tr("Mean tidal flow speed"), "m/s", "m/s", QObject::tr("Mean tidal flow speed")};
Quantity ReceptorAtUpstream =
    {110, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "ru", "ru", QObject::tr("Receptor at estuary upstream location?"), "", "", QObject::tr("Receptor x located at either upstream or downstream.")};

Quantity WaterDepth =
    {111, Rad::Real, 2, 0, MAX_REAL, 0, "D", "D", QObject::tr("Water depth"), "m", "m", QObject::tr("Water depth at the radionuclide discharge effluent outfall")};
Quantity PartialMixingIndex =
    {112, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "A", "A", QObject::tr("Partial mixing index"), "", "", QObject::tr("Partial mixing index)")};
Quantity CompleteMixingDistance =
    {113, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Lz", "L<sub>z</sub>", QObject::tr("Complete mixing distance"), "m", "m", QObject::tr("Complete vertical mixing distance")};
Quantity UpstreamTravelDistance =
    {114, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Lu", "L<sub>u</sub>", QObject::tr("Upstream travel distance"), "m", "m", QObject::tr("The radionuclide upstream travel distance")};
Quantity TidalToTimescaleRatio =
    {115, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "M", "M", QObject::tr("Tidal period to timescale ratio"), "", "", QObject::tr("The ratio of the tidal period to the timescale fo cross-sectional mixing")};
Quantity LongitudinalCoeffRatio =
    {116, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "N", "N", QObject::tr("Longitudinal dispersion coefficient ratio"), "", "", QObject::tr("The ratio of the longitudinal disperstion coefficient in the estuary to that in a river")};

Quantity ConcentrationInShoreline =
    {117, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cws", "C<sub>ws</sub>", QObject::tr("Concentration in shoreline"), "Bq/m3", "Bq/m<sup>3</sup>", QObject::tr("Total radionuclide concentration for the shoreline (for use as the basis for activity concentraions in shoreline sediments for calculation of external doses)")};
Quantity ConcentrationInWaterFish =
    {118, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cwf", "C<sub>wf</sub>", QObject::tr("Concentration for fishing"), "Bq/m3", "Bq/m<sup>3</sup>", QObject::tr("Total radionuclide concentration in water (for use as the basis for activity concentrations in fish, etc.)")};
Quantity CoastalCurrent =
    {119, Rad::Real, 2, 0, MAX_REAL, 0, "U", "U", QObject::tr("Coastal current"), "m/s", "m/s", QObject::tr("Coastal current")};

Quantity UpstreamToDownstreamFactor =
    {120, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "UCF", "UCF", QObject::tr("Upstream concentration factor"), "", "", QObject::tr("The ratio of upstream to downstream concentration")};
Quantity LateralDispersionCoeff =
    {121, Rad::NumText, DIGIT_DEF, 0, MAX_REAL, 0, "Ey", "E<sub>y</sub>", QObject::tr("Lateral dispersion coefficient"), "m2/s", "m<sup>2</sup>/s", QObject::tr("The lateral dispersion coefficient")};
Quantity DistanceFromShoreline =
    {122, Rad::Real, 2, 0, MAX_REAL, 0, "y", "y", QObject::tr("Distance from shoreline"), "m", "m", QObject::tr("Distance from shoreline")};
Quantity UseOkuboDispersionExp =
    {123, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "Ey", "E<sub>y</sub>", QObject::tr("Use Okubo's dispersion expression for Ey"), "", "", QObject::tr("Use Okubo's dispersion expression to calculate lateral dispersion coefficient Ey")};

Quantity LakeSurfaceArea =
    {124, Rad::Real, 2, 0, MAX_REAL, 0, "Al", "A<sub>l</sub>", QObject::tr("Lake surface area"), "m2", "m<sup>2</sup>", QObject::tr("Lake surface area")};
Quantity LakeVolume =
    {125, Rad::Real, 2, 0, MAX_REAL, 0, "V", "V", QObject::tr("Lake volume"), "m3", "m<sup>3</sup>", QObject::tr("Lake volume")};
Quantity LakeDepth =
    {126, Rad::Real, 2, 0, MAX_REAL, 0, "D", "D", QObject::tr("Lake depth"), "m", "m", QObject::tr("Average lake depth")};

Quantity CombinedReleaseRate =
    {127, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qi'", "Q<sub>i</sub>'", QObject::tr("Combined release rate"), "Bq/s", "Bq/s", QObject::tr("Combined radionuclide release rate Qi'")};
Quantity LakeFlowVelocity =
    {128, Rad::Real, 2, 0, MAX_REAL, 0, "U", "U", QObject::tr("Lake flow velocity"), "m/s", "m/s", QObject::tr("Lake flow velocity")};
Quantity IsSaltWater =
    {129, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "sw", "sw", QObject::tr("Salt water"), "", "", QObject::tr("Salt water flag")};

Quantity WaterDailyIrrigation =
    {130, Rad::Real, 2, 0, MAX_REAL, 0, "Wd", "W<sub>d</sub>", QObject::tr("Daily irrigation"), "L/m^2", "L/m<sup>2</sup>", QObject::tr("Amount of water usage for daily irrigation")};
Quantity AnnualIrrigationPeriod =
    {131, Rad::Integer, DIGIT_DEF, 0, MAX_INT, 0, "Td", "T<sub>d</sub>", QObject::tr("Annual irrigation period"), "d", "d", QObject::tr("Total number of irrigation periods for 1 year")};
Quantity IrrigationDays =
    {132, Rad::Integer, DIGIT_DEF, 0, MAX_INT, 0, "Nd", "N<sub>d</sub>", QObject::tr("Total irrigation days"), "d", "d", QObject::tr("Total number of irrigation days during irrigation period")};

Quantity AverageDepositionRate =
    {133, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "di", "d<sub>i</sub>", QObject::tr("Average deposition rate"), "Bq.m-2.d-1", "Bq.m<sup>-2</sup>.d<sup>-1</sup>", QObject::tr("Average deposition rate over irrigation period to the ground surface of radionuclide i from both dry and wet processes")};
Quantity AnnualAverageDepositionRate =
    {134, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "di", "d<sub>i</sub>", QObject::tr("Annual average deposition rate"), "Bq.m-2.d-1", "Bq.m<sup>-2</sup>.d<sup>-1</sup>", QObject::tr("Annual average deposition rate to the ground surface of radionuclide i from both dry and wet processes")};
Quantity AnnualIrrigationRate =
    {135, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Iwa", "I<sub>wa</sub>", QObject::tr("Annual irrigation rate"), "m3.m-2.d-1", "m<sup>3</sup>.m<sup>-2</sup>.d<sup>-1</sup>", QObject::tr("Annual average irrigation rate")};

Quantity LambdaConstant =
    {136, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Lc", "L<sub>c</sub>", QObject::tr("Lambda constant"), "s-1", "s<sup>-1</sup>", QObject::tr("Lambda constant")};

Quantity ConsDueDirectContamination =
    {137, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cv,i,1", "C<sub>v,i,1</sub>", QObject::tr("Concentration due to direct contamination"), "Bq/kg", "Bq/kg", QObject::tr("Radionuclide concentration due to direct contamination of nuclide in vegetation.")};

Quantity ConsDueIndirectProcess =
    {138, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cv,i,2", "C<sub>v,i,2</sub>", QObject::tr("Concentration due to indirect process"), "Bq/kg", "Bq/kg", QObject::tr("Concentration in vegetation arising from indirect process.")};

Quantity IsAnion =
    {139, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "an", "an", QObject::tr("Is anion?"), "", "", QObject::tr("Anionic radionuclide or not.")};

Quantity ConsumptionDelay =
    {140, Rad::Real, 2, 0, MAX_REAL, 0, "tfm", "f<sub>fm</sub>", QObject::tr("Period after milking/slaughter"), "d", "d", QObject::tr("Time interval between slaughter/milking and consumption of the meat")};

Quantity DailyFeedingAmount =
    {141, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qd", "Q<sub>d</sub>", QObject::tr("Amount of daily feeding"), "kg/d", "kg/d", QObject::tr("Amount of feed consumed per da by a milk/meat producing animal")};
Quantity IsShortLiveNuclide =
    {142, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "sl", "sl", QObject::tr("Is short lived radionuclide?"), "", "", QObject::tr("Flag for shortlived radionuclide.")};

Quantity IncrementValue =
    {143, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Inc", "Inc", QObject::tr("Increment value"), "", "", QObject::tr("Increment value relative from base value.")};

Quantity SolidMaterialConcentration =
    {144, Rad::Real, 2, 0, 100, 0, "Ps", "P<sub>s</sub>", QObject::tr("Solid material concentration"), "%", "%", QObject::tr("Solid material concentration in sewage sludge.")};

Quantity PersonSludgeProduction =
    {145, Rad::Real, 2, 0, 100000, 20, "Sp", "S<sub>p</sub>", QObject::tr("Average sludge production"), "kg/a/person", "kg/a/person", QObject::tr("Average sludge production per person in dry weight.")};

Quantity SewageSludgeDensity =
    {146, Rad::Real, 2, 0, MAX_REAL, 1000, "p", "p", QObject::tr("Sewage sludge density"), "kg/m3", "kg/m<sup>3</sup>", QObject::tr("The density of sewage sludge.")};

Quantity SludgeContainerDepth =
    {147, Rad::Real, 2, 0, 100000, 1, "d", "d", QObject::tr("Sewage container depth"), "m", "m", QObject::tr("Depth of sewage sludge container.")};
Quantity ConcentrationInWetSewage =
    {148, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Csl", "C<sub>sluldge</sub>", QObject::tr("Concentration in sludge (wet weight)"), "Bq/kg", "Bq/kg", QObject::tr("Annual average radionuclide concentration in sewage sludge (wet weight)")};

Quantity Concentration =
    {149, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "C", "C", QObject::tr("Concentration"), "", "", QObject::tr("Radionuclide concentration")};

Quantity TotalDose =
    {150, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "E", "E", QObject::tr("Total dose"), "Sv/a", "Sv/a", QObject::tr("Total dose rate")};

Quantity AbsoluteAirHumidity =
    {151, Rad::NumText, DIGIT_DEF, 0, MAX_REAL, 6e-3, "Hx", "H<sub>x</sub>", QObject::tr("Absolute air humidity"), "L/m3", "L/m<sup>3</sup>", QObject::tr("Absolute humidity of the atmosphere")};
Quantity AtmosphericTritiumConcentration =
    {152, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cax", "C<sub>ax</sub>", QObject::tr("Concentration of H-3 in atmosphere"), "Bq/L", "Bq/L", QObject::tr("Steady state concentration of H-3 in atmospheric wator vapour")};
Quantity WaterTritiumConcentration =
    {153, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cwx", "C<sub>wx</sub>", QObject::tr("Concentration of H-3 in water"), "Bq/L", "Bq/L", QObject::tr("Steady state concentration of H-3 in water")};
Quantity AtmosphericWaterIntakeFraction =
    {154, Rad::Real, DIGIT_DEF, 0, 1, 1, "fA", "f<sub>A</sub>", QObject::tr("Fraction of total water intake from atmosphere"), "", "", QObject::tr("Fraction of the total water intake that is derived from atmospheric water vapour")};
Quantity WaterIntakeFraction =
    {155, Rad::Real, DIGIT_DEF, 0, 1, 1, "fW", "f<sub>W</sub>", QObject::tr("Fraction of total water intake from water"), "", "", QObject::tr("Fraction of the total water intake from aquatic discharges")};
Quantity TritiumDoseFactor =
    {156, Rad::NumText, DIGIT_DEF, 0, MAX_REAL, 2.6e-8, "g", "g", QObject::tr("Dose factor of H-3"), "Sv/a per Bq/L", "Sv/a per Bq/L", QObject::tr("Dose rate conversion factor for H-3")};
Quantity SpecificCarbonActivity =
    {157, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ax", "A<sub>x</sub>", QObject::tr("Activity of C-14 per gram of C"), "Bq/g", "Bq/g", QObject::tr("Specific activity of C-14 per gram of Carbon")};
Quantity DietaryCarbonFraction =
    {158, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 1, "fca", "f<sub>ca</sub>", QObject::tr("Fraction of total dietary carbon"), "", "", QObject::tr("Fraction of total dietary carbon")};
Quantity CarbonDoseFactor =
    {159, Rad::NumText, DIGIT_DEF, 0, MAX_REAL, 5.6e-5, "g", "g", QObject::tr("Dose factor of C-14"), "Sv/a per Bq/g", "Sv/a per Bq/g", QObject::tr("Effective dose rate factor that relates to annual dose rate to the concentration of C-14 per gram of carbon in people")};
Quantity AirborneCarbonConcentration =
    {160, Rad::NumText, DIGIT_DEF, 0, MAX_REAL, 0.18, "Cx", "C<sub>x</sub>", QObject::tr("Concentration of airborne carbon"), "g/m3", "g/m<sup>3</sup>", QObject::tr("Concentration of airborne carbon (default value assumed to be 0.18, corresponding to an average CO2 concentration of 330 ppm")};

Quantity ScalarOutput =
    {161, Rad::Real, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "Y","Y",QObject::tr("Output"),"","","Arbitrary numeric value"};
Quantity CommentQuantity =
    {162, Rad::Comment, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "C","C",QObject::tr("Comment"),"","","Arbitrary comment"};
Quantity NameQuantity =
    {163, Rad::Text, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "","",QObject::tr("Name"),"","","Arbritrary name"};
Quantity UseDefaultValue =
    {164, Rad::Boolean, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "ud","ud",QObject::tr("Use predefined default value(s)"),"","","Use default value defined in software."};
Quantity LongCommentQuantity =
    {165, Rad::LongComment, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "C","C",QObject::tr("Comment"),"","","Full row arbitrary comment"};
Quantity UseDefaultValue2 =
    {166, Rad::Boolean, DIGIT_DEF, MIN_REAL, MAX_REAL, 0, "ud","ud",QObject::tr("Use predefined default value(s)"),"","","Use default value defined in software."};

//end of symbol namespace
}

