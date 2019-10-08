#ifndef BSO_BP_WINDOW_CPP
#define BSO_BP_WINDOW_CPP

namespace bso { namespace building_physics { namespace state { namespace dependent {

window::window(const unsigned int& index,  bso::utilities::geometry::polygon* geometry,
	const bso::building_physics::properties::glazing& glazing,
	state* side1, state* side2)
: dependent_state(index), mGeometry(geometry->clone()), mArea(geometry->getArea()*1e-6),
	mGlazing(glazing), mSide1(side1), mSide2(side2)
{
	mIsWindow = true;
	mCapacitance = mGlazing.getCapacitancePerArea()*mArea;
	
	mAdjacentStates[mSide1] = 1/(2*mGlazing.getU()*mArea);
	mAdjacentStates[mSide2] = 1/(2*mGlazing.getU()*mArea);
	
	for (const auto& i : mAdjacentStates)
	{
		if (!(i.first->isDependent())) continue;
		auto tempDepStatePtr = dynamic_cast<dependent_state*>(i.first);
		tempDepStatePtr->addAdjacentState(this, i.second);
	}
	
} // ctor()

window::~window()
{
	delete mGeometry;
} // dtor()

bso::utilities::geometry::polygon* window::getGeometry() const
{
	try
	{
		mGeometry->end();
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, requesting geometry pointer from window, but it does not\n"
								 << "seem to exist anymore. Was it deleted?\n"
								 << "(bso/building_physics/state/dependent/window.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	return mGeometry;
}

} // namespace dependent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_WINDOW_CPP