#ifndef BSO_BP_WALL_CPP
#define BSO_BP_WALL_CPP

namespace bso { namespace building_physics { namespace state { namespace dependent {

wall::wall(const unsigned int& index,  bso::utilities::geometry::polygon* geometry,
	const bso::building_physics::properties::construction& construction,
	state* side1, state* side2)
: dependent_state(index), mGeometry(geometry->clone()), mArea(geometry->getArea()*1e-6),
	mConstruction(construction), mSide1(side1), mSide2(side2)
{
	mIsWall = true;
	mCapacitance = mConstruction.getCapacitancePerArea()*mArea;

	double transResistanceSide1, transResistanceSide2;
	if (mSide1->isSpace()) transResistanceSide1 = 0.13;
	else if (mSide1->isWeatherProfile()) transResistanceSide1 = 0.04;
	else if (mSide1->isGroundProfile()) transResistanceSide1 = 0.0;
	else
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to assign transition resistance to side 1\n"
								 << "of a wall but could not recognize the type of the adjacent state\n"
								 << "(bso/building_physics/state/dependent/wall.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	if (mSide2->isSpace()) transResistanceSide2 = 0.13;
	else if (mSide2->isWeatherProfile()) transResistanceSide2 = 0.04;
	else if (mSide2->isGroundProfile()) transResistanceSide2 = 0.0;
	else
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to assign transition resistance to side 2\n"
								 << "of a wall but could not recognize the type of the adjacent state\n"
								 << "(bso/building_physics/state/dependent/wall.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	
	mAdjacentStates[mSide1] = (mConstruction.getResistanceSide1() + transResistanceSide1) / mArea;
	mAdjacentStates[mSide2] = (mConstruction.getResistanceSide2() + transResistanceSide2) / mArea;
	
	for (const auto& i : mAdjacentStates)
	{
		if (!(i.first->isDependent())) continue;
		auto tempDepStatePtr = dynamic_cast<dependent_state*>(i.first);
		tempDepStatePtr->addAdjacentState(this, i.second);
	}
	
} // ctor()

wall::~wall()
{
	delete mGeometry;
} // dtor()

bso::utilities::geometry::polygon* wall::getGeometry() const
{
	try
	{
		mGeometry->end();
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, requesting geometry pointer from wall, but it does not\n"
								 << "seem to exist anymore. Was it deleted?\n"
								 << "(bso/building_physics/state/dependent/wall.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	return mGeometry;
}

} // namespace dependent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_WALL_CPP