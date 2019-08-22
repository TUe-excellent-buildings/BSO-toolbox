#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "bp_properties_layer_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/properties/layer.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_properties_test {
using namespace bso::building_physics::properties;

BOOST_AUTO_TEST_SUITE( bp_properties_layer_test )

	BOOST_AUTO_TEST_CASE( initialization_and_getters )
	{
		material m1("1234","test_material",1,2,3);
		BOOST_REQUIRE_NO_THROW(layer l1(m1, 100));
		BOOST_REQUIRE_THROW(layer l2(m1, -100), std::invalid_argument);
		layer l1(m1, 100);
		
		BOOST_REQUIRE(l1.getThickness() == 100);
		BOOST_REQUIRE(l1.getMaterial().getID() == "1234");
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_properties_test