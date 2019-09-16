#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_tests"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/spatial_design/ms_building.hpp>
#include <bso/spatial_design/cf_building.hpp>
#include <bso/structural_design/sd_model.hpp>
#include <bso/building_physics/bp_model.hpp>
#include <bso/grammar/grammar.hpp>

#include <unit_tests/grammar/rule_set/vertex_property_test.cpp>
#include <unit_tests/grammar/rule_set/line_property_test.cpp>
#include <unit_tests/grammar/rule_set/rectangle_property_test.cpp>
#include <unit_tests/grammar/rule_set/cuboid_property_test.cpp>

#include <unit_tests/grammar/rule_set/point_property_test.cpp>
#include <unit_tests/grammar/rule_set/edge_property_test.cpp>
#include <unit_tests/grammar/rule_set/surface_property_test.cpp>
#include <unit_tests/grammar/rule_set/space_property_test.cpp>

#include <unit_tests/grammar/rule_set/sd_rule_set/sd_vertex_rule_test.cpp>
#include <unit_tests/grammar/rule_set/sd_rule_set/sd_line_rule_test.cpp>
#include <unit_tests/grammar/rule_set/sd_rule_set/sd_rectangle_rule_test.cpp>

#include <unit_tests/grammar/rule_set/bp_rule_set/bp_rectangle_rule_test.cpp>
#include <unit_tests/grammar/rule_set/bp_rule_set/bp_space_rule_test.cpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_test {
using namespace bso::grammar;

BOOST_AUTO_TEST_SUITE( grammar_test )

	BOOST_AUTO_TEST_CASE( initialization )
	{
		bso::spatial_design::ms_building ms("grammar/ms_test_1.txt");
		bso::spatial_design::cf_building cf(ms);

		bso::grammar::grammar gram(cf);
	}

BOOST_AUTO_TEST_SUITE_END()

#include <unit_tests/grammar/sd_grammars/default_sd_grammar_test.cpp>
#include <unit_tests/grammar/bp_grammars/default_bp_grammar_test.cpp>
} // namespace grammar_test