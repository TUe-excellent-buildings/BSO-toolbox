#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_space_property_test"
#endif

#include <bso/grammar/rule_set/space_property.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_space_property_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	
	bso::utilities::geometry::quad_hexahedron space1 = {
		{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0},
		{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}};
	bso::spatial_design::conformal::cf_space* spacePtr1;
	for (const auto& i : cfModel.cfSpaces())
	{
		if (i->isSameAs(space1)) spacePtr1 = i;
	}
	BOOST_REQUIRE_NO_THROW(space_property sp1(spacePtr1));
	space_property sp1(spacePtr1);
	BOOST_REQUIRE(sp1.getSpacePtr() == spacePtr1);
	
	BOOST_REQUIRE(!sp1.isVertexProperty());
	BOOST_REQUIRE(!sp1.isLineProperty());
	BOOST_REQUIRE(!sp1.isRectangleProperty());
	BOOST_REQUIRE(!sp1.isCuboidProperty());
	BOOST_REQUIRE(!sp1.isPointProperty());
	BOOST_REQUIRE(!sp1.isEdgeProperty());
	BOOST_REQUIRE(!sp1.isSurfaceProperty());
	BOOST_REQUIRE( sp1.isSpaceProperty());
}

BOOST_AUTO_TEST_CASE( get_space_type )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	
	bso::utilities::geometry::quad_hexahedron space1 = {
		{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0},
		{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}};
	bso::utilities::geometry::quad_hexahedron space2 = {
		{6000,0,0},{9000,0,0},{9000,3000,0},{6000,3000,0},
		{6000,0,3000},{9000,0,3000},{9000,3000,3000},{6000,3000,3000}};
	bso::spatial_design::conformal::cf_space* spacePtr1, * spacePtr2;
	for (const auto& i : cfModel.cfSpaces())
	{
		if (i->isSameAs(space1)) spacePtr1 = i;
		if (i->isSameAs(space2)) spacePtr2 = i;
	}

	space_property sp1(spacePtr1);
	space_property sp2(spacePtr2);
	
	BOOST_REQUIRE(sp1.getSpaceType() == "");
	BOOST_REQUIRE(sp2.getSpaceType() == "B");
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_rule_set_test