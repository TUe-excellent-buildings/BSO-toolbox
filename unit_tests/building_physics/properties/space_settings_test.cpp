#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "bp_properties_space_settings_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/properties/space_settings.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_properties_test {
using namespace bso::building_physics::properties;

BOOST_AUTO_TEST_SUITE( bp_properties_space_settings_test )

	BOOST_AUTO_TEST_CASE( initialization_and_getters )
	{
		BOOST_REQUIRE_NO_THROW(space_settings s1("89",4,6,-8,25,2));
		BOOST_REQUIRE_THROW(space_settings s1("89",4,-6,-8,25,2),std::invalid_argument);
		BOOST_REQUIRE_THROW(space_settings s1("89",-4,6,-8,25,2),std::invalid_argument);
		BOOST_REQUIRE_THROW(space_settings s1("89",4,6,-8,25,-2),std::invalid_argument);
		BOOST_REQUIRE_THROW(space_settings s1("89",4,6,8,-2,2),std::invalid_argument);
		BOOST_REQUIRE_THROW(space_settings s1("89",4,6,8,8.5,2),std::invalid_argument);
		space_settings s1("767",4.5,6.7,24,25,2.3);
		
		BOOST_REQUIRE(s1.getID() == "767");
		BOOST_REQUIRE(s1.getHeatingCapacity() == 4.5);
		BOOST_REQUIRE(s1.getCoolingCapacity() == 6.7);
		BOOST_REQUIRE(s1.getHeatingSetPoint() == 24);
		BOOST_REQUIRE(s1.getCoolingSetPoint() == 25);
		BOOST_REQUIRE(s1.getACH() == 2.3);
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_properties_test