#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_bp_rectangle_rule_set_test"
#endif

#include <bso/grammar/rule_set/bp_rule_set/bp_rectangle_rule.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_bp_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_bp_rectangle_rule_set_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr = geomModel->addRectangle({{0,0,0},{3000,0,0},{3000,0,3000},{0,0,3000}});
	rectangle_property rp1(rectPtr);
	
	BOOST_REQUIRE_NO_THROW(bp_rectangle_rule bpr1(&rp1));
	bp_rectangle_rule bpr1(&rp1);
	
	// assign space states
	auto spacePtr = rectPtr->cfSurfaces().front()->cfSpaces().front();
	bso::building_physics::properties::space_settings
		spaceSetting("space_setting_1",100,150,20,25,1.0);
	bso::building_physics::state::weather_profile wp(1);
	bso::building_physics::state::dependent::space bpSpace(1,spacePtr,spaceSetting,&wp);
	std::map<bso::spatial_design::conformal::cf_space*,
		bso::building_physics::state::space*> spaceStatesMap;
	spaceStatesMap.emplace(spacePtr,&bpSpace);
	bpr1.assignSpaceStatesMap(&spaceStatesMap);
	
	// assign weather profile
	bpr1.assignWeatherProfile(&wp);
	
	bso::building_physics::bp_model bp;
	bpr1.apply(bp);
	BOOST_REQUIRE(bp.getDependentStates().size() == 0);
	BOOST_REQUIRE(bp.getIndependentStates().size() == 0);
}

BOOST_AUTO_TEST_CASE( apply_rule_1 )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr = geomModel->addRectangle({{0,0,0},{3000,0,0},{3000,0,3000},{0,0,3000}});
	rectangle_property rp1(rectPtr);
	bp_rectangle_rule bpr1(&rp1);
	
	// assign glazing
	Eigen::Vector4d visProps;
	visProps << 1,2,3,4;
	bso::building_physics::properties::glazing g1("34",3.5,6.8,{"blue",visProps});
	bpr1.assignGlazing(g1);
	
	// assign space states
	auto spacePtr = rectPtr->cfSurfaces().front()->cfSpaces().front();
	bso::building_physics::properties::space_settings
		spaceSetting("space_setting_1",100,150,20,25,1.0);
	bso::building_physics::state::weather_profile wp(1);
	bso::building_physics::state::dependent::space bpSpace(1,spacePtr,spaceSetting,&wp);
	std::map<bso::spatial_design::conformal::cf_space*,
		bso::building_physics::state::space*> spaceStatesMap;
	spaceStatesMap.emplace(spacePtr,&bpSpace);
	bpr1.assignSpaceStatesMap(&spaceStatesMap);
	
	// assign weather profile
	bpr1.assignWeatherProfile(&wp);
	
	// apply rule
	bso::building_physics::bp_model bp;
	bpr1.apply(bp);

	BOOST_REQUIRE(bp.getDependentStates().size() == 1);
	BOOST_REQUIRE(bp.getIndependentStates().size() == 0);
	BOOST_REQUIRE(bp.getSpaces().size() == 0);
	BOOST_REQUIRE(bp.getWalls().size() == 0);
	BOOST_REQUIRE(bp.getWindows().size() == 1);
	BOOST_REQUIRE(bp.getFloors().size() == 0);
	BOOST_REQUIRE(abs(bp.getWindows().front()->getCapacitance()/61.2 -1.0) < 1e-9);
}

BOOST_AUTO_TEST_CASE( apply_rule_2 )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr = geomModel->addRectangle({{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0}});
	rectangle_property rp1(rectPtr);
	bp_rectangle_rule bpr1(&rp1);
	
	// assign construction
	Eigen::Vector4d visProps;
	visProps << 1,2,3,4;
	bso::building_physics::properties::material m1("mat1","concrete",2400,850,1.8);
	bso::building_physics::properties::material m2("mat2","insulation",60,850,0.04);
	std::vector<bso::building_physics::properties::layer> layers = 
		{bso::building_physics::properties::layer(m2,50),
			bso::building_physics::properties::layer(m1,200),
			bso::building_physics::properties::layer(m2,50)};
	bso::building_physics::properties::construction constr1("constr1",layers,0.5,{"blue",visProps});
	bpr1.assignConstruction(constr1);
	
	// assign space states
	auto spacePtr = rectPtr->cfSurfaces().front()->cfSpaces().front();
	bso::building_physics::properties::space_settings
		spaceSetting("space_setting_1",100,150,20,25,1.0);
	bso::building_physics::state::weather_profile wp(1);
	bso::building_physics::state::dependent::space bpSpace(1,spacePtr,spaceSetting,&wp);
	std::map<bso::spatial_design::conformal::cf_space*,
		bso::building_physics::state::space*> spaceStatesMap;
	spaceStatesMap.emplace(spacePtr,&bpSpace);
	bpr1.assignSpaceStatesMap(&spaceStatesMap);
	
	// assign ground profile
	bso::building_physics::state::ground_profile gp(1,10.0);
	bpr1.assignGroundProfile(&gp);
	
	// apply rule
	bso::building_physics::bp_model bp;
	bpr1.apply(bp);
	
	BOOST_REQUIRE(bp.getDependentStates().size() == 1);
	BOOST_REQUIRE(bp.getIndependentStates().size() == 0);
	BOOST_REQUIRE(bp.getSpaces().size() == 0);
	BOOST_REQUIRE(bp.getWalls().size() == 0);
	BOOST_REQUIRE(bp.getWindows().size() == 0);
	BOOST_REQUIRE(bp.getFloors().size() == 1);
	BOOST_REQUIRE(abs(bp.getFloors().front()->getCapacitance()/3.7179e6 -1.0) < 1e-9);
}

BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_bp_rule_set_test