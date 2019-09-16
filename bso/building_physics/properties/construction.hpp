#ifndef BSO_BP_CONSTRUCTION_HPP
#define BSO_BP_CONSTRUCTION_HPP

#include <bso/building_physics/properties/layer.hpp>

namespace bso { namespace building_physics { namespace properties {

class construction
{
private:
	std::string mID;
	std::pair<std::string,Eigen::Vector4d> mVisualizationProperties;
	std::vector<layer> mLayers;
	double mThickness;
	double mCapacitancePerArea;
	double mResistanceSide1;
	double mResistanceSide2;
	double mRelativeMeasurePoint;
public:
	construction(const std::string& ID, const std::vector<layer> layers,
							 const double& relativeMeasurePoint = 0.5,
							 const std::pair<std::string,Eigen::Vector4d>& visualizationProps = {"",Eigen::Vector4d::Ones()});
	~construction();
	
	const double& getCapacitancePerArea() const {return mCapacitancePerArea;}
	const double& getResistanceSide1() const {return mResistanceSide1;}
	const double& getResistanceSide2() const {return mResistanceSide2;}
	const std::pair<std::string,Eigen::Vector4d>& getVisualizationProperties() const {return mVisualizationProperties;}
	const std::string& getID() const {return mID;}
	const double& getThickness() const {return mThickness;}
	
	construction& operator = (const construction& rhs);
};

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/properties/construction.cpp>

#endif // BSO_BP_CONSTRUCTION_HPP