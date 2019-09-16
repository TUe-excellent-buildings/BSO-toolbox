#ifndef BSO_BP_WINDOW_HPP
#define BSO_BP_WINDOW_HPP

#include <bso/building_physics/state/dependent/dependent_state.hpp>
#include <bso/building_physics/properties/glazing.hpp>

#include <bso/utilities/geometry.hpp>

namespace bso { namespace building_physics { namespace state { namespace dependent {

class window : public dependent_state
{
private:
	bso::building_physics::properties::glazing mGlazing;
	state* mSide1;
	state* mSide2;
	
	double mArea;
	bso::utilities::geometry::polygon* mGeometry;
public:
	window(const unsigned int& index,  bso::utilities::geometry::polygon* geometry,
				 const bso::building_physics::properties::glazing& glazing,
				 state* side1, state* side2);
	~window();
	
	const std::pair<std::string,Eigen::Vector4d>& getVisualizationProperties() const {return mGlazing.getVisualizationProperties();}
	const double& getArea() const {return mArea;}
	const bso::building_physics::properties::glazing& getGlazing() const {return mGlazing;}
	state* getSide1() const {return mSide1;}
	state* getSide2() const {return mSide2;}
	bso::utilities::geometry::polygon* getGeometry() const;
};

} // namespace dependent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/state/dependent/window.cpp>

#endif // BSO_BP_WINDOW_HPP