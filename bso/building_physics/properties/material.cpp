#ifndef BSO_BP_MATERIAL_CPP
#define BSO_BP_MATERIAL_CPP

#include <sstream>
#include <stdexcept>

namespace bso { namespace building_physics { namespace properties {

material::material(const std::string& ID, const std::string& description,
	const double& specificWeight, const double& specificHeat,
	const double& thermalConductivity)
:	mID(ID), mDescription(description), mSpecificWeight(specificWeight),
	mSpecificHeat(specificHeat), mThermalConductivity(thermalConductivity)
{
	if (mSpecificWeight <= 0)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize a material with an invalid\n"
								 << "specific weight: " << mSpecificWeight << ".\n"
								 << "value should be non-negative and non-zero.\n"
								 << "(bso/building_physics/properties/material.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (mSpecificHeat <= 0)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize a material with an invalid\n"
								 << "specific heat: " << mSpecificHeat << ".\n"
								 << "value should be non-negative and non-zero.\n"
								 << "(bso/building_physics/properties/material.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (mThermalConductivity <= 0)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize a material with an invalid\n"
								 << "thermal conductivity: " << mThermalConductivity << ".\n"
								 << "value should be non-negative and non-zero.\n"
								 << "(bso/building_physics/properties/material.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // ctor()

material::~material()
{
	
} // dtor()

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_MATERIAL_CPP