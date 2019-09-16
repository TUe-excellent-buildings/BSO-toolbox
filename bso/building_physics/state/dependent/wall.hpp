#ifndef BSO_BP_WALL_HPP
#define BSO_BP_WALL_HPP

#include <bso/building_physics/state/dependent/dependent_state.hpp>
#include <bso/building_physics/properties/construction.hpp>

#include <bso/utilities/geometry.hpp>

namespace bso { namespace building_physics { namespace state { namespace dependent {

class wall : public dependent_state
{
private:
	bso::building_physics::properties::construction mConstruction;
	state* mSide1;
	state* mSide2;
	
	double mArea;
	bso::utilities::geometry::polygon* mGeometry;
public:
	wall(const unsigned int& index,  bso::utilities::geometry::polygon* geometry,
			 const bso::building_physics::properties::construction& construction,
			 state* side1, state* side2);
	~wall();
	
	const std::pair<std::string,Eigen::Vector4d>& getVisualizationProperties() const {return mConstruction.getVisualizationProperties();}
	const double& getThickness() const {return mConstruction.getThickness();}
	const double& getArea() const {return mArea;}
	const bso::building_physics::properties::construction& getConstruction() const {return mConstruction;}
	state* getSide1() const {return mSide1;}
	state* getSide2() const {return mSide2;}
	bso::utilities::geometry::polygon* getGeometry() const;
};

} // namespace dependent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/state/dependent/wall.cpp>

#endif // BSO_BP_WALL_HPP