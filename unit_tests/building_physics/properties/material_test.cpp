#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "bp_properties_material_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/properties/material.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_properties_test {
using namespace bso::building_physics::properties;

BOOST_AUTO_TEST_SUITE( bp_properties_material_test )

	BOOST_AUTO_TEST_CASE( initialize_and_getters )
	{
		BOOST_REQUIRE_NO_THROW(material m1("1","test_material",2.5,3.6,4.7));
		BOOST_REQUIRE_THROW(material m2("mat2","test_material",-2.5,3.6,4.7), std::invalid_argument);
		BOOST_REQUIRE_THROW(material m2("mat2","test_material",2.5,-3.6,4.7), std::invalid_argument);
		BOOST_REQUIRE_THROW(material m2("mat2","test_material",2.5,3.6,-4.7), std::invalid_argument);
		material m1("1","test_material",2.5,3.6,4.7);
		
		BOOST_REQUIRE(m1.getID() == "1");
		BOOST_REQUIRE(m1.getDescription() == "test_material");
		BOOST_REQUIRE(m1.getSpecificWeight() == 2.5);
		BOOST_REQUIRE(m1.getSpecificHeat() == 3.6);
		BOOST_REQUIRE(m1.getThermalConductivity() == 4.7);
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_properties_test