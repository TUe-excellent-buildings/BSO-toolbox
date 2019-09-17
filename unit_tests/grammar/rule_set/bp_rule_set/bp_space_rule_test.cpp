#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_bp_space_rule_set_test"
#endif

#include <bso/grammar/rule_set/bp_rule_set/bp_space_rule.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_bp_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_bp_space_rule_set_test )

BOOST_AUTO_TEST_CASE( initialization_and_application )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	auto spacePtr = cfModel.cfSpaces().front();
	space_property spaceProp(spacePtr);
	
	BOOST_REQUIRE_NO_THROW(bp_space_rule bpsr1(&spaceProp));
	bp_space_rule bpsr1(&spaceProp);
	
	// assign space settings
	bso::building_physics::properties::space_settings spaceSettings(
		"space setting 1",4.5,6.7,24,25,2.3);
	bpsr1.assignSettings(spaceSettings);
	
	// assign SpaceStatesMap
	std::map<bso::spatial_design::conformal::cf_space*,
		bso::building_physics::state::space*> spaceStatesMap;
	bpsr1.assignSpaceStatesMap(&spaceStatesMap);
	
	// assign weather profile
	bso::building_physics::state::weather_profile wp(1);
	bpsr1.assignWeatherProfile(&wp);
	
	// apply
	bso::building_physics::bp_model bp;
	bpsr1.apply(bp);
	
	BOOST_REQUIRE(bp.getDependentStates().size() == 1);
	BOOST_REQUIRE(bp.getIndependentStates().size() == 0);
	BOOST_REQUIRE(bp.getSpaces().size() == 1);
	BOOST_REQUIRE(bp.getWalls().size() == 0);
	BOOST_REQUIRE(bp.getWindows().size() == 0);
	BOOST_REQUIRE(bp.getFloors().size() == 0);
	BOOST_REQUIRE(abs(bp.getSpaces().front()->getCapacitance()/97.2e3 -1.0) < 1e-9);
}

BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_bp_rule_set_test