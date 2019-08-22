#ifndef BSO_BP_SPACE_SETTINGS_CPP
#define BSO_BP_SPACE_SETTINGS_CPP

#include <sstream>
#include <stdexcept>


namespace bso { namespace building_physics { namespace properties {

space_settings::space_settings(const std::string& ID, const double& heatingCapacity,
	const double& coolingCapacity, const double& heatingSetPoint,
	const double& coolingSetPoint, const double& ACH)
:	mID(ID), mHeatingCapacity(heatingCapacity), mCoolingCapacity(coolingCapacity),
	mHeatingSetPoint(heatingSetPoint), mCoolingSetPoint(coolingSetPoint), mACH(ACH)
{
	if (mHeatingCapacity < 0)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize space settings with an invalid\n"
								 << "heating capacity: " << mHeatingCapacity << ".\n"
								 << "value should be non-negative.\n"
								 << "(bso/building_physics/properties/space_setting.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (mCoolingCapacity < 0)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize space settings with an invalid\n"
								 << "cooling capacity: " << mCoolingCapacity << ".\n"
								 << "value should be non-negative.\n"
								 << "(bso/building_physics/properties/space_setting.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (mACH < 0)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize space settings with an invalid\n"
								 << "air changes per hour (ACH): " << mACH << ".\n"
								 << "value should be non-negative.\n"
								 << "(bso/building_physics/properties/space_setting.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (mHeatingSetPoint > mCoolingSetPoint - 1)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize space settings with a\n"
								 << "heating set point (" << mHeatingSetPoint << ")" 
								 << "that is larger than the cooling set point (" << mCoolingSetPoint
								 << "), or too close to it.\n Cooling set point should be at least\n"
								 << "one degree Kelving higher than the heating set point\n"
								 << "(bso/building_physics/properties/space_setting.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // ctor()

space_settings::~space_settings()
{
	
} // dtor()

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_SPACE_SETTINGS_CPP