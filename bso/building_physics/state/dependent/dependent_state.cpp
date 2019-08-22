#ifndef BSO_BP_DEPENDENT_STATE_CPP
#define BSO_BP_DEPENDENT_STATE_CPP

#include <sstream>
#include <stdexcept>

namespace bso { namespace building_physics { namespace state { namespace dependent {

dependent_state::dependent_state(const unsigned int& index) 
: state(index)
{
	mIsDependent = true;
} // ctor()

dependent_state::~dependent_state()
{

} // dtor()

void dependent_state::initSystem(bso::building_physics::state_space_system& system)
{
	for (const auto& i : mAdjacentStates)
	{
		double fluxValue = 1/(mCapacitance*i.second);
		if (i.first->isDependent())
		{
			system.getA()(this->getIndex(),this->getIndex())    += -fluxValue;
			system.getA()(this->getIndex(),i.first->getIndex()) +=  fluxValue;
		}
		else if (i.first->isIndependent())
		{
			system.getA()(this->getIndex(),this->getIndex())    += -fluxValue;
			system.getB()(this->getIndex(),i.first->getIndex()) +=  fluxValue;
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError when updating state space system with a state's contribution\n" 
									 <<  "Did not recognize type of the state\n"
									 << "(bso/building_physics/state/dependent/dependent_state.hpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	}
} // initSystem()

void dependent_state::updateSystem(bso::building_physics::state_space_system& system)
{
	// empty by default
} // updateSystem()

void dependent_state::addAdjacentState(state* adjacentState, const double& resistance)
{
	mAdjacentStates[adjacentState] = resistance;
} // addAdjacentState

} // namespace dependent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_DEPENDENT_STATE_CPP