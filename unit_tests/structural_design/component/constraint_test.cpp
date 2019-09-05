#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_constraint_component_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/component/constraint.hpp>

#include <sstream>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace component_test {
using namespace bso::structural_design::component;

BOOST_AUTO_TEST_SUITE( sd_constraint_component )
	
	BOOST_AUTO_TEST_CASE( constraint_test )
	{
		constraint c1(0);
		BOOST_REQUIRE(c1.DOF() == 0);

		BOOST_REQUIRE_THROW(constraint c2(-1), std::invalid_argument);
		BOOST_REQUIRE_THROW(constraint c2( 6), std::invalid_argument);
		BOOST_REQUIRE_NO_THROW(constraint c2(0));
		BOOST_REQUIRE_NO_THROW(constraint c2(3));
		BOOST_REQUIRE_NO_THROW(constraint c2(5));
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace component_test