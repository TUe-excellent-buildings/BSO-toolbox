#ifndef BSO_BP_INDEPENDENT_STATE_CPP
#define BSO_BP_INDEPENDENT_STATE_CPP

namespace bso { namespace building_physics { namespace state { namespace independent {

independent_state::independent_state(const unsigned int& index)
: state(index)
{
	mIsIndependent = true;
} // ctor()

independent_state::~independent_state()
{
	
} // dtor()

} // namespace independent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_INDEPENDENT_STATE_CPP