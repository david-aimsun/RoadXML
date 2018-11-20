
#include "stk/Unit.h"
#include "stk/Macro.h"

#include <string>

#include <string.h>

#if (_MSC_VER >= 1800)
#pragma execution_character_set("utf-8")
#endif

using namespace stk;

t_unit stk::SIUnit(t_dimension dim)
{
	switch(dim)
	{
	default:
	case TDimNone:						return stk::TUnitNone;
	case TDimLength:					return stk::TUnitMetre;
	case TDimSurface:					return stk::TUnitMetre2;
	case TDimVolume:					return stk::TUnitMetre3;
	case TDimVolumeAngle:				return stk::TUnitMetre3PerRadian;
	case TDimAngle:						return stk::TUnitRadian;
	case TDimMass:						return stk::TUnitKiloGram;
	case TDimVolumetricMass:			return stk::TUnitKiloGramPerMetre3;
	case TDimInertia:					return stk::TUnitKiloGramMetre2;
	case TDimTime:						return stk::TUnitSecond;
	case TDimFrequency:					return stk::TUnitHertz;
	case TDimSpeed:						return stk::TUnitMetrePerSecond;
	case TDimAccel:						return stk::TUnitMetrePerSecond2;
	case TDimSpeedAngle:				return stk::TUnitRadianPerSecond;
	case TDimAccelAngle:				return stk::TUnitRadianPerSecond2;
	case TDimForce:						return stk::TUnitNewton;
	case TDimPressure:					return stk::TUnitPascal;
	case TDimEnergy:					return stk::TUnitJoule;
	case TDimTorque:					return stk::TUnitNewtonMetre;
	case TDimPower:						return stk::TUnitWatt;
	case TDimDynamicViscosity:			return stk::TUnitPascalSecond;
	case TDimCinematicViscosity:		return stk::TUnitMetre2PerSecond;
	case TDimTemperature:				return stk::TUnitKelvin;
	case TDimIntensity:					return stk::TUnitAmpere;
	case TDimTension:					return stk::TUnitVolt;
	case TDimResistance:				return stk::TUnitOhm;
	case TDimQuantElec:					return stk::TUnitCoulomb;
	case TDimInductance:				return stk::TUnitHenry;
	case TDimVariationAngle:			return stk::TUnitParRadian;
	case TDimVariationForce:			return stk::TUnitParNewton;
	case TDimConstanteTorqueAngle:		return stk::TUnitNewtonMetrePerRadian2PerSeconde2;
	case TDimConstanteTorqueElecMotor:	return stk::TUnitAmperePerNewtonMetre;
	case TDimStiffness:					return stk::TUnitNewtonPerMetre;
	case TDimAngularStiffness:			return stk::TUnitNewtonPerRadian;
	case TDimAngularSpeedStiffness:		return stk::TUnitDecaNewtonPerTourPerMinute;
	case TDimTorsionAngleStiffness:		return stk::TUnitNewtonMetrePerRadian;
	case TDimSensLinPressureStiffness:	return stk::TUnitNewtonPerMetrePerPascal;
	case TDimTorsionalFlexibility:		return stk::TUnitRadianPerNewtonMetre;
	case TDimLinearDamping:				return stk::TUnitNewtonPerMetrePerSecond;
	case TDimAngularTorsionDamping:		return stk::TUnitNewtonMetrePerRadianPerSecond;
	case TDimFuelConsumption:			return stk::TUnitMetre3PerSecond;
	case TDimSpecConsumption:			return stk::TUnitKiloGrammePerWattPerSecond;
	case TDimDistConsumption:			return stk::TUnitMetre3PerMetre;
	case TDimCurvature:					return stk::TUnitPerMetre;
	case TDimWeightPower:				return stk::TUnitKiloGramPerWatt;
	case TDimFlexibility:				return stk::TUnitMetrePerNewton;
	case TDimBrakeEfficiency:			return stk::TUnitNewtonMetrePerPascal;
	case TDimThermicCapacity:			return stk::TUnitJoulePerKiloGramPerKelvin;
	case TDimThermicConductivity:		return stk::TUnitWattPerMetrePerKelvin;
	case TDimThermicConvection:			return stk::TUnitWattPerMetre2PerKelvin;
	case TDimInstPollution:				return stk::TUnitKiloGramPerSecond;
	case TDimPollutionDist:				return stk::TUnitKiloGramPerMetre;
	case TDimOthers:					return stk::TUnitMetrePerRadian;
	}
}


t_unit stk::commonlyUsedUnit(t_dimension dim)
{
	switch(dim)
	{
	default:
	case TDimNone:						return stk::TUnitNone;
	case TDimLength:					return stk::TUnitMetre;
	case TDimSurface:					return stk::TUnitMetre2;
	case TDimVolume:					return stk::TUnitMetre3;
	case TDimVolumeAngle:				return stk::TUnitCentiMetre3PerTurn;
	case TDimAngle:						return stk::TUnitDegre;
	case TDimMass:						return stk::TUnitKiloGram;
	case TDimVolumetricMass:			return stk::TUnitKiloGramPerDeciMetre3;
	case TDimInertia:					return stk::TUnitKiloGramMetre2;
	case TDimTime:						return stk::TUnitSecond;
	case TDimFrequency:					return stk::TUnitHertz;
	case TDimSpeed:						return stk::TUnitKilometrePerHour;
	case TDimAccel:						return stk::TUnitMetrePerSecond2;
	case TDimSpeedAngle:				return stk::TUnitDegrePerSecond;
	case TDimAccelAngle:				return stk::TUnitDegrePerSecond2;
	case TDimForce:						return stk::TUnitDecaNewton;
	case TDimPressure:					return stk::TUnitBar;
	case TDimEnergy:					return stk::TUnitKiloJoule;
	case TDimTorque:					return stk::TUnitDecaNewtonMetre;
	case TDimPower:						return stk::TUnitKiloWatt;
	case TDimDynamicViscosity:			return stk::TUnitPoiseuille;
	case TDimCinematicViscosity:		return stk::TUnitMetre2PerSecond;
	case TDimTemperature:				return stk::TUnitCelsius;
	case TDimIntensity:					return stk::TUnitAmpere;
	case TDimTension:					return stk::TUnitVolt;
	case TDimResistance:				return stk::TUnitOhm;
	case TDimQuantElec:					return stk::TUnitCoulomb;
	case TDimInductance:				return stk::TUnitHenry;
	case TDimVariationAngle:			return stk::TUnitParDegre;
	case TDimVariationForce:			return stk::TUnitParDecaNewton;
	case TDimConstanteTorqueAngle:		return stk::TUnitDecaNewtonMetrePerTour2PerMinute2;
	case TDimConstanteTorqueElecMotor:	return stk::TUnitAmperePerNewtonMetre;
	case TDimStiffness:					return stk::TUnitNewtonPerMetre;
	case TDimAngularStiffness:				return stk::TUnitDecaNewtonPerDegre;
	case TDimTorsionAngleStiffness:		return stk::TUnitDecaNewtonMetrePerDegre;
	case TDimSensLinPressureStiffness:	return stk::TUnitDecaNewtonPerMilliMetrePerBar;
	case TDimTorsionalFlexibility:		return stk::TUnitDegrePerDecaNewtonMetre;
	case TDimLinearDamping:				return stk::TUnitDecaNewtonPerMetrePerSecond;
	case TDimAngularTorsionDamping:			return stk::TUnitDecaNewtonMetrePerDegrePerSecond;
	case TDimFuelConsumption:					return stk::TUnitLitrePerSecond;
	case TDimSpecConsumption:					return stk::TUnitGramsPerKiloWattPerHour;
	case TDimDistConsumption:					return stk::TUnitLitrePer100KiloMeter;
	case TDimCurvature:					return stk::TUnitPerMetre;
	case TDimWeightPower:			return stk::TUnitKiloGramPerWatt;
	case TDimFlexibility:				return stk::TUnitMilliMetrePerDecaNewton;
	case TDimBrakeEfficiency:			return stk::TUnitDecaNewtonMetrePerBar;
	case TDimThermicCapacity:			return stk::TUnitJoulePerKiloGramPerKelvin;
	case TDimThermicConductivity:		return stk::TUnitWattPerMetrePerKelvin;
	case TDimThermicConvection:		return stk::TUnitWattPerMetre2PerKelvin;
	case TDimInstPollution:				return stk::TUnitGramPerSecond;
	case TDimPollutionDist:				return stk::TUnitGramPerKiloMetre;
	case TDimOthers:					return stk::TUnitDegrePerMilliMetre;
	}
}


t_unit stk::toTUnit(const char* str)
{
	for (int i=0 ; i<stk::ToutesLesUnitessize ; i++)
	{
		if (strcmp(stk::ToutesLesUnites[i].Nom,str)==0)
			return (t_unit)i;
		if (strcmp(stk::ToutesLesUnites[i].NomAlt,str)==0)
			return (t_unit)i;
	}

//les unitées avec des erreurs
	if (strcmp("DaN",str)==0)
		return stk::TUnitDecaNewton;

	if (strcmp("DaN.m",str)==0)
		return stk::TUnitDecaNewtonMetre;

	return stk::TUnitNotFound;
}

double stk::convToSI(t_unit unite, double value)
{
	if (unite>=0 && unite<stk::ToutesLesUnitessize)
		return value*stk::ToutesLesUnites[unite].FacteurConv + stk::ToutesLesUnites[unite].OffsetConv;
	else
		return 0;
}

double stk::convToUnit(stk::t_unit unite, double value)
{
	if (unite>=0 && unite<stk::ToutesLesUnitessize)
		return (value-stk::ToutesLesUnites[unite].OffsetConv)/stk::ToutesLesUnites[unite].FacteurConv;
	else
		return 0;
}

std::istream& stk::operator>>(std::istream& in, t_unit &x)
{
	std::string tmp;
	in >> tmp;
	x = stk::toTUnit(tmp.c_str());
	return in;
}

std::ostream& stk::operator<<(std::ostream& out, const t_unit& x)
{
	if (x<0 || x>=stk::ToutesLesUnitessize)
		return out;
	return out << stk::ToutesLesUnites[x].Nom;
}

//ATTENTION!!
//-on peut en ajouter mais: doivent etre dans le mm ordre que l'enum
//-il ne faut pas modifier le text du nom des unitées (MEME LA CASE!)
stk::DecritUnite stk::ToutesLesUnites[] =
{
// NOT FOUND
	{"Unit not found","Unité non trouvée",1							, 0		, TDimNone},//stk::TUnitNotFound,
// SANS
	{""				,""				,1								, 0		, TDimNone},//stk::TUnitNone,
	{"0.0-1.0"		,"0.0-1.0"		,1								, 0		, TDimNone},//stk::TUnitFraction,
	{"%"			,"%"			,0.01							, 0		, TDimNone},//stk::TUnitPercent,
	{"N/kN"			,"N/kN"			,0.001							, 0		, TDimNone},//stk::TUnitNewtonPerKiloNewton
// LONGUEUR															
	{"m"			,"m"			,1								, 0		, TDimLength},//stk::TUnitMetre,
	{"mm"			,"mm"			,0.001							, 0		, TDimLength},//stk::TUnitMilliMetre,
	{"cm"			,"cm"			,0.01							, 0		, TDimLength},//stk::TUnitCentiMetre,
	{"km"			,"km"			,1000							, 0		, TDimLength},//stk::TUnitKiloMetre,
	{"inch"			,"pouce"		,0.0254							, 0		, TDimLength}, //stk::TUnitInch
	{"feet"			,"pied"			,0.3048							, 0		, TDimLength}, //stk::TUnitFeet
	{"yard"			,"yard"			,0.9144							, 0		, TDimLength}, //stk::TUnitYard
	{"mile"			,"mile"			,1609.344						, 0		, TDimLength}, //stk::TUnitMile
// SURFACE															
	{"m²"			,"m²"			,1								, 0		, TDimSurface},//stk::TUnitMetre2
	{"dm²"			,"dm²"			,0.01							, 0		, TDimSurface},//stk::TUnitDeciMetre2
	{"cm²"			,"cm²"			,0.0001							, 0		, TDimSurface},//stk::TUnitCentiMetre2
	{"mm²"			,"mm²"			,0.000001						, 0		, TDimSurface},//stk::TUnitMilliMetre2
// VOLUME															
	{"m3"			,"m3"			,1								, 0		, TDimVolume}, //stk::TUnitMetre3,
	{"dm3"			,"dm3"			,1.0e-3							, 0		, TDimVolume}, //stk::TUnitDeciMetre3,	
	{"cm3"			,"cm3"			,1.0e-6							, 0		, TDimVolume}, //stk::TUnitCentiMetre3,
	{"mm3"			,"mm3"			,1.0e-9							, 0		, TDimVolume}, //stk::TUnitMilliMetre3,
	{"l"			,"l"			,1.0e-3							, 0		, TDimVolume}, //stk::TUnitLitre,
	{"in3"			,"pouce3"		,1.6387064e-5					, 0		, TDimVolume}, //stk::TUnitInch3
	{"ft3"			,"pied3"		,2.8316846592e-2				, 0		, TDimVolume}, //stk::TUnitFeet3
	{"galUK"		,"galUK"		,4.546092e-3					, 0		, TDimVolume}, //stk::TUnitGallonUK,
	{"galUS"		,"galUS"		,3.785411784e-3					, 0		, TDimVolume}, //stk::TUnitGallonUS,
// VOLUME PAR UNITE ANGULAIRE
	{"m3/rad"		,"m3/rad"		,1								, 0		, TDimVolumeAngle}, //stk::TUnitMetre3PerRadian,
	{"cm3/round"	,"cm3/tr"		,1.0e-6/(2*PI)					, 0		, TDimVolumeAngle}, //stk::TUnitCentiMetre3PerTurn,	
// ANGLE																
	{"rad"			,"rad"			,1								, 0		, TDimAngle},//stk::TUnitRadian
	{"°"			,"°"			,PI/180							, 0		, TDimAngle},//stk::TUnitDegrees
	{"10°"			,"10°"			,10.0*(PI/180)					, 0		, TDimAngle},//stk::TUnitDixDegrees
	{"minute"		,"minute"		,PI/180							, 0		, TDimAngle},//stk::TUnitAngleMinute
	{"millième"		,"millième"		,(2*PI)/6400					, 0		, TDimAngle},//stk::TUnitMillieme
	{"revolution"	,"tour"			,(2*PI)							, 0		, TDimAngle},//stk::TUnitTurn
// MASSE																
	{"kg"			,"kg"			,1								, 0		, TDimMass},//stk::TUnitKiloGram,
	{"g"			,"g"			,0.001							, 0		, TDimMass},//stk::TUnitGram,
	{"tonne"		,"tonne"		,1000							, 0		, TDimMass},//stk::TUnitTon
	{"pound"		,"livre"		,0.45359237						, 0		, TDimMass},//stk::TUnitPound
	{"ounce"		,"once"			,0.028349523125					, 0		, TDimMass},//stk::TUnitOnce
	{"stone"		,"stone"		,6.350029318					, 0		, TDimMass},//stk::TUnitStone
// MASSE VOLUMIQUE													
	{"kg/m3"		,"kg/m3"		,1								, 0		, TDimVolumetricMass},//stk::TUnitKiloGramPerMetre3
	{"kg/dm3"		,"kg/dm3"		,1.0/0.001						, 0		, TDimVolumetricMass},//stk::TUnitKiloGramPerDeciMetre3
// MOMENT D'INERTIE															
	{"kg.m²"		,"kg.m²"		,1								, 0		, TDimInertia},	//stk::TUnitKiloGramMetre2
	{"kg.mm²"		,"kg.mm²"		,0.001*0.001					, 0		, TDimInertia},	//stk::TUnitKiloGramMilliMetre2
// TEMPS																
	{"s"			,"s"			,1								, 0		, TDimTime},//stk::TUnitSecond,
	{"ms"			,"ms"			,0.001							, 0		, TDimTime},//stk::TUnitMilliSecond,
	{"µs"			,"µs"			,0.000001						, 0		, TDimTime},//stk::TUnitMicroSecond,
	{"min"			,"min"			,60								, 0		, TDimTime},//stk::TUnitMinute,
	{"h"			,"h"			,3600							, 0		, TDimTime},//stk::TUnitHour,
// FREQUENCE															
	{"Hz"			,"Hz"			,1								, 0		, TDimFrequency}, //stk::TUnitHertz
	{"1/s"			,"1/s"			,1.0							, 0		, TDimFrequency}, //stk::TUnitPerSecond
	{"1/min"		,"1/min"		,1.0/60.0						, 0		, TDimFrequency}, //stk::TUnitParMinute
// VITESSE															
	{"m/s"			,"m/s"			,1								, 0		, TDimSpeed},//stk::TUnitMetrePerSecond,
	{"km/h"			,"km/h"			,1000.0/3600					, 0		, TDimSpeed},//stk::TUnitKilometrePerHour,
	{"mile/h"		,"mile/h"		,1.609344/3.6					, 0		, TDimSpeed},//stk::TUnitMilesPerHour
	{"mm/s"			,"mm/s"			,0.001							, 0		, TDimSpeed},//stk::TUnitMilliMetrePerSecond
// ACCELERATION														
	{"m/s²"			,"m/s²"			,1								, 0		, TDimAccel},//stk::TUnitMetrePerSecond2,
	{"G"			,"G"			,9.80655						, 0		, TDimAccel},//stk::TUnitG,
	{"mm/s²"		,"mm/s²"		,0.001							, 0		, TDimAccel},//stk::TUnitMilliMetrePerSecond2
// VITESSE ANGULAIRE													
	{"rad/s"		,"rad/s"		,1								, 0		, TDimSpeedAngle},//stk::TUnitRadianPerSecond
	{"rpm"			,"tr/min"		,(2*PI)/60						, 0		, TDimSpeedAngle},//stk::TUnitRevolutionPerMinute
	{"x1000rpm"		,"x1000tr/min"	,(2000*PI)/60					, 0		, TDimSpeedAngle},//stk::TUnit1000RevolutionsPerMinute
	{"°/s"			,"°/s"			,PI/180							, 0		, TDimSpeedAngle}, //stk::TUnitDegrePerSecond
// ACCELERATION ANGULAIRE											
	{"rad/s²"		,"rad/s²"		,1								, 0		, TDimAccelAngle},//stk::TUnitRadianPerSecond2
	{"°/s²"			,"°/s²"			,PI/180							, 0		, TDimAccelAngle},//stk::TUnitDegrePerSecond2
// FORCE																
	{"N"			,"N"			,1								, 0		, TDimForce},//stk::TUnitNewton,
	{"daN"			,"daN"			,10								, 0		, TDimForce},//stk::TUnitDecaNewton
	{"kN"			,"kN"			,1000							, 0		, TDimForce},//stk::TUnitKiloNewton
	{"kgForce"		,"KgForce"		,9.80655						, 0		, TDimForce},//stk::TUnitKiloGrammeForce
	{"daN/%"		,"daN/%"		,10*100							, 0		, TDimForce},//stk::TUnitDecaNewtonParPourcent
	{"lbf"			,"lbf"			,4.44822161526					, 0		, TDimForce},//stk::TUnitPoundForce
// PRESSION															
	{"Pa"			,"Pa"			,1								, 0		, TDimPressure}, //stk::TUnitPascal
	{"kPa"			,"kPa"			,1000							, 0		, TDimPressure}, //stk::TUnitKiloPascal
	{"MPa"			,"MPa"			,1000000						, 0		, TDimPressure}, //stk::TUnitMegaPascal
	{"bar"			,"bar"			,100000							, 0		, TDimPressure},//stk::TUnitBar,
	{"psi"			,"psi"			,6894.75729317					, 0		, TDimPressure},//stk::TUnitPsi,
	{"mbar"			,"millibar"		,100							, 0		, TDimPressure},//stk::TUnitMilliBar,
// CONTRAINTE MÉCANIQUE												
// ENERGIE															
	{"J"			,"J"			,1								, 0		, TDimEnergy},//stk::TUnitJoule
	{"KJ"			,"KJ"			,1000							, 0		, TDimEnergy},//stk::TUnitKiloJoule
// COUPLE															
	{"N.m"			,"N.m"			,1.0							, 0		, TDimTorque},//stk::TUnitNewtonMetre,
	{"daN.m"		,"daN.m"		,10.0							, 0		, TDimTorque},//stk::TUnitDecaNewtonMetre,
	{"Kg.m"			,"Kg.m"			,9.80655						, 0		, TDimTorque},//stk::TUnitKiloGrammeMetre,
// PUISSANCE															
	{"W"			,"W"			,1								, 0		, TDimPower},//stk::TUnitWatt,
	{"bhp"			,"bhp"			,745.699871582					, 0		, TDimPower},//stk::TUnitHP, /*bhp = british horse power*/
	{"hp"			,"ch"			,735.499						, 0		, TDimPower},//stk::TUnitPS,,
	{"kW"			,"kW"			,1000							, 0		, TDimPower},//stk::TUnitKiloWatt,
	{"mW"			,"mW"			,0.001							, 0		, TDimPower},//stk::TUnitMilliWatt
// VISCOSITE DYNAMIQUE (Pa.s)
	{"PI"			,"PI"			,1								, 0		, TDimDynamicViscosity},//stk::TUnitPoiseuille
	{"Pa.s"			,"Pa.s"			,1								, 0		, TDimDynamicViscosity},//stk::TUnitPascalSecond										
	{"mPI"			,"mPI"			,0.001							, 0		, TDimDynamicViscosity},//stk::TUnitMilliPoiseuille
	{"Po"			,"Po"			,0.1							, 0		, TDimDynamicViscosity},//stk::TUnitPoise										
// VISCOSITE CINEMATIQUE												
	{"m²/s"			,"m²/s"			,1								, 0		, TDimCinematicViscosity},//stk::TUnitMetre2PerSecond
	{"mm²/s"		,"mm²/s"		,0.000001						, 0		, TDimCinematicViscosity},//stk::TUnitMilliMetre2ParSecond,
// TEMPERATURE														
	{"K"			,"K"			,1								, 0		, TDimTemperature},//stk::TUnitKelvin
	{"°C"			,"°C"			,1								, 273.15, TDimTemperature},//stk::TUnitCelsius
	{"°F"			,"°F"			,0.5556							, 255.37222222222, TDimTemperature},//stk::TUnitFarenheit
// INTENSITE															
	{"A"			,"A"			,1								, 0		, TDimIntensity},//stk::TUnitAmpere,
	{"mA"			,"mA"			,0.001							, 0		, TDimIntensity}, //stk::TUnitMilliAmpere
// TENSION															
	{"V"			,"V"			,1								, 0		, TDimTension},//stk::TUnitVolt,
	{"mV"			,"mV"			,0.001							, 0		, TDimTension}, //stk::TUnitMilliVolt
// CONDUCTANCE (siemens : S )										
// RESISTANCE														
	{"Ohm"			,"Ohm"			,1								, 0		, TDimResistance},// stk::TUnitOhm
	{"kOhm"			,"kOhm"			,1000							, 0		, TDimResistance},// stk::TUnitKiloOhm
	{"MOhm"			,"MOhm"			,1000000						, 0		, TDimResistance},// stk::TUnitMegaOhm
// QUANTITE D'ELECTRICITE											
	{"C"			,"C"			,1								, 0		, TDimQuantElec},//stk::TUnitCoulomb,
	{"A.s"			,"A.s"			,1								, 0		, TDimQuantElec},//stk::TUnitAmpereSecond
	{"Ah"			,"Ah"			,3600							, 0		, TDimQuantElec},//stk::TUnitAmpereHour
// CAPACITÉ ÉLÉCTRIQUE (Farrad : F)									
// FLUX D'INDUCTION MAGNÉTIQUE (Weber : Wb)							
// INDUCTION MAGNÉTIQUE (Tesla : T)									
// INDUCTANCE														
	{"H"			,"H"			,1								, 0		, TDimInductance},//stk::TUnitHenry,
	{"mH"			,"mH"			,0.001							, 0		, TDimInductance},//stk::TUnitMilliHenry
// CHAMP MAGNÉTIQUE (A/m)											
// VARIATION ANGULAIRE												
	{"1/rad"		,"1/rad"		,1								, 0		, TDimVariationAngle},//stk::TUnitParRadian
	{"1/°"			,"1/°"			,1.0/(PI/180)					, 0		, TDimVariationAngle},//stk::TUnitParDegre
// VARIATION DE FORCE
	{"1/N"			,"1/N"			,1								, 0		, TDimVariationForce},//	stk::TUnitParNewton,
	{"1/daN"		,"1/daN"		,1/10.0							, 0		, TDimVariationForce},//	stk::TUnitParDecaNewton
// CONSTANTE DE COUPLAGE ANGULAIRE									
	{"N.m/(rad/s)²"	,"N.m/(rad/s)²"	,1								, 0		, TDimConstanteTorqueAngle},//stk::TUnitNewtonMetrePerRadian2PerSeconde2
	{"daN.m/rpm²"	,"daN.m/(tr/min)²",10.0/((2*PI/60)*(2*PI/60))	, 0		, TDimConstanteTorqueAngle},//stk::TUnitDecaNewtonMetrePerTour2PerMinute2
// CONSTANTE DE COUPLE DU MOTEUR ELECTRIQUE
	{"A/(N.m)"		,"A/(N.m)"		,1								, 0		, TDimConstanteTorqueElecMotor},//stk::TUnitAmperePerNewtonMetre
// RAIDEUR															
	{"N/m"			,"N/m"			,1								, 0		, TDimStiffness}, //stk::TUnitNewtonPerMetre,
	{"daN/mm"		,"daN/mm"		,10.0/0.001						, 0		, TDimStiffness}, //stk::TUnitDecaNewtonParMiliMetre
	{"kN/mm"		,"kN/mm"		,1000.0/0.001					, 0		, TDimStiffness}, //stk::TUnitKiloNewtonPerMilliMetre
// RAIDEUR ANGULAIRE													
	{"N/rad"		,"N/rad"		,1								, 0		, TDimAngularStiffness}, //stk::TUnitNewtonPerRadian
	{"N/°"			,"N/°"			,1.0/(PI/180)					, 0		, TDimAngularStiffness}, //stk::TUnitNewtonPerDegre
	{"daN/°"		,"daN/°"		,10.0/(PI/180)					, 0		, TDimAngularStiffness}, //stk::TUnitDecaNewtonPerDegre
// RAIDEUR VITESSE ANGULAIRE													
	{"N/(rad/s)"	,"N/(rad/s)"	,1								, 0		, TDimAngularSpeedStiffness}, //stk::TUnitNewtonPerRadianPerSecond
	{"N/(°/s)"		,"N/(°/s)"		,1.0/(PI/180)					, 0		, TDimAngularSpeedStiffness}, //stk::TUnitNewtonPerDegrePerSecond
	{"daN/rpm"		,"daN/(tr/min)"	,10.0/((2*PI)/60)				, 0		, TDimAngularSpeedStiffness}, //stk::TUnitDecaNewtonPerTourPerMinute
// RAIDEUR ANGULAIRE DE TORSION										
	{"N.m/rad"		,"N.m/rad"		,1								, 0		, TDimTorsionAngleStiffness}, //stk::TUnitNewtonMetrePerRadian
	{"daN.m/°"		,"daN.m/°"		,10.0/(PI/180)					, 0		, TDimTorsionAngleStiffness}, //stk::TUnitDecaNewtonMetrePerDegre
	{"N.m/°"		,"N.m/°"		,1.0/(PI/180)				, 0		, TDimTorsionAngleStiffness }, //stk::TUnitNewtonMetrePerDegre
// SENSIBILITE DE LA RAIDEUR LINEAIRE A LA PRESSION					
	{"N/m/Pa"		,"N/m/Pa"		,1								, 0		, TDimSensLinPressureStiffness}, //stk::TUnitNewtonPerMetrePerPascal
	{"daN/mm/bar"	,"daN/mm/bar"	,10.0/0.001/100000				, 0		, TDimSensLinPressureStiffness}, //stk::TUnitDecaNewtonPerMilliMetrePerBar
// FLEXIBILITE DE TORSION											
	{"rad/N.m"		,"rad/N.m"		,1								, 0		, TDimTorsionalFlexibility}, //stk::TUnitRadianPerNewtonMetre
	{"°/daN.m"		,"°/daN.m"		,(PI/180.0)/10.0				, 0		, TDimTorsionalFlexibility}, //stk::TUnitDegrePerDecaNewtonMetre 
// AMORTISSEMENT LINEAIRE											
	{"N/m/s"		,"N/m/s"		,1								, 0		, TDimLinearDamping}, //stk::TUnitNewtonPerMetrePerSecond
	{"daN/m/s"		,"daN/m/s"		,10								, 0		, TDimLinearDamping}, //stk::TUnitDecaNewtonPerMetrePerSecond
// AMORTISSEMENT ANGULAIRE DE TORSION								
	{"N.m/rad/s"	,"N.m/rad/s"	,1								, 0		, TDimAngularTorsionDamping}, //stk::TUnitNewtonMetrePerRadianPerSecond
	{"daN.m/°/s"	,"daN.m/°/s"	,10.0/(PI/180.0)				, 0		, TDimAngularTorsionDamping}, //stk::TUnitDecaNewtonMetrePerDegrePerSecond
	{"N.m/°/s"		, "N.m/°/s"		,1.0 / (PI / 180.0)				, 0		, TDimAngularTorsionDamping}, //stk::TUnitNewtonMetrePerDegrePerSecond
	{ "N.m/rpm"		, "N.m/rpm"		, 1.0 / ((2 * PI) / 60)			, 0		, TDimAngularTorsionDamping}, //stk::TUnitNewtonMetrePerRPM
// CONSOMMATION INSTANTANNEE
	{"m3/s"			,"m3/s"			,1.0							, 0		, TDimFuelConsumption},//stk::TUnitMetre3PerSecond										
	{"l/s"			,"l/s"			,0.001							, 0		, TDimFuelConsumption},//stk::TUnitLitrePerSecond
	{"l/min"		,"l/min"		,0.001/60						, 0		, TDimFuelConsumption},//stk::TUnitLitrePerMinute,
	{"l/h"			,"l/h"			,0.001/3600						, 0		, TDimFuelConsumption},//stk::TUnitLitrePerHour
	{"ml/s"			,"ml/s"			,0.000001						, 0		, TDimFuelConsumption},//stk::TUnitMilliLitrePerSecond
// CONSOMMATION SPEC													
	{"Kg/(W.s)"		,"Kg/(W.s)"		,1								, 0		, TDimSpecConsumption},//stk::TUnitKiloGrammePerWattPerSecond,
	{"g/(kW.h)"		,"g/(kW.h)"		,0.001/(1000.0*3600)			, 0		, TDimSpecConsumption},//stk::TUnitGramsPerKiloWattPerHour
// CONSOMMATION / DIST
	{"m3/m"			,"m3/m"			,1.0							, 0		, TDimDistConsumption},//stk::TUnitMetre3PerMetre											
	{"l/100km"		,"l/100km"		,0.001/100000					, 0		, TDimDistConsumption},//stk::TUnitLitrePer100KiloMeter
// COURBURE															
	{"1/m"			,"1/m"			,1.0							, 0		, TDimCurvature}, //stk::TUnitPerMetre
// RAPPORT POIDS PUISSANCE											
	{"Kg/W"			,"Kg/W"			,1								, 0		, TDimWeightPower},//stk::TUnitKiloGramPerWatt
	{"Kg/KW"		,"Kg/KW"		,1.0/1000						, 0		, TDimWeightPower},//stk::TUnitKiloGramPerKiloWatt
	{"Kg/hp"		,"Kg/ch"		,1.0/735.499					, 0		, TDimWeightPower},//stk::TUnitKiloGrammePerPS
// FLEXIBILITE														
	{"m/N"			,"m/N"			,1								, 0		, TDimFlexibility},//stk::TUnitMetrePerNewton
	{"mm/daN"		,"mm/daN"		,0.001/10						, 0		, TDimFlexibility},//stk::TUnitMiliMetrePardaN
	{"mm/kN"		,"mm/kN"		,0.001/1000.0					, 0		, TDimFlexibility}, //stk::TUnitMilliMetrePerKiloNewton
// EFFICACITE DE FREIN												
	{"N.m/Pa"		,"N.m/Pa"		,1								, 0		, TDimBrakeEfficiency},	//stk::TUnitNewtonMetrePerPascal
	{"daN.m/bar"	,"daN.m/bar"	,0.0001							, 0		, TDimBrakeEfficiency},	//stk::TUnitDecaNewtonMetrePerBar
// CAPACITE THERMIQUE
	{"J/kg/K"		,"J/kg/K"		,1								, 0		, TDimThermicCapacity},	//stk::TUnitJoulePerKiloGramPerKelvin
// CONDUCTIVITE THERMIQUE
	{"W/m/K"		,"W/m/K"		,1								, 0		, TDimThermicConductivity},	//stk::TUnitWattPerMetrePerKelvin
// CONVECTION THERMIQUE
	{"W/m²/K"		,"W/m²/K"		,1								, 0		, TDimThermicConvection},	//stk::TUnitWattPerMetre2PerKelvin
// POLLUTION INSTANTANEE
	{"kg/s"			,"kg/s"			,1								, 0		, TDimInstPollution},	//stk::TUnitKiloGramPerSecond,
	{"g/s"			,"g/s"			,0.001							, 0		, TDimInstPollution},	//stk::TUnitGramPerSecond,
// POLLUTION DISTANCE
	{"kg/m"			,"kg/m"			,1								, 0		, TDimPollutionDist},	//stk::TUnitKiloGramPerMetre
	{"g/km"			,"g/km"			,0.000001						, 0		, TDimPollutionDist},	//stk::TUnitGramPerKiloMetre,
// AUTRES															
	{"°/mm"			,"°/mm"			,(PI/180.0)/0.001				, 0		, TDimOthers},	//stk::TUnitDegresParMilliMetre
	{"°/g"			,"°/g"			,(PI/180.0)/9.80655				, 0		, TDimOthers},	//stk::TUnitDegresParG
	{"rpm/(daN.m)^1/2","tr/min/(daN.m)^1/2",0.033115				, 0		, TDimOthers},	//stk::TUnitRPMPerRacinedaNMetre
	{"m/rad"		,"m/rad"		,1								, 0		, TDimOthers},	//stk::TUnitMetrePerRadian
	{"mm/°"			,"mm/°"			,0.18/PI						, 0		, TDimOthers},	//stk::TUnitMillimetrePerDegre
	{"bar/daN"		,"bar/daN"		,100000/10						, 0		, TDimOthers}	//stk::TUnitBarPerDecaNewton
};

const int stk::ToutesLesUnitessize = ARRAY_SIZE(stk::ToutesLesUnites);

t_dimension stk::getDimension(t_unit unite)
{
	if (unite<0 || unite>=stk::ToutesLesUnitessize)
		return TDimNone;
	return stk::ToutesLesUnites[unite].dim;
}

const char* stk::toText(t_unit unite)
{
	if (unite<0 || unite>ARRAY_SIZE(stk::ToutesLesUnites))
		return stk::ToutesLesUnites[0].Nom;
	return stk::ToutesLesUnites[unite].Nom;
}
