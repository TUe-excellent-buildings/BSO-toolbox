#ifndef BSO_BP_GROUND_PROFILE_CPP
#define BSO_BP_GROUND_PROFILE_CPP

namespace bso { namespace building_physics { namespace state { namespace independent {

ground_profile::ground_profile(const unsigned int& index, const double& temperature)
: independent_state(index), mTemperature(temperature)
{
	mIsGroundProfile = true;
} // ctor() 

ground_profile::~ground_profile()
{
	
} // dtor()

void ground_profile::initSystem(bso::building_physics::state_space_system& system)
{
	system.getu()(mIndex) = mTemperature;
} // initSystem

void ground_profile::updateSystem(bso::building_physics::state_space_system& system)
{
	// nothing to update after initialization
} // updateSystem


} // namespace independent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_GROUND_PROFILE_CPP