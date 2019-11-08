#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "default_bp_grammar_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/spatial_design/ms_building.hpp>
#include <bso/spatial_design/cf_building.hpp>
#include <bso/building_physics/bp_model.hpp>
#include <bso/grammar/grammar.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_test {
using namespace bso::grammar;

BOOST_AUTO_TEST_SUITE( grammar_default_bp_test )

	BOOST_AUTO_TEST_CASE( default_bp_grammar_test_1 )
	{
		bso::spatial_design::ms_building ms("grammar/ms_test_1.txt");
		bso::spatial_design::cf_building cf(ms);

		bso::grammar::grammar gram(cf);
		bso::building_physics::bp_model bp = gram.bp_grammar<>(std::string("grammar/bp_settings_1.txt"),true);
		
		BOOST_REQUIRE(bp.getStates().size() == 9);
		BOOST_REQUIRE(bp.getIndependentStates().size() == 2);
		BOOST_REQUIRE(bp.getDependentStates().size() == 7);
		BOOST_REQUIRE(bp.getSpaces().size() == 1);
		BOOST_REQUIRE(bp.getWalls().size() == 3);
		BOOST_REQUIRE(bp.getFloors().size() == 2);
		BOOST_REQUIRE(bp.getWindows().size() == 1);
	}


BOOST_AUTO_TEST_SUITE_END()

} // namespace grammar_test