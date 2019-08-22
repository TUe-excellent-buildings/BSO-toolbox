#ifndef BSO_BP_SPACE_SETTINGS_HPP
#define BSO_BP_SPACE_SETTINGS_HPP

namespace bso { namespace building_physics { namespace properties {

class space_settings
{
private:
	std::string mID;
	double mHeatingCapacity;
	double mCoolingCapacity;
	double mHeatingSetPoint;
	double mCoolingSetPoint;
	double mACH;
public:
	space_settings(const std::string& ID, const double& heatingCapacity,
								 const double& coolingCapacity, const double& heatingSetPoint,
								 const double& coolingSetPoint, const double& ACH);
	~space_settings();
	
	const std::string& getID() const {return mID;}
	const double& getHeatingCapacity() const {return mHeatingCapacity;}
	const double& getCoolingCapacity() const {return mCoolingCapacity;}
	const double& getHeatingSetPoint() const {return mHeatingSetPoint;}
	const double& getCoolingSetPoint() const {return mCoolingSetPoint;}
	const double& getACH() const {return mACH;}
};

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/properties/space_settings.cpp>

#endif // BSO_BP_SPACE_SETTINGS_HPP