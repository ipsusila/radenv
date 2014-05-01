#ifndef QUANTITY_H
#define QUANTITY_H

#include "radglobal.h"

namespace Srs19 {
    ConstQuantityList availableQuantities();
    QuantityList editableQuantities();
    bool reloadQuantities(IModelFactory * factory);
    bool saveQuantities(IModelFactory * factory);

    //quantities
    extern Quantity DischargePeriod;
    extern Quantity Diameter;
    extern Quantity NumOfServedPerson;

    extern Quantity CrossSectionalArea;
    extern Quantity LeeGaussianDiffusionFactor;
    extern Quantity AirConcentrationIncrease;
    extern Quantity ConcentrationInAir;
    extern Quantity DailyDepositionRate;
    extern Quantity SigmaFitParameterE;
    extern Quantity SigmaFitParameterG;
    extern Quantity ReductionFactor;
    extern Quantity HeightGaussianDiffusionFactor;
    extern Quantity ConstantValue1Metre;
    extern Quantity ReleaseHeight;
    extern Quantity BuildingHeight;
    extern Quantity BuildingWidth;
    extern Quantity FractionOfWind;
    extern Quantity AtmosphericDischargeRate;
    extern Quantity GeometricMeanOfWindSpeed;
    extern Quantity AirFlowRate;
    extern Quantity DryDepositionCoeff;
    extern Quantity WetDepositionCoeff;
    extern Quantity TotalDepositionCoeff;
    extern Quantity DownwindDistance;
    extern Quantity RadiactiveDecayConstant;
    extern Quantity VerticalDiffusionParameter;

    extern Quantity RiverEstuaryWidth;
    extern Quantity EstimatedRiverWidth;
    extern Quantity EffluentRadionuclideConcentration;
    extern Quantity ConcentrationInBottomSediment;
    extern Quantity ConcentrationInShoreSediment;
    extern Quantity ConcentrationInSuspendedSediment;
    extern Quantity OppositeBankMaxConcentration;
    extern Quantity ConcentrationInDrySewage;
    extern Quantity ConcentrationInFilteredWater;
    extern Quantity TotalConcentrationInWater;
    extern Quantity FlowDepth;
    extern Quantity LiquidFlowRate;
    extern Quantity SedimentDistributionCoeff;
    extern Quantity MixingCorrectionFactor;
    extern Quantity LowRiverFlowRate;
    extern Quantity MeanRiverFlowRate;
    extern Quantity WaterDischargeRate;
    extern Quantity SewageDischargeRate;
    extern Quantity AnnualSludgeProduction;
    extern Quantity SuspendedSedimentConcentration;
    extern Quantity EffShoreAccumulationTime;
    extern Quantity TidalPeriod;
    extern Quantity NetFreshwaterVelocity;
    extern Quantity LongitudinalDistance;
    extern Quantity ReleaseToBeachDistance;

    extern Quantity BioaccumulationFactor;
    extern Quantity ConcentrationFactor;
    extern Quantity ConcentrationInAnimalFeed;
    extern Quantity ConcentrationInAquaticFood;
    extern Quantity ConcentrationInMeat;
    extern Quantity ConcentrationInMilk;
    extern Quantity ConcentrationInStoredAnimalFeed;
    extern Quantity ConcentrationInDrySoil;
    extern Quantity ConcentrationInVegetation;
    extern Quantity ConcentrationInWater;
    extern Quantity FractionIntakeAtSlaughter;
    extern Quantity FractionIntakeAtMilking;
    extern Quantity AnnualPastureFraction;
    extern Quantity UptakeConcentrationFactor;
    extern Quantity IrrigationRate;
    extern Quantity MeatAnimalFeed;
    extern Quantity MilkAnimalFeed;
    extern Quantity WaterConsumption;
    extern Quantity CropExposedPeriod;
    extern Quantity IntervalAfterSlaughter;
    extern Quantity IntervalAfterHarvest;
    extern Quantity IntervalAfterMilking;
    extern Quantity MassInterceptionFactor;
    extern Quantity ReductionRateOnRoot;
    extern Quantity ReductionRateOnVegetation;
    extern Quantity ReductionOnSurface;
    extern Quantity ReductionOnSoil;
    extern Quantity SurfaceSoilDensity;

    extern Quantity ConcentrationInSurfaceSoil;
    extern Quantity ConcentrationInFoodstuff;
    extern Quantity ConcentrationInSedimentSurface;
    extern Quantity SurfaceSludgeConcentration;
    extern Quantity SkinDoseCoeff;
    extern Quantity GroundExposureDoseCoeff;
    extern Quantity ImmersionDoseCoeff;
    extern Quantity IngestionDoseCoeff;
    extern Quantity InhalationDoseCoeff;
    extern Quantity DustLoadingFactor;
    extern Quantity GroundExposureDoseRate;
    extern Quantity InhalationDoseRate;
    extern Quantity ImmersionDoseRate;
    extern Quantity SkinDoseRate;
    extern Quantity IngestionDoseRate;
    extern Quantity SedimentDoseRate;
    extern Quantity ResuspensionDoseRate;
    extern Quantity SewageDoseRate;
    extern Quantity ConsumptionRate;
    extern Quantity ExposedFractionTime;
    extern Quantity InhalationRate;
    extern Quantity Density;

    extern Quantity OnSameBuilding;
    extern Quantity CorrectedVerticalDiffusionParameter;
    extern Quantity ReceptorOnOpposite;
    extern Quantity EstimateParameters;
    extern Quantity MaximumEbbVelocity;
    extern Quantity MaximumFloodVelocity;
    extern Quantity TidalFlowRate;
    extern Quantity MeanTidalFlowSpeed;
    extern Quantity ReceptorAtUpstream;

    extern Quantity WaterDepth;
    extern Quantity PartialMixingIndex;
    extern Quantity CompleteMixingDistance;
    extern Quantity UpstreamTravelDistance;
    extern Quantity TidalToTimescaleRatio;
    extern Quantity LongitudinalCoeffRatio;
    extern Quantity ConcentrationInShoreline;
    extern Quantity ConcentrationInWaterFish;
    extern Quantity CoastalCurrent;
    extern Quantity UpstreamToDownstreamFactor;
    extern Quantity LateralDispersionCoeff;
    extern Quantity DistanceFromShoreline;
    extern Quantity UseOkuboDispersionExp;
    extern Quantity LakeSurfaceArea;
    extern Quantity LakeVolume;
    extern Quantity LakeDepth;

    extern Quantity CombinedReleaseRate;
    extern Quantity LakeFlowVelocity;
    extern Quantity IsSaltWater;

    extern Quantity WaterDailyIrrigation;
    extern Quantity AnnualIrrigationPeriod;
    extern Quantity IrrigationDays;
    extern Quantity AverageDepositionRate;
    extern Quantity AnnualAverageDepositionRate;
    extern Quantity AnnualIrrigationRate;

    extern Quantity LambdaConstant;

    extern Quantity ConsDueDirectContamination;
    extern Quantity ConsDueIndirectProcess;
    extern Quantity DailyFeedingAmount;
    extern Quantity ConsumptionDelay;

    extern Quantity IsAnion;
    extern Quantity IsShortLiveNuclide;
    extern Quantity IncrementValue;
    extern Quantity SolidMaterialConcentration;
    extern Quantity PersonSludgeProduction;
    extern Quantity SewageSludgeDensity;
    extern Quantity SludgeContainerDepth;
    extern Quantity ConcentrationInWetSewage;
    extern Quantity Concentration;

    extern Quantity TotalDose;

    extern Quantity AbsoluteAirHumidity;
    extern Quantity AtmosphericTritiumConcentration;
    extern Quantity WaterTritiumConcentration;
    extern Quantity AtmosphericWaterIntakeFraction;
    extern Quantity WaterIntakeFraction;
    extern Quantity TritiumDoseFactor;
    extern Quantity SpecificCarbonActivity;
    extern Quantity DietaryCarbonFraction;
    extern Quantity CarbonDoseFactor;
    extern Quantity AirborneCarbonConcentration;
}

#endif // QUANTITY_H
