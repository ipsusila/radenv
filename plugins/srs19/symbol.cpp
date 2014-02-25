#include <QObject>
#include "symbol.h"

namespace Srs19 {
//symbol definition
//ID, type, digit, min, max, default, symbol, symbol(rich-text), label, unit, unit(rich-text), detail

const Symbol DischargePeriod =
    {1, Rad::Real, 1, 0, MAX_REAL, 30, "tb", "t_b", QObject::tr("Discharge period"), "year(s)", "year(s)", QObject::tr("Duration of discharge")};
const Symbol NumOfServedPerson =
    {2, Rad::Integer, DIGIT_NONE, 0, MAX_INT, 0, "Np", "N_p", QObject::tr("Served person"), "", "", QObject::tr("Number of served person in the facility")};
const Symbol Diameter =
    {3, Rad::Real, 2, 0, MAX_REAL, 0, "d", "d", QObject::tr("Diameter"), "m", "m", QObject::tr("Stack diameter")};


const Symbol CrossSectionalArea =
    {4, Rad::Real, 2, 0, MAX_REAL, 0, "Ab", "A_b", QObject::tr("Cross-sectional area"), "m2", "m^2", QObject::tr("Projected Cross-sectional area of the building most influencing the flow of plume")};
const Symbol LeeGaussianDiffusionFactor =
    {5, Rad::NumText, 2, 0, 100, 0, "B", "B", QObject::tr("Gaussian diffusion factor"), "m-2", "m^{-2}", QObject::tr("Gaussian diffusion factor corrected for dispersion in the lee fo a building inside the wake zone (use instead of F)")};
const Symbol AirConcentrationIncrease =
    {6, Rad::Real, 2, 0, MAX_REAL, 30, "B0", "B_0", QObject::tr("Air concentration increase"), "", "", QObject::tr("Unitless constant that accounts for the potential increase in air concentration along a vertical wall")};
const Symbol ConcentrationInAir =
    {7, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ca", "C_A", QObject::tr("Air concentration"), "Bq/m3", "Bq/m^3", QObject::tr("Annual average concentration of radionuclide in air (at ground level)")};
const Symbol DailyDepositionRate =
    {8, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "di", "d_i", QObject::tr("Daily deposition rate"), "Bq.m-2.d-1", "Bq.m^{-2}.d-1", QObject::tr("Daily average deposition rate to the ground surface of radionuclide i from both dry and wet processes")};
const Symbol SigmaFitParameterE =
    {9, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "E", "E", QObject::tr("Sigma fit parameter E"), "", "", QObject::tr("Fit parameters for the equation that defines sigma-z")};
const Symbol SigmaFitParameterG =
    {10, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "G", "G", QObject::tr("Sigma fit parameter G"), "", "", QObject::tr("Fit parameters for the equation that defines sigma-z")};
const Symbol ReductionFactor =
    {11, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "f", "f", QObject::tr("Reduction factor"), "", "", QObject::tr("Unitless reduction factor accounting for radioactive decay during atmospheric transport")};
const Symbol HeightGaussianDiffusionFactor =
    {12, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "F", "F", QObject::tr("Height Gaussian diffusion factor"), "m-2", "m^{-2}", QObject::tr("Gaussian diffusion factor appropriate for the height of release H and downwind distance x")};
const Symbol ConstantValue1Metre =
    {13, Rad::Real, 2, 0, MAX_REAL, 1, "K", "K", QObject::tr("Constant 1 m"), "m", "m", QObject::tr("Constant with a value of 1m")};
const Symbol ReleaseHeight =
    {14, Rad::Real, 2, 0, MAX_REAL, 0, "H", "H", QObject::tr("Release height"), "m", "m", QObject::tr("Height at which the radionuclide is released")};
const Symbol BuildingHeight =
    {15, Rad::Real, 2, 0, MAX_REAL, 0, "Hb", "H_B", QObject::tr("Building height"), "m", "m", QObject::tr("Height of the building that dominates the air flow near the radionuclide release point")};
const Symbol BuildingWidth =
    {16, Rad::Real, 2, 0, MAX_REAL, 0, "Wb", "W_B", QObject::tr("Building width"), "m", "m", QObject::tr("Width of the building that dominates the air flow near the radionuclide release point")};
const Symbol FractionOfWind =
    {17, Rad::Real, 4, 0, 10, 0.25, "Pp", "P_p", QObject::tr("Fraction of wind"), "", "", QObject::tr("Fraction of time during the year that the wind blows towards the receptor of intereset in sector p")};
const Symbol AtmosphericDischargeRate =
    {18, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qi", "Q_i", QObject::tr("Atmospheric discharge"), "Bq/s", "Bq/s", QObject::tr("Annual average rate of radionuclide emission to the atmosphere")};
const Symbol GeometricMeanOfWindSpeed =
    {19, Rad::Real, 4, 0, MAX_REAL, 0, "ua", "u_a", QObject::tr("Geometric mean of wind speed"), "m/s", "m/s", QObject::tr("Annual geometric mean of the wind speed at the height of the release")};
const Symbol AirFlowRate =
    {20, Rad::Real, 4, 0, MAX_REAL, 0, "V", "V", QObject::tr("Air flow rate"), "m3/s", "m3/s", QObject::tr("Volumetric flow rate of vent or stack athe point of release")};
const Symbol DryDepositionCoeff =
    {21, Rad::Real, 2, 0, MAX_REAL, 500, "Vd", "V_d", QObject::tr("Dry deposition coefficient"), "m/d", "m/d", QObject::tr("Dry deposition coefficient (deposition on the soil and vegetation)")};
const Symbol WetDepositionCoeff =
    {22, Rad::Real, 2, 0, MAX_REAL, 500, "Vw", "V_w", QObject::tr("Wet deposition coefficient"), "m/d", "m/d", QObject::tr("Wet deposition coefficient")};
const Symbol TotalDepositionCoeff =
    {23, Rad::Real, 2, 0, MAX_REAL, 1000, "Vt", "V_t", QObject::tr("Total deposition coefficient"), "m/d", "m/d", QObject::tr("Total deposition coefficient")};
const Symbol DownwindDistance =
    {24, Rad::Real, 2, 0, MAX_REAL, 0, "x", "x", QObject::tr("Receptor distance"), "m", "m", QObject::tr("Location of the receptor (downwind distance)")};
const Symbol RadiactiveDecayConstant =
    {25, Rad::NumText, 4, 0, MAX_REAL, 0, "li", "l_i", QObject::tr("Decay constant"), "s-1", "s^{-1}", QObject::tr("Radioactive decay constant for radionuclide i")};
const Symbol VerticalDiffusionParameter =
    {26, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "sz", "s_z", QObject::tr("Vertical diffusion parameter"), "m", "m", QObject::tr("Vertical diffusion parameter")};

//---
const Symbol RiverEstuaryWidth =
    {27, Rad::Real, 2, 0, MAX_REAL, 0, "B", "B", QObject::tr("River/Estuary width"), "m", "m", QObject::tr("River or estuary width")};
const Symbol TidalRiverWidth =
    {28, Rad::Real, 2, 0, MAX_REAL, 0, "Bd", "Bd", QObject::tr("River width"), "m", "m", QObject::tr("River width under a mean of annual river flow rate upstream of the tidal flow rate")};
const Symbol EffluentRadionuclideConcentration =
    {29, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "C0", "C_0", QObject::tr("Concentration in effluent"), "Bq/m3", "Bq/m^3", QObject::tr("Radionuclide concentration in water at the effluent discharge outfall")};
const Symbol ConcentrationInBottomSediment =
    {30, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs,b", "C_{s,b}", QObject::tr("Concentration in bottom sediment"), "Bq/kg", "Bq/kg", QObject::tr("Radionuclide concentration in bottom sediment")};
const Symbol ConcentrationInShore =
    {31, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs,s", "C_{s,s}", QObject::tr("Concentration in shore sediment"), "Bq/m2", "Bq/m2", QObject::tr("Radionuclide concentration in shore or beach sediment")};
const Symbol ConcentrationInSuspendedSediment =
    {32, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs,w", "C_{s,w}", QObject::tr("Concentration in suspended sediment"), "Bq/kg", "Bq/kg", QObject::tr("Radionuclide concentration in suspended sediment")};
const Symbol OppositeBankMaxConcentration =
    {33, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cte", "C_{te}", QObject::tr("Opposite bank max concentration"), "Bq/m3", "Bq/m^3", QObject::tr("Maximum radionuclide concentration on the river bank opposite to the discharge point")};
const Symbol AverageConcentrationInSewage =
    {34, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Csl", "C_{sluldge}", QObject::tr("Concentration in sludge"), "Bq/kg", "Bq/kg", QObject::tr("Annual average radionuclide concentration in sewage sludge")};
const Symbol ConcentrationInFilteredWater =
    {35, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cw,s", "C_{w,s}", QObject::tr("Concentration in filtered water"), "Bq/m3", "Bq/m^3", QObject::tr("Radionuclide concentration in filtered water")};
const Symbol TotalConcentrationInWater =
    {36, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cw,tot", "C_{w,tot}", QObject::tr("Total concentration in water"), "Bq/m3", "Bq/m^3", QObject::tr("Total radionuclide concentration in water")};
const Symbol FlowDepth =
    {37, Rad::Real, 2, 0, MAX_REAL, 0, "D", "D", QObject::tr("Flow depth"), "m", "m", QObject::tr("Flow depth that correspondence to qr")};
const Symbol LiquidFlowRate =
    {38, Rad::Real, 4, 0, MAX_REAL, 0, "F", "F", QObject::tr("Liquid flow rate"), "m3/s", "m^3/s", QObject::tr("Flow rate of the liquid effluent")};
const Symbol SedimentDistributionCoeff =
    {39, Rad::NumText, 0, 0, MAX_REAL, 0, "Kd", "K_d", QObject::tr("Sediment distribution coefficient"), "L/kg", "L/kg", QObject::tr("Distribution coefficient between the radionuclide on sediment and that dissolved in water")};
const Symbol MixingCorrectionFactor =
    {40, Rad::NumText, DIGIT_DEF, 0, MAX_REAL, 0, "Pr", "P_r", QObject::tr("Mixing correction factor"), "", "", QObject::tr("Correction factor for partial mixing")};
const Symbol LowRiverFlowRate =
    {41, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "qr", "q_r", QObject::tr("Low river flow rate"), "m3/s", "m^3/s", QObject::tr("30 year low annual river flow rate")};
const Symbol MeanRiverFlowRate =
    {42, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "qr", "q_r", QObject::tr("Mean river flow rate"), "m3/s", "m^3/s", QObject::tr("Mean annual river flow rate")};
const Symbol WaterDischargeRate =
    {43, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qi", "Q_i", QObject::tr("Surface water discharge"), "Bq/s", "Bq/s", QObject::tr("Average radionuclide discharge rate into river, estuary, lake or coastal water")};
const Symbol SewageDischargeRate =
    {44, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qsl", "Q_{sludge}", QObject::tr("Sewage discharge"), "Bq/a", "Bq/a", QObject::tr("Annual discharge of a radionuclide to a sewage plant")};
const Symbol AnnualSludgeProduction =
    {45, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ssl", "S_{sludge}", QObject::tr("Sludge production"), "kg/a", "kg/a", QObject::tr("Anual sewage sludge production at the relevant sewage treatment works")};
const Symbol SuspendedSedimentConcentration =
    {46, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ss", "S_s", QObject::tr("Suspended sediment concentration"), "kg/m3", "kg/m^3", QObject::tr("Suspended sediment concentration")};
const Symbol EffShoreAccumulationTime =
    {47, Rad::NumText, 0, 0, MAX_REAL, 3.15e+7, "Te", "T_e", QObject::tr("Effective accumulation time"), "s", "s", QObject::tr("Effective accumulation time for shore or beach sediment")};
const Symbol TidalPeriod =
    {48, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Tp", "T_p", QObject::tr("Tidal period"), "s", "s", QObject::tr("Tidal period")};
const Symbol NetFreshwaterVelocity =
    {49, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "U", "U", QObject::tr("Net freshwater velocity"), "m/s", "m/s", QObject::tr("Net freshwater velocity")};
const Symbol LongitudinalDistance =
    {50, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "x", "x", QObject::tr("Longitudinal distance"), "m", "m", QObject::tr("Longitudinal distance from the release point to a potential receptor locatoin")};
const Symbol ReleaseToBeachDistance =
    {51, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "y0", "y_0", QObject::tr("Release to beach distance"), "m", "m", QObject::tr("Distance between the release point and the beach")};

const Symbol BioaccumulationFactor =
    {52, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Bp", "B_p", QObject::tr("Bioaccumulation factor"), "L/kg", "L/kg", QObject::tr("Equilibrium ratio of the concentration of a radionuclide in aquatic food p to its dissolved concentration in water")};
const Symbol ConcentrationFactor =
    {53, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Bv", "Bv", QObject::tr("Uptake concentration factor"), "Bq/kg", "Bq/kg", QObject::tr("Concentration factor for the uptake of radionuclide from soil by edible parts of crops; adhesion of soil to the vegetation is not taken into account. For pasture grass the unit of mass is dry matter; for vegetables consumed by humans it is fresh weight")};
const Symbol ConcentrationInAnimalFeed =
    {54, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ca,i", "C_{a,i}", QObject::tr("Concentration in animal feed"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in animal feed")};
const Symbol ConcentrationInAquaticFood =
    {55, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Caf,i", "C_{af,i}", QObject::tr("Concentration in aquatic food"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in aquatic foods")};
const Symbol ConcentrationInMeat =
    {56, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cf,i", "C_{f,i}", QObject::tr("Concentration in meat"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in meat")};
const Symbol ConcentrationInMilk =
    {57, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cm,i", "C_{m,i}", QObject::tr("Concentration in milk"), "Bq/L", "Bq/L", QObject::tr("Annual average concentration of radionuclide in milk")};
const Symbol ConcentrationInStoredAnimalFeed =
    {58, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cp,i", "C_{p,i}", QObject::tr("Concentration in stored animal feed"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in stored animal feed")};
const Symbol ConcentrationInDrySoil =
    {59, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs", "C_s", QObject::tr("Concentration in dry soil"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in dry soil")};
const Symbol ConcentrationFoodCrop =
    {60, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cv,i", "C_{v,i}", QObject::tr("Concentration in food crop"), "Bq/kg", "Bq/kg", QObject::tr("Annual average concentration of radionuclide in food crops (express in fresh matter for vegetation consumed by humans and in dry matter for vegetation consumed by animals)")};
const Symbol ConcentrationInWater =
    {61, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cw,i", "C_{w,i}", QObject::tr("Concentration in water"), "Bq/L", "Bq/L", QObject::tr("Annual average concentration in water")};
const Symbol FractionIntakeAtSlaughter =
    {62, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Ff", "F_f", QObject::tr("Daily intake at slaughter"), "d/kg", "d/kg", QObject::tr("Fraction of the animal daily intake that appears in a kilogram of flesh at the time of slaughter")};
const Symbol FractionIntakeAtMilking =
    {63, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Fm", "F_m", QObject::tr("Daily intake at milking"), "d/L", "d/L", QObject::tr("Fraction of animal daily intake that appears in a litre of milke at the time of milking")};
const Symbol AnnualPastureFraction =
    {64, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "fp", "fp", QObject::tr("Annual pasture fraction"), "", "", QObject::tr("Fraction of time during a year that an animal consumes fresh pasture")};
const Symbol UptakeConcentrationFactor =
    {65, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Fv", "F_v", QObject::tr("Uptake concentration factor"), "Bq/kg", "Bq/kg", QObject::tr("Concentration factor for the uptake of radionuclide from soil by edible parts of crops, adjusted implicitly to account for adhesion of soil to the vegetation")};
const Symbol IrrigationRate =
    {66, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Iw", "I_w", QObject::tr("Irrigation rate"), "m3.m-2.d-1", "m3.m-2.d-1", QObject::tr("Average irrigation rate")};
const Symbol MeatAnimalFeed =
    {67, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qf", "Q_f", QObject::tr("Meat animal feed"), "kg/d", "kg/d", QObject::tr("Amount of feed consumed per dy by a meat producing animal")};
const Symbol MilkAnimalFeed =
    {68, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qm", "Q_m", QObject::tr("Milk animal feed"), "kg/d", "kg/d", QObject::tr("Amount of feed consumed per da by a milk producing animal")};
const Symbol WaterConsumption =
    {69, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Qw", "Q_w", QObject::tr("Water consumption"), "m3/d", "m3/d", QObject::tr("Amount of water consumed per day by an animal")};
const Symbol CropExposedPeriod =
    {70, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "te", "t_e", QObject::tr("Exposed period"), "d", "d", QObject::tr("Time period that crops are exposed to contamination during the growing season")};
const Symbol IntervalAfterSlaughter =
    {71, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "tf", "f_f", QObject::tr("Period after slaughter"), "d", "d", QObject::tr("Time interval between slaughter and consumption of the meat")};
const Symbol IntervalAfterHarvest =
    {72, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "th", "t_h", QObject::tr("Hold up time"), "d", "d", QObject::tr("Delay (hold up) time that represents the time interval between harvest and consumption of the food")};
const Symbol IntervalAfterMilking =
    {73, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "tm", "tm", QObject::tr("Period after milking"), "d", "d", QObject::tr("Time interval between milking and consumption of the milk")};
const Symbol MassInterceptionFactor =
    {74, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "a", "a", QObject::tr("Mass interception factor"), "m2/kg", "m2/kg", QObject::tr("Mass interception fraction for a given food crop")};
const Symbol ReductionRateOnRoot =
    {75, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "lEis", "l_{E_i^s}", QObject::tr("Reduction rate on root"), "d-1", "d-1", QObject::tr("Effective rate constant responsible for reduction of radionuclide concentration in the root zone")};
const Symbol ReductionRateOnVegetation =
    {76, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "lEiv", "l_{E_i^v}", QObject::tr("Reduction rate on vegetation"), "d-1", "d-1", QObject::tr("Effective rate constant responsible for reduction fo radionuclide concentration on vegetation")};
const Symbol ReductionOnSurface =
    {77, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "lw", "l_w", QObject::tr("Reduction rate on plant surface"), "d-1", "d-1", QObject::tr("Effective rate constant responsible for reduction of the concentration of material deposited on plant surfaces due to process other than radioactive decay")};
const Symbol ReductionOnSoil =
    {78, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "ls", "l_s", QObject::tr("Reduction rate on soil"), "d-1", "d-1", QObject::tr("Effective rate constant responsible for reduction of the concentration of material deposited on the root zone of soils due to processes other than radioactive decay")};
const Symbol SurfaceSoilDensity =
    {79, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "p", "p", QObject::tr("Surface soil density"), "kg/m2", "kg/m2", QObject::tr("Effective surface soil density for the root zone")};

const Symbol ConcentrationInSurfaceSoil =
    {80, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cgr", "C_{gr}", QObject::tr("Ground surface concentration"), "Bq/m2", "Bq/m2", QObject::tr("Ground surface concentration resulting from a 30 year discharge to the atmosphere")};
const Symbol ConcentrationInFoodstuff =
    {81, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cp,i", "C_{p,i}", QObject::tr("Concentration in foodstuff"), "Bq/kg", "Bq/kg", QObject::tr("Concentration of radionuclide i in foodstuff p at the time of consumption")};
const Symbol ConcentrationInSedimentSurface =
    {82, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Cs,s", "C_{s,s}", QObject::tr("Sediment surface concentration"), "Bq/m2", "Bq/m2", QObject::tr("Annual average sediment surface concentration")};
const Symbol SurfaceSludgeConcentration =
    {83, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Csl", "C_{sludge}", QObject::tr("Surface concentration in sludge"), "Bq/m2", "Bq/m2", QObject::tr("Surface concentration deposited in sewage sludge")};
const Symbol SkinDoseCoeff =
    {84, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFs", "DF_s", QObject::tr("Skin dose coefficient"), "Sv/a per Bq/m3", "Sv/a per Bq/m3m", QObject::tr("Dose coefficient for skin from air immersion")};
const Symbol GroundExposureDoseCoeff =
    {85, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFgr", "DF_{gr}", QObject::tr("Ground exposure dose coefficient"), "Sv/a per Bq/m2", "Sv/a per Bq/m2", QObject::tr("Dose coefficient for exposure to contaminated ground or sediment surfaces")};
const Symbol ImmersionDoseCoeff =
    {86, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFim", "DF_{im}", QObject::tr("Immersion dose coefficient"), "Sv/a per Bq/m3", "Sv/a per Bq/m3", QObject::tr("Immersion dose coefficient")};
const Symbol IngestionDoseCoeff =
    {87, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFing", "DF_{ing}", QObject::tr("Ingestion dose coefficient"), "Sv/Bq", "Sv/Bq", QObject::tr("Dose coefficient for ingestion of radionuclides")};
const Symbol InhalationDoseCoeff =
    {88, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DFinh", "DF_{inh}", QObject::tr("Inhalation dose coefficient"), "Sv/Bq", "Sv/Bq", QObject::tr("Dose coefficient for inhalation of radionuclides")};
const Symbol DustLoadingFactor =
    {89, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "DL", "DL", QObject::tr("Dust loading factor"), "kg/m3", "kg/m3", QObject::tr("Dust loading factor")};
const Symbol GroundExposureDoseRate=
    {90, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Egr", "E_{gr}", QObject::tr("Ground exposure dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate from ground deposits after a 30 year discharge to the atmosphere")};
const Symbol InhalationDoseRate =
    {91, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Einh", "E_{inh}", QObject::tr("Inhalation dose rate"), "Sv/a", "Sv/a", QObject::tr("Annual inhalation dose from concentration in air")};
const Symbol ImmersionDoseRate =
    {92, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Eim", "E_{im}", QObject::tr("Immersion dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate from immersion in a discharge cloud")};
const Symbol SkinDoseRate =
    {93, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Eim,s", "E_{im,s}", QObject::tr("Skin immersion dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate to skin from immersion in a discharge cloud")};
const Symbol IngestionDoseRate =
    {94, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Eing,p", "E_{ing,p}", QObject::tr("Ingestion dose rate"), "Sv/a", "Sv/a", QObject::tr("Annual dose from ingestion of foodstuff p")};
const Symbol SedimentDoseRate =
    {95, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Em", "E_m", QObject::tr("Sediment exposure dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate from exposure to contaminated sediment")};
const Symbol ResuspensionDoseRate =
    {96, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Eres", "E_{res}", QObject::tr("Inhalation of resuspended sediment"), "Sv/a", "Sv/a", QObject::tr("Annual inhalation dose from resuspension of radionuclides deposited in sewage sludge")};
const Symbol SewageDoseRate =
    {97, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Es", "E_s", QObject::tr("Sewage sludge dose rate"), "Sv/a", "Sv/a", QObject::tr("External dose rate from concentration in sewage sludge")};
const Symbol ConsumptionRate =
    {98, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Hp", "H_p", QObject::tr("Consumption rate"), "kg/a", "kg/a", QObject::tr("Average consumption rate for foodstuff p")};
const Symbol ExposedFractionTime =
    {99, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Of", "O_f", QObject::tr("Exposed time fraction"), "", "", QObject::tr("Fractoin of time in a year when an individual is exposed to a particular exposure pathway")};
const Symbol InhalationRate =
    {100, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Rinh", "R_{inh}", QObject::tr("Inhalation rate"), "m3/a", "m3/a", QObject::tr("Annual average inhalation rate")};
const Symbol Density =
    {101, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "p", "p", QObject::tr("Density"), "kg/m3", "kg3", QObject::tr("Density")};

const Symbol OnSameBuilding =
    {102, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "sb", "sb", QObject::tr("Source and receptor on same building"), "", "", QObject::tr("Source and receptor on same building surface (dispersion in the lee of a building inside the cavity zone).")};
const Symbol CorrectedVerticalDiffusionParameter  =
    {103, Rad::Real, DIGIT_DEF, 0, MAX_REAL, 0, "Sz", "S_z", QObject::tr("Corrected vertical diffusion parameter"), "m", "m", QObject::tr("Corrected vertical diffusion parameter")};
const Symbol ReceptorOnOpposite =
    {104, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "ro", "ro", QObject::tr("Receptor located at the opposite to discharge point?"), "", "", QObject::tr("Receptor x on the same bank side or in opposite side of the discharge point.")};
const Symbol EstimateParameters =
    {105, Rad::Boolean, DIGIT_DEF, 0, MAX_REAL, 0, "ep", "ep", QObject::tr("Estimate parameter(s)"), "", "", QObject::tr("Estimate needed parameters.")};

//end of symbol namespace
}

