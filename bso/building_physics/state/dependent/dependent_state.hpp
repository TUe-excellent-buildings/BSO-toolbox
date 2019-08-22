#ifndef BSO_BP_DEPENDENT_STATE_HPP
#define BSO_BP_DEPENDENT_STATE_HPP

#include <bso/building_physics/state/state.hpp>

namespace bso { namespace building_physics { namespace state { namespace dependent {

class dependent_state : public state
{
protected:
	std::map<state*, double> mAdjacentStates; // adjacent state and the resistance to a heat flux to that state
	double mCapacitance;

public:
	dependent_state(const unsigned int& index);
	virtual ~dependent_state() = 0;

	virtual void initSystem(bso::building_physics::state_space_system& system);
	virtual void updateSystem(bso::building_physics::state_space_system& system);
	virtual void addAdjacentState(state* adjacentState, const double& resistance);
	
	virtual const double& getCapacitance() const {return mCapacitance;}
};

} // namespace dependent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/state/dependent/dependent_state.cpp>

#endif // BSO_BP_DEPENDENT_STATE_HPP