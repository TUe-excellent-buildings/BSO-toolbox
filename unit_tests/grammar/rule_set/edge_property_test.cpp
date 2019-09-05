#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_edge_property_test"
#endif

#include <bso/grammar/rule_set/edge_property.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_edge_property_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	
	bso::utilities::geometry::line_segment edge1 = {{0,0,0},{3000,0,0}};
	bso::spatial_design::conformal::cf_edge* edgePtr1;
	for (const auto& i : cfModel.cfEdges())
	{
		if (i->isSameAs(edge1)) edgePtr1 = i;
	}
	BOOST_REQUIRE_NO_THROW(edge_property ep1(edgePtr1));
	edge_property ep1(edgePtr1);
	BOOST_REQUIRE(ep1.getEdgePtr() == edgePtr1);
	
	BOOST_REQUIRE(!ep1.isVertexProperty());
	BOOST_REQUIRE(!ep1.isLineProperty());
	BOOST_REQUIRE(!ep1.isRectangleProperty());
	BOOST_REQUIRE(!ep1.isCuboidProperty());
	BOOST_REQUIRE(!ep1.isPointProperty());
	BOOST_REQUIRE( ep1.isEdgeProperty());
	BOOST_REQUIRE(!ep1.isSurfaceProperty());
	BOOST_REQUIRE(!ep1.isSpaceProperty());
}

BOOST_AUTO_TEST_CASE( is_vertical_or_horizontal )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	
	bso::utilities::geometry::line_segment edge1 = {{0,0,0},{3000,0,0}};
	bso::utilities::geometry::line_segment edge2 = {{0,0,0},{0,0,3000}};
	bso::spatial_design::conformal::cf_edge* edgePtr1,* edgePtr2;
	for (const auto& i : cfModel.cfEdges())
	{
		if (i->isSameAs(edge1)) edgePtr1 = i;
		if (i->isSameAs(edge2)) edgePtr2 = i;
	}

	edge_property ep1(edgePtr1);
	edge_property ep2(edgePtr2);
	
	BOOST_REQUIRE( ep1.isHorizontal());
	BOOST_REQUIRE(!ep2.isHorizontal());
	
	BOOST_REQUIRE(!ep1.isVertical());
	BOOST_REQUIRE( ep2.isVertical());
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_rule_set_test