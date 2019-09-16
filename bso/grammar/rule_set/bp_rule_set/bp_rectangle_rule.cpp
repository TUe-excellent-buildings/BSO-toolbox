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

} // apply()

} // namespace bp_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_BP_RECTANGLE_RULE_CPP