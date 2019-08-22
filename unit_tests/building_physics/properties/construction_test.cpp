#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "bp_properties_construction_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/properties/construction.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_properties_test {
using namespace bso::building_physics::properties;

BOOST_AUTO_TEST_SUITE( bp_properties_construction_test )

	BOOST_AUTO_TEST_CASE( test_1 )
	{
		Eigen::Vector4d visProps;
		visProps << 1,2,3,4;
		material m1("mat1","concrete",2400,850,1.8);
		material m2("mat2","insulation",60,850,0.04);
		std::vector<layer> layers = {layer(m2,50),layer(m1,200),layer(m2,50)};
		BOOST_REQUIRE_NO_THROW(construction c1("con1",layers));
		BOOST_REQUIRE_NO_THROW(construction c2("con2",layers,0.5));
		BOOST_REQUIRE_NO_THROW(construction c3("con3",layers,0.5,{"blue",visProps}));
		BOOST_REQUIRE_THROW(construction c4("con4",layers,1.5), std::invalid_argument);
		
		construction c1("con1",layers);
		construction c2("con2",layers,0.6,{"brown",visProps});

		BOOST_REQUIRE(c1.getID() == "con1");
		BOOST_REQUIRE(abs(c1.getCapacitancePerArea()/413.1e3 - 1) < 1e-5);
		BOOST_REQUIRE(abs(c1.getResistanceSide1()/1.30556 - 1) < 1e-5);
		BOOST_REQUIRE(abs(c1.getResistanceSide2()/1.30556 - 1) < 1e-5);
		BOOST_REQUIRE(c1.getVisualizationProperties().first == "");
		BOOST_REQUIRE(c1.getThickness() == 300);
		
		BOOST_REQUIRE(c2.getID() == "con2");
		BOOST_REQUIRE(abs(c2.getCapacitancePerArea()/413.1e3 - 1) < 1e-5);
		BOOST_REQUIRE(abs(c2.getResistanceSide1()/1.32222 - 1) < 1e-5);
		BOOST_REQUIRE(abs(c2.getResistanceSide2()/1.28888 - 1) < 1e-5);
		BOOST_REQUIRE(c2.getVisualizationProperties().first == "brown");
		BOOST_REQUIRE(c2.getThickness() == 300);
		
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_properties_test