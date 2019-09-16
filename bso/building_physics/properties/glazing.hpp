#ifndef BSO_BP_GLAZING_HPP
#define BSO_BP_GLAZING_HPP

namespace bso { namespace building_physics { namespace properties {

class glazing
{
private:
	std::string mID;
	std::pair<std::string,Eigen::Vector4d> mVisualizationProperties;
	double mU;
	double mCapacitancePerArea;
public:
	glazing(const std::string& ID, const double& U, const double& capacitancePerArea,
				  const std::pair<std::string,Eigen::Vector4d>& visualizationProps = {"",Eigen::Vector4d::Ones()});
	~glazing();
	
	const std::string& getID() const {return mID;}
	const std::pair<std::string,Eigen::Vector4d> & getVisualizationProperties() const {return mVisualizationProperties;}
	const double& getU() const {return mU;}
	const double& getCapacitancePerArea() const {return mCapacitancePerArea;}
	
	glazing& operator = (const glazing& rhs);
};

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/properties/glazing.cpp>

#endif // BSO_BP_GLAZING_HPP