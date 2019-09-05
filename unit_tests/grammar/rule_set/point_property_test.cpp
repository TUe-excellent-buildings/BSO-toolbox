#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_point_property_test"
#endif

#include <bso/grammar/rule_set/point_property.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_point_property_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	
	bso::utilities::geometry::vertex point1 = {{0,0,0}};
	bso::spatial_design::conformal::cf_point* pointPtr1;
	for (const auto& i : cfModel.cfPoints())
	{
		if (i->isSameAs(point1)) pointPtr1 = i;
	}
	BOOST_REQUIRE_NO_THROW(point_property pp1(pointPtr1));
	point_property pp1(pointPtr1);
	BOOST_REQUIRE(pp1.getPointPtr() == pointPtr1);
	
	BOOST_REQUIRE(!pp1.isVertexProperty());
	BOOST_REQUIRE(!pp1.isLineProperty());
	BOOST_REQUIRE(!pp1.isRectangleProperty());
	BOOST_REQUIRE(!pp1.isCuboidProperty());
	BOOST_REQUIRE( pp1.isPointProperty());
	BOOST_REQUIRE(!pp1.isEdgeProperty());
	BOOST_REQUIRE(!pp1.isSurfaceProperty());
	BOOST_REQUIRE(!pp1.isSpaceProperty());
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_rule_set_test