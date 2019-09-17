#ifndef BSO_GRAMMAR_BP_RECTANGLE_RULE_CPP
#define BSO_GRAMMAR_BP_RECTANGLE_RULE_CPP

namespace bso { namespace grammar { namespace rule_set { namespace bp_rule_set {

bp_rectangle_rule::bp_rectangle_rule(rectangle_property* rectangleProperty)
:	mConstruction(bso::building_physics::properties::construction("",{})),
	mGlazing(bso::building_physics::properties::glazing("",1,1))
{
	mRectangleProperty = rectangleProperty;
} // ctor()

bp_rectangle_rule::~bp_rectangle_rule()
{
	
} // dtor()

void bp_rectangle_rule::assignConstruction(const bso::building_physics::properties::construction& constr)
{
	mConstructionAssigned = true;
	mConstruction = constr;
} // assignConstruction()

void bp_rectangle_rule::assignGlazing(const bso::building_physics::properties::glazing& glaz)
{
	mGlazingAssigned = true;
	mGlazing = glaz;
} // assignGlazing()

void bp_rectangle_rule::assignSpaceStatesMap(std::map<bso::spatial_design::conformal::cf_space*,
		bso::building_physics::state::space*>* spaceStates)
{
	mSpaceStates = spaceStates;
} // assignWeatherProfile()

void bp_rectangle_rule::assignGroundProfile(bso::building_physics::state::ground_profile* gp)
{
	mGroundProfile = gp;
} // assignWeatherProfile()


void bp_rectangle_rule::assignWeatherProfile(bso::building_physics::state::weather_profile* wp)
{
	mWeatherProfile = wp;
} // assignWeatherProfile()

void bp_rectangle_rule::apply(bso::building_physics::bp_model& bp) const
{
	bso::building_physics::state::state* side1 = nullptr, * side2 = nullptr;
	try
	{
		if (mRectangleProperty->isSpaceSeparating() && mRectangleProperty->isExternal())
		{
			auto spaceSearch = mSpaceStates->find(
			mRectangleProperty->getRectanglePtr()->cfSurfaces().front()->cfSpaces().front());
			if (spaceSearch == mSpaceStates->end()) throw std::runtime_error(
				"Could not find the matching bpSpace for a cfSpace");
			side1 = spaceSearch->second;
			if (mRectangleProperty->isAboveGroundSurface())
			{
				side2 = mWeatherProfile;
			}
			else
			{
				side2 = mGroundProfile;
			}
		}
		else if (mRectangleProperty->isSpaceSeparating())
		{
			auto spaceSearch = mSpaceStates->find(
			mRectangleProperty->getRectanglePtr()->cfSurfaces().front()->cfSpaces().front());
			if (spaceSearch == mSpaceStates->end()) throw std::runtime_error(
				"Could not find the matching bpDpace for a cfSpace");
			side1 = spaceSearch->second;
			spaceSearch = mSpaceStates->find(
			mRectangleProperty->getRectanglePtr()->cfSurfaces().back()->cfSpaces().front());
			if (spaceSearch == mSpaceStates->end()) throw std::runtime_error(
				"Could not find the matching bpDpace for a cfSpace");
			side2 = spaceSearch->second;
		}
		if (side1 == nullptr) throw std::runtime_error("Did not find state pointer for side 1");
		if (side2 == nullptr) throw std::runtime_error("Did not find state pointer for side 2");
		
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, when applying a bp rectangle rule, received the"
								 << "following exception: \n" << e.what() << "\n"
								 << "(bso/grammar/rule_set/bp_rule_set/bp_rectangle_rule_set.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	
	if (mConstructionAssigned)
	{
		if (mRectangleProperty->isFloor())
		{ // create a bpFloor
	
			bp.addState(new bso::building_physics::state::floor(
				bp.getNextDependentIndex(), mRectangleProperty->getRectanglePtr(),
				mConstruction, side1, side2));
		}
		else if (mRectangleProperty->isWall())
		{ // create a bpWall
	
			bp.addState(new bso::building_physics::state::wall(
				bp.getNextDependentIndex(), mRectangleProperty->getRectanglePtr(),
				mConstruction, side1, side2));
		}
	}
	else if (mGlazingAssigned)
	{
		if (mRectangleProperty->isFloor())
		{ // throw error
			std::stringstream errorMessage;
			errorMessage << "\nError, while applying a bp rectangle rule,\n"
									 << "trying to apply a glazing to a floor"
									 << "(bso/grammar/rule_set/bp_rule_set/bp_rectangle_rule_set.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		else if (mRectangleProperty->isWall())
		{ // create a bpWindow
			if (!mRectangleProperty->isAboveGroundSurface())
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, while applying a bp rectangle rule,\n"
										 << "trying to apply a glazing to a wall that is not\n"
										 << "above the ground surface\n"
										 << "(bso/grammar/rule_set/bp_rule_set/bp_rectangle_rule_set.cpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
			bp.addState(new bso::building_physics::state::window(
				bp.getNextDependentIndex(), mRectangleProperty->getRectanglePtr(),
				mGlazing, side1, side2));
		}
	}
	else { } // nothing happens

} // apply()

} // namespace bp_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_BP_RECTANGLE_RULE_CPP