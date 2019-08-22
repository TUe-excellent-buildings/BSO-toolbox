#ifndef BSO_BP_INDEPENDENT_STATE_HPP
#define BSO_BP_INDEPENDENT_STATE_HPP

#include <bso/building_physics/state/state.hpp>

namespace bso { namespace building_physics { namespace state { namespace independent {

class independent_state : public state
{
private:
	
public:
	independent_state(const unsigned int& index);
	virtual ~independent_state() = 0;
};

} // namespace independent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/state/independent/independent_state.cpp>

#endif // BSO_BP_INDEPENDENT_STATE_HPP