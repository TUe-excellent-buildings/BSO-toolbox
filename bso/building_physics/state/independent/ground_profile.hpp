#ifndef BSO_BP_GROUND_PROFILE_HPP
#define BSO_BP_GROUND_PROFILE_HPP

#include <bso/building_physics/state/independent/independent_state.hpp>

namespace bso { namespace building_physics { namespace state { namespace independent {

class ground_profile : public independent_state
{
private:
	double mTemperature;
public:
	ground_profile(const unsigned int& index, const double& temperature);
	~ground_profile();
	
	void initSystem(bso::building_physics::state_space_system& system);
	void updateSystem(bso::building_physics::state_space_system& system);
	
	const double& getTemperature() const {return mTemperature;}
};

} // namespace independent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/state/independent/ground_profile.cpp>

#endif // BSO_BP_GROUND_PROFILE_HPP