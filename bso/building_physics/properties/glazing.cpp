#ifndef BSO_BP_GLAZING_CPP
#define BSO_BP_GLAZING_CPP

#include <sstream>
#include <stdexcept>

namespace bso { namespace building_physics { namespace properties {

glazing::glazing(const std::string& ID, const double& U,
	const double& capacitancePerArea,
	const std::pair<std::string,Eigen::Vector4d>& visualizationProps /*= {"",Eigen::Vector4d::Ones()}*/)
:	mID(ID), mU(U), mCapacitancePerArea(capacitancePerArea),
	mVisualizationProperties(visualizationProps)
{
	// mVisualizationProperties = std::pair<std::string,Eigen::Vector4d>(visualizationProps.first, visualizationProps.second);
	if (mU <= 0)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize glazing with an invalid\n"
								 << "U-value: " << mU << ".\n"
								 << "value should be non-negative and non-zero.\n"
								 << "(bso/building_physics/properties/glazing.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (mCapacitancePerArea <= 0)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize glazing with an invalid\n"
								 << "capacitance per area: " << mCapacitancePerArea << ".\n"
								 << "value should be non-negative and non-zero.\n"
								 << "(bso/building_physics/properties/glazing.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // ctor()


glazing::~glazing()
{

} // dtor()

glazing& glazing::operator = (const glazing& rhs)
{
	mID = rhs.mID;
	mVisualizationProperties.first = rhs.mVisualizationProperties.first;
	mVisualizationProperties.second << rhs.mVisualizationProperties.second;mID = rhs.mID;
	mCapacitancePerArea = rhs.mCapacitancePerArea;
	
	return *this;
}

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_GLAZING_CPP