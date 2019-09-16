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
	
}

BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_bp_rule_set_test