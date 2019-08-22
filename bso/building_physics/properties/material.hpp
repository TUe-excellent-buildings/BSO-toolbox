#ifndef BSO_BP_MATERIAL_HPP
#define BSO_BP_MATERIAL_HPP

#include <string>

namespace bso { namespace building_physics { namespace properties {

class material
{
private:
	std::string mID;
	std::string mDescription;
	double mSpecificWeight;
	double mSpecificHeat;
	double mThermalConductivity;
public:
	material(const std::string& ID, const std::string& description,
					 const double& specificWeight, const double& specificHeat,
					 const double& thermalConductivity);
	~material();
	
	const std::string& getID() const {return mID;}
	const std::string& getDescription() const {return mDescription;}
	const double& getSpecificWeight() const {return mSpecificWeight;}
	const double& getSpecificHeat() const {return mSpecificHeat;}
	const double& getThermalConductivity() const {return mThermalConductivity;}
};

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/properties/material.cpp>

#endif // BSO_BP_MATERIAL_HPP