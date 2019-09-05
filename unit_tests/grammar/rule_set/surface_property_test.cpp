#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_surface_property_test"
#endif

#include <bso/grammar/rule_set/surface_property.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_surface_property_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	
	bso::utilities::geometry::quadrilateral surface1 = {
		{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0}};
	bso::spatial_design::conformal::cf_surface* surfPtr1;
	for (const auto& i : cfModel.cfSurfaces())
	{
		if (i->isSameAs(surface1)) surfPtr1 = i;
	}
	BOOST_REQUIRE_NO_THROW(surface_property sp1(surfPtr1));
	surface_property sp1(surfPtr1);
	BOOST_REQUIRE(sp1.getSurfacePtr() == surfPtr1);
	
	BOOST_REQUIRE(!sp1.isVertexProperty());
	BOOST_REQUIRE(!sp1.isLineProperty());
	BOOST_REQUIRE(!sp1.isRectangleProperty());
	BOOST_REQUIRE(!sp1.isCuboidProperty());
	BOOST_REQUIRE(!sp1.isPointProperty());
	BOOST_REQUIRE(!sp1.isEdgeProperty());
	BOOST_REQUIRE( sp1.isSurfaceProperty());
	BOOST_REQUIRE(!sp1.isSpaceProperty());
}

BOOST_AUTO_TEST_CASE( get_orientation )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	
	bso::utilities::geometry::quadrilateral surface1 = {
		{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0}};
	bso::utilities::geometry::quadrilateral surface2 = {
		{6000,-1000,0},{9000,-1000,0},{9000,3000,0},{6000,3000,0}};
	bso::utilities::geometry::quadrilateral surface3 = {
		{0,0,3000},{0,3000,3000},{0,3000,6000},{0,0,6000}};
	bso::spatial_design::conformal::cf_surface* surfPtr1 = nullptr,* surfPtr2 = nullptr,
		* surfPtr3 = nullptr;
	for (const auto& i : cfModel.cfSurfaces())
	{
		if (i->isSameAs(surface1)) surfPtr1 = i;
		if (i->isSameAs(surface2)) surfPtr2 = i;
		if (i->isSameAs(surface3)) surfPtr3 = i;
	}

	surface_property sp1(surfPtr1);
	surface_property sp2(surfPtr2);
	surface_property sp3(surfPtr3);
	
	BOOST_REQUIRE(sp1.getOrientation().isCodirectional({0,0,1}));
	BOOST_REQUIRE(sp2.getOrientation().isCodirectional({0,0,-1}));
	BOOST_REQUIRE(sp3.getOrientation().isCodirectional({1,0,0}));
}

BOOST_AUTO_TEST_CASE( get_surface_type )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	
	bso::utilities::geometry::quadrilateral surface1 = {
		{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0}};
	bso::utilities::geometry::quadrilateral surface2 = {
		{6000,-1000,0},{9000,-1000,0},{9000,3000,0},{6000,3000,0}};
	bso::utilities::geometry::quadrilateral surface3 = {
		{15000,0,3000},{15000,3000,3000},{15000,3000,6000},{15000,0,6000}};
	bso::spatial_design::conformal::cf_surface* surfPtr1 = nullptr,* surfPtr2 = nullptr,
		* surfPtr3 = nullptr;
	for (const auto& i : cfModel.cfSurfaces())
	{
		if (i->isSameAs(surface1)) surfPtr1 = i;
		if (i->isSameAs(surface2)) surfPtr2 = i;
		if (i->isSameAs(surface3)) surfPtr3 = i;
	}

	surface_property sp1(surfPtr1);
	surface_property sp2(surfPtr2);
	surface_property sp3(surfPtr3);
	
	BOOST_REQUIRE(sp1.getSurfaceType() == "F");
	BOOST_REQUIRE(sp2.getSurfaceType() == "F");
	BOOST_REQUIRE(sp3.getSurfaceType() == "B");
}

BOOST_AUTO_TEST_CASE( is_wall_or_floor )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	
	bso::utilities::geometry::quadrilateral surface1 = {
		{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0}};
	bso::utilities::geometry::quadrilateral surface2 = {
		{6000,-1000,0},{9000,-1000,0},{9000,3000,0},{6000,3000,0}};
	bso::utilities::geometry::quadrilateral surface3 = {
		{12000,0,3000},{12000,3000,3000},{12000,3000,6000},{12000,0,6000}};
	bso::spatial_design::conformal::cf_surface* surfPtr1 = nullptr,* surfPtr2 = nullptr,
		* surfPtr3 = nullptr;
	for (const auto& i : cfModel.cfSurfaces())
	{
		if (i->isSameAs(surface1)) surfPtr1 = i;
		if (i->isSameAs(surface2)) surfPtr2 = i;
		if (i->isSameAs(surface3)) surfPtr3 = i;
	}

	surface_property sp1(surfPtr1);
	surface_property sp2(surfPtr2);
	surface_property sp3(surfPtr3);
	
	BOOST_REQUIRE(!sp1.isWall());
	BOOST_REQUIRE(!sp2.isWall());
	BOOST_REQUIRE( sp3.isWall());

	BOOST_REQUIRE( sp1.isFloor());
	BOOST_REQUIRE( sp2.isFloor());
	BOOST_REQUIRE(!sp3.isFloor());
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_rule_set_test