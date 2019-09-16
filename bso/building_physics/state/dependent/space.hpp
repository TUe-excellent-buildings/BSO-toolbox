#ifndef BSO_BP_SPACE_HPP
#define BSO_BP_SPACE_HPP

#include <bso/building_physics/state/dependent/dependent_state.hpp>
#include <bso/building_physics/properties/space_settings.hpp>

#include <bso/utilities/geometry.hpp>

namespace bso { namespace building_physics { namespace state { namespace dependent {

class space : public dependent_state
{
private:
	bso::building_physics::properties::space_settings mSettings;
	
	double mCumulativeHeatingEnergy = 0.0;
	double mCumulativeCoolingEnergy = 0.0;
	
	double mVolume;
	bso::utilities::geometry::polyhedron* mGeometry;
public:
	space(const unsigned int index, bso::utilities::geometry::polyhedron* geometry,
			  const bso::building_physics::properties::space_settings& settings,
			  bso::building_physics::state::state* outside_ptr);
	~space();
	
	void updateSystem(bso::building_physics::state_space_system& system);
	void resetCumulativeEnergies();
	
	const double& getVolume() const {return mVolume;}
	const double& getCumulativeHeatingEnergy() const {return mCumulativeHeatingEnergy;}
	const double& getCumulativeCoolingEnergy() const {return mCumulativeCoolingEnergy;}
	const bso::building_physics::properties::space_settings& getSettings() const {return mSettings;}
	bso::utilities::geometry::polyhedron* getGeometry() const;
};

} // namespace dependent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/state/dependent/space.cpp>

#endif // BSO_BP_SPACE_HPP