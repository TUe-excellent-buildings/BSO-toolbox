#ifndef BSO_GRAMMAR_BP_RECTANGLE_RULE_HPP
#define BSO_GRAMMAR_BP_RECTANGLE_RULE_HPP

#include <bso/grammar/rule_set/rectangle_property.hpp>
#include <bso/grammar/rule_set/bp_rule_set/bp_rule_set.hpp>

namespace bso { namespace grammar { namespace rule_set { namespace bp_rule_set {

class bp_rectangle_rule : public bp_rule_set
{
private:
	rectangle_property* mRectangleProperty;
	
	bso::building_physics::properties::glazing mGlazing;
	bso::building_physics::properties::construction mConstruction;
	
	bool mGlazingAssigned = false;
	bool mConstructionAssigned = false;
	
	std::map<bso::spatial_design::conformal::cf_space*,
		bso::building_physics::state::space*>* mSpaceStates = nullptr;
	bso::building_physics::state::ground_profile* mGroundProfile = nullptr;
	bso::building_physics::state::weather_profile* mWeatherProfile = nullptr;
public:
	bp_rectangle_rule(rectangle_property* rectangleProperty);
	~bp_rectangle_rule();
	
	void assignConstruction(const bso::building_physics::properties::construction& constr);
	void assignGlazing(const bso::building_physics::properties::glazing& glaz);
	
	void assignSpaceStatesMap(std::map<bso::spatial_design::conformal::cf_space*,
		bso::building_physics::state::space*>* spaceStates);
	void assignGroundProfile(bso::building_physics::state::ground_profile* gp);
	void assignWeatherProfile(bso::building_physics::state::weather_profile* wp);
	
	void apply(bso::building_physics::bp_model& bp) const;
};
	
} // namespace bp_rule_set
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/bp_rule_set/bp_rectangle_rule.cpp>

#endif // BSO_GRAMMAR_BP_RECTANGLE_RULE_HPP