#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_vertex_property_test"
#endif

#include <bso/grammar/rule_set/vertex_property.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_vertex_property_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto vertexPtr1 = geomModel->addVertex({0,0,0});
	
	BOOST_REQUIRE_NO_THROW(vertex_property vp1(vertexPtr1));
	vertex_property vp1(vertexPtr1);
	BOOST_REQUIRE( vp1.getVertexPtr() == vertexPtr1);
	
	BOOST_REQUIRE( vp1.isVertexProperty());
	BOOST_REQUIRE(!vp1.isLineProperty());
	BOOST_REQUIRE(!vp1.isRectangleProperty());
	BOOST_REQUIRE(!vp1.isCuboidProperty());
	BOOST_REQUIRE(!vp1.isPointProperty());
	BOOST_REQUIRE(!vp1.isEdgeProperty());
	BOOST_REQUIRE(!vp1.isSurfaceProperty());
	BOOST_REQUIRE(!vp1.isSpaceProperty());
}

BOOST_AUTO_TEST_CASE( is_above_or_below_ground_surface )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto vertexPtr1 = geomModel->addVertex({0,0,0});
	auto vertexPtr2 = geomModel->addVertex({6000,0,3000});
	auto vertexPtr3 = geomModel->addVertex({9000,0,-3000});
	
	vertex_property vp1(vertexPtr1);
	vertex_property vp2(vertexPtr2);
	vertex_property vp3(vertexPtr3);
	
	BOOST_REQUIRE(!vp1.isAboveGroundSurface());
	BOOST_REQUIRE( vp2.isAboveGroundSurface());
	BOOST_REQUIRE(!vp3.isAboveGroundSurface());
	
	BOOST_REQUIRE(!vp1.isBelowGroundSurface());
	BOOST_REQUIRE(!vp2.isBelowGroundSurface());
	BOOST_REQUIRE( vp3.isBelowGroundSurface());
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_rule_set_test