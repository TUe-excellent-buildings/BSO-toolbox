#ifndef BSO_GRAMMAR_BP_SPACE_RULE_HPP
#define BSO_GRAMMAR_BP_SPACE_RULE_HPP

#include <bso/grammar/rule_set/rectangle_property.hpp>
#include <bso/grammar/rule_set/bp_rule_set/bp_rule_set.hpp>

namespace bso { namespace grammar { namespace rule_set { namespace bp_rule_set {

class bp_space_rule : public bp_rule_set
{
private:
	space_property* mSpaceProperty;
	bool mSpaceSettingsAssigned = false;
	bso::building_physics::properties::space_settings mSpaceSettings;
	std::map<bso::spatial_design::conformal::cf_space*,
		bso::building_physics::state::space*>* mSpaceStates = nullptr;
	bso::building_physics::state::weather_profile* mWeatherProfile = nullptr;
public:
	bp_space_rule(space_property* spaceProperty);
	~bp_space_rule();
	
	void assignSettings(const bso::building_physics::properties::space_settings& settings);

	void assignSpaceStatesMap(std::map<bso::spatial_design::conformal::cf_space*,
		bso::building_physics::state::space*>* spaceStates);
	void assignWeatherProfile(bso::building_physics::state::weather_profile* wp);
	
	void apply(bso::building_physics::bp_model& bp) const;
};
	
} // namespace bp_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/bp_rule_set/bp_space_rule.cpp>

#endif // BSO_GRAMMAR_BP_SPACE_RULE_HPP