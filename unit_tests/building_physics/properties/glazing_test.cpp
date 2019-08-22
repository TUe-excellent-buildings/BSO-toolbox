#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "bp_properties_glazing_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/properties/glazing.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_properties_test {
using namespace bso::building_physics::properties;

BOOST_AUTO_TEST_SUITE( bp_properties_glazing_test )

	BOOST_AUTO_TEST_CASE( initialization_and_getters )
	{
		Eigen::Vector4d visProps;
		visProps << 1,2,3,4;
		BOOST_REQUIRE_NO_THROW(glazing g1("34",3.5,6.8));
		BOOST_REQUIRE_NO_THROW(glazing g1("34",3.5,6.8,{"brown",visProps}));
		BOOST_REQUIRE_THROW(glazing g1("12",-10,50), std::invalid_argument);
		BOOST_REQUIRE_THROW(glazing g1("12",10,-50), std::invalid_argument);
		glazing g1("34",3.5,6.8,{"blue",visProps});
		glazing g2("56",7.5,7.8);
		
		BOOST_REQUIRE(g1.getID() == "34");
		BOOST_REQUIRE(g1.getVisualizationProperties().first == "blue");
		BOOST_REQUIRE(g2.getVisualizationProperties().first == "");
		BOOST_REQUIRE(g1.getU() == 3.5);
		BOOST_REQUIRE(g1.getCapacitancePerArea() == 6.8);
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_properties_test