#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_line_property_test"
#endif

#include <bso/grammar/rule_set/line_property.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_line_property_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto linePtr = geomModel->addLine({{0,0,0},{3000,0,0}});
	
	BOOST_REQUIRE_NO_THROW(line_property lp1(linePtr));
	line_property lp1(linePtr);
	BOOST_REQUIRE( lp1.getLinePtr() == linePtr);
	
	BOOST_REQUIRE(!lp1.isVertexProperty());
	BOOST_REQUIRE( lp1.isLineProperty());
	BOOST_REQUIRE(!lp1.isRectangleProperty());
	BOOST_REQUIRE(!lp1.isCuboidProperty());
	BOOST_REQUIRE(!lp1.isPointProperty());
	BOOST_REQUIRE(!lp1.isEdgeProperty());
	BOOST_REQUIRE(!lp1.isSurfaceProperty());
	BOOST_REQUIRE(!lp1.isSpaceProperty());
}

BOOST_AUTO_TEST_CASE( is_vertical_or_horizontal )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto linePtr1 = geomModel->addLine({{0,0,0},{3000,0,0}});
	auto linePtr2 = geomModel->addLine({{0,0,0},{0,3000,0}});
	auto linePtr3 = geomModel->addLine({{0,0,0},{0,0,3000}});
	
	line_property lp1(linePtr1);
	line_property lp2(linePtr2);
	line_property lp3(linePtr3);
	
	BOOST_REQUIRE( lp1.isHorizontal());
	BOOST_REQUIRE( lp2.isHorizontal());
	BOOST_REQUIRE(!lp3.isHorizontal());
	
	BOOST_REQUIRE(!lp1.isVertical());
	BOOST_REQUIRE(!lp2.isVertical());
	BOOST_REQUIRE( lp3.isVertical());
}


BOOST_AUTO_TEST_CASE( is_space_edge_or_on_space_surface )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto linePtr1 = geomModel->addLine({{0,0,0},{3000,0,0}});
	auto linePtr2 = geomModel->addLine({{0,0,0},{0,3000,0}});
	auto linePtr3 = geomModel->addLine({{0,0,0},{0,0,3000}});
	auto linePtr4 = geomModel->addLine({{6000,0,-3000},{9000,0,-3000}});
	
	line_property lp1(linePtr1);
	line_property lp2(linePtr2);
	line_property lp3(linePtr3);
	line_property lp4(linePtr4);
	
	BOOST_REQUIRE( lp1.isSpaceEdge());
	BOOST_REQUIRE( lp2.isSpaceEdge());
	BOOST_REQUIRE( lp3.isSpaceEdge());
	BOOST_REQUIRE(!lp4.isSpaceEdge());
	
	BOOST_REQUIRE( lp1.isOnSpaceSurface());
	BOOST_REQUIRE( lp2.isOnSpaceSurface());
	BOOST_REQUIRE( lp3.isOnSpaceSurface());
	BOOST_REQUIRE( lp4.isOnSpaceSurface());
}

BOOST_AUTO_TEST_CASE( is_above_or_below_ground_surface )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto linePtr1 = geomModel->addLine({{0,0,0},{3000,0,0}});
	auto linePtr2 = geomModel->addLine({{0,0,3000},{3000,0,3000}});
	auto linePtr3 = geomModel->addLine({{0,0,0},{0,0,3000}});
	auto linePtr4 = geomModel->addLine({{6000,0,-3000},{9000,0,-3000}});
	
	line_property lp1(linePtr1);
	line_property lp2(linePtr2);
	line_property lp3(linePtr3);
	line_property lp4(linePtr4);
	
	BOOST_REQUIRE(!lp1.isAboveGroundSurface());
	BOOST_REQUIRE( lp2.isAboveGroundSurface());
	BOOST_REQUIRE( lp3.isAboveGroundSurface());
	BOOST_REQUIRE(!lp4.isAboveGroundSurface());
	
	BOOST_REQUIRE(!lp1.isBelowGroundSurface());
	BOOST_REQUIRE(!lp2.isBelowGroundSurface());
	BOOST_REQUIRE(!lp3.isBelowGroundSurface());
	BOOST_REQUIRE( lp4.isBelowGroundSurface());
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_rule_set_test