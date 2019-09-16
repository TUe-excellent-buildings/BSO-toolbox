#ifndef BSO_GRAMMAR_BP_SPACE_RULE_CPP
#define BSO_GRAMMAR_BP_SPACE_RULE_CPP

namespace bso { namespace grammar { namespace rule_set { namespace bp_rule_set {

bp_space_rule::bp_space_rule(space_property* spaceProperty)
:	mSpaceSettings(bso::building_physics::properties::space_settings("",0,0,0,1,0))
{
	mSpaceProperty = spaceProperty;
} // ctor()

bp_space_rule::~bp_space_rule()
{
	
} // dtor()

void bp_space_rule::assignSettings(const bso::building_physics::properties::space_settings& settings)
{
	mSpaceSettings = settings;
} // assignSettings()

void bp_space_rule::assignSpaceStatesMap(std::map<bso::spatial_design::conformal::cf_space*,
	bso::building_physics::state::space*>* spaceStates)
{
	mSpaceSettingsAssigned = true;
	mSpaceStates = spaceStates;
} // assignSpaceStatesMap()

void bp_space_rule::assignWeatherProfile(bso::building_physics::state::weather_profile* wp)
{
	mWeatherProfile = wp;
} // assignWeatherProfile()

void bp_space_rule::apply(bso::building_physics::bp_model& bp) const
{
	if (mSpaceStates == nullptr || mWeatherProfile == nullptr || !mSpaceSettingsAssigned)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, when applying a bp space rule set,\n"
								 << "missing some data, cannot create the bp space state\n"
								 << "(bso/grammar/rule_set/bp_rule_set.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	auto spacePtr = new bso::building_physics::state::space(bp.getNextDependentIndex(),
		mSpaceProperty->getSpacePtr(), mSpaceSettings, mWeatherProfile);
	mSpaceStates->emplace(mSpaceProperty->getSpacePtr(), spacePtr);
	bp.addState(spacePtr);
} // apply()
	
} // namespace bp_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_BP_SPACE_RULE_CPP