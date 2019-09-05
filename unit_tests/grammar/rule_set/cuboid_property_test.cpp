#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_cuboid_property_test"
#endif

#include <bso/grammar/rule_set/cuboid_property.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_cuboid_property_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto cubPtr1 = geomModel->addCuboid({
		{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0},
		{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}});

	BOOST_REQUIRE_NO_THROW(cuboid_property cp1(cubPtr1));
	cuboid_property cp1(cubPtr1);
	BOOST_REQUIRE(cp1.getCuboidPtr() == cubPtr1);
	BOOST_REQUIRE(cp1.isInSpace());
	
	BOOST_REQUIRE(!cp1.isVertexProperty());
	BOOST_REQUIRE(!cp1.isLineProperty());
	BOOST_REQUIRE(!cp1.isRectangleProperty());
	BOOST_REQUIRE( cp1.isCuboidProperty());
	BOOST_REQUIRE(!cp1.isPointProperty());
	BOOST_REQUIRE(!cp1.isEdgeProperty());
	BOOST_REQUIRE(!cp1.isSurfaceProperty());
	BOOST_REQUIRE(!cp1.isSpaceProperty());
}

BOOST_AUTO_TEST_CASE( get_space_type )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto cubPtr1 = geomModel->addCuboid({
		{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0},
		{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}});
	auto cubPtr2 = geomModel->addCuboid({
		{3000,0,0},{6000,0,0},{6000,3000,0},{3000,3000,0},
		{3000,0,3000},{6000,0,3000},{6000,3000,3000},{3000,3000,3000}});

	cuboid_property cp1(cubPtr1);
	cuboid_property cp2(cubPtr2);

	BOOST_REQUIRE(cp1.getSpaceType() == "");
	BOOST_REQUIRE(cp2.getSpaceType() == "T");
}

BOOST_AUTO_TEST_CASE( is_above_or_below_ground_surfaces )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto cubPtr1 = geomModel->addCuboid({
		{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0},
		{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}});
	auto cubPtr2 = geomModel->addCuboid({
		{3000,0,3000},{6000,0,3000},{6000,3000,3000},{3000,3000,3000},
		{3000,0,6000},{6000,0,6000},{6000,3000,6000},{3000,3000,6000}});
	auto cubPtr3 = geomModel->addCuboid({
		{6000,0,-3000},{9000,0,-3000},{9000,3000,-3000},{6000,3000,-3000},
		{6000,0,0},{9000,0,0},{9000,3000,0},{6000,3000,0}});

	cuboid_property cp1(cubPtr1);
	cuboid_property cp2(cubPtr2);
	cuboid_property cp3(cubPtr3);

	BOOST_REQUIRE( cp1.isAboveGroundSurface());
	BOOST_REQUIRE( cp2.isAboveGroundSurface());
	BOOST_REQUIRE(!cp3.isAboveGroundSurface());
	
	BOOST_REQUIRE(!cp1.isBelowGroundSurface());
	BOOST_REQUIRE(!cp2.isBelowGroundSurface());
	BOOST_REQUIRE( cp3.isBelowGroundSurface());
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_rule_set_test