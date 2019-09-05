#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_rectangle_property_test"
#endif

#include <bso/grammar/rule_set/rectangle_property.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_rectangle_property_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr = geomModel->addRectangle({{0,0,0},{3000,0,0},{3000,0,3000},{0,0,3000}});

	BOOST_REQUIRE_NO_THROW(rectangle_property rp1(rectPtr));
	rectangle_property rp1(rectPtr);
	BOOST_REQUIRE(rp1.getRectanglePtr() == rectPtr);
	BOOST_REQUIRE(rp1.getOrientation().isCodirectional({0,1,0}));
	
	BOOST_REQUIRE(!rp1.isVertexProperty());
	BOOST_REQUIRE(!rp1.isLineProperty());
	BOOST_REQUIRE( rp1.isRectangleProperty());
	BOOST_REQUIRE(!rp1.isCuboidProperty());
	BOOST_REQUIRE(!rp1.isPointProperty());
	BOOST_REQUIRE(!rp1.isEdgeProperty());
	BOOST_REQUIRE(!rp1.isSurfaceProperty());
	BOOST_REQUIRE(!rp1.isSpaceProperty());
}

BOOST_AUTO_TEST_CASE( space_and_surface_types )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr1 = geomModel->addRectangle({{0,0,3000},{3000,0,3000},{3000,0,6000},{0,0,6000}});
	auto rectPtr2 = geomModel->addRectangle({{3000,0,0},{6000,0,0},{6000,3000,0},{3000,3000,0}});
	auto rectPtr3 = geomModel->addRectangle({{9000,0,3000},{9000,3000,3000},{9000,3000,6000},{9000,0,6000}});
	auto rectPtr4 = geomModel->addRectangle({{9000,-1000,-3000},{9000,0,-3000},{9000,0,0},{9000,-1000,0}});
	auto rectPtr5 = geomModel->addRectangle({{3000,0,0},{3000,3000,0},{3000,3000,3000},{3000,0,3000}});
	auto rectPtr6 = geomModel->addRectangle({{6000,-1000,-3000},{9000,-1000,-3000},{9000,-1000,0},{6000,-1000,0}});
	auto rectPtr7 = geomModel->addRectangle({{0,0,3000},{0,3000,3000},{0,3000,6000},{0,0,6000}});

	rectangle_property rp1(rectPtr1);
	rectangle_property rp2(rectPtr2);
	rectangle_property rp3(rectPtr3);
	rectangle_property rp4(rectPtr4);
	rectangle_property rp5(rectPtr5);
	rectangle_property rp6(rectPtr6);
	rectangle_property rp7(rectPtr7);
	
	typedef std::pair<std::string, std::string> type_set;

	BOOST_REQUIRE(rp1.getSpaceTypes() == type_set("E","F"));
	BOOST_REQUIRE(rp2.getSpaceTypes() == type_set("G","T"));
	BOOST_REQUIRE(rp3.getSpaceTypes() == type_set("","T"));
	BOOST_REQUIRE(rp4.getSpaceTypes() == type_set("F","G"));
	BOOST_REQUIRE(rp5.getSpaceTypes() == type_set("","T"));
	BOOST_REQUIRE(rp6.getSpaceTypes() == type_set("F","G"));
	BOOST_REQUIRE(rp7.getSpaceTypes() == type_set("E","F"));
	
	BOOST_REQUIRE(rp1.getSurfaceTypes() == type_set("E","T"));
	BOOST_REQUIRE(rp2.getSurfaceTypes() == type_set("F","G"));
	BOOST_REQUIRE(rp3.getSurfaceTypes() == type_set("B","F"));
	BOOST_REQUIRE(rp4.getSurfaceTypes() == type_set("B","G"));
	BOOST_REQUIRE(rp5.getSurfaceTypes() == type_set("","B"));
	BOOST_REQUIRE(rp6.getSurfaceTypes() == type_set("B","G"));
	BOOST_REQUIRE(rp7.getSurfaceTypes() == type_set("E","F"));
}

BOOST_AUTO_TEST_CASE( is_wall_check )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr1 = geomModel->addRectangle({{0,0,0},{3000,0,0},{3000,0,3000},{0,0,3000}});
	auto rectPtr2 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,0,0},{6000,0,0}});
	auto rectPtr3 = geomModel->addRectangle({{6000,0,3000},{6000,3000,3000},{6000,3000,6000},{6000,0,6000}});
	auto rectPtr4 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,3000,-3000},{6000,3000,-3000}});
	auto rectPtr5 = geomModel->addRectangle({{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}});

	rectangle_property rp1(rectPtr1);
	rectangle_property rp2(rectPtr2);
	rectangle_property rp3(rectPtr3);
	rectangle_property rp4(rectPtr4);
	rectangle_property rp5(rectPtr5);
	
	BOOST_REQUIRE( rp1.isWall());
	BOOST_REQUIRE( rp2.isWall());
	BOOST_REQUIRE( rp3.isWall());
	BOOST_REQUIRE(!rp4.isWall());
	BOOST_REQUIRE(!rp5.isWall());
}

BOOST_AUTO_TEST_CASE( is_floor_check )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr1 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,3000,-3000},{6000,3000,-3000}});
	auto rectPtr2 = geomModel->addRectangle({{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}});
	auto rectPtr3 = geomModel->addRectangle({{9000,0,6000},{12000,0,6000},{12000,3000,6000},{9000,3000,6000}});
	auto rectPtr4 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,0,0},{6000,0,0}});
	auto rectPtr5 = geomModel->addRectangle({{6000,0,3000},{6000,3000,3000},{6000,3000,6000},{6000,0,6000}});

	rectangle_property rp1(rectPtr1);
	rectangle_property rp2(rectPtr2);
	rectangle_property rp3(rectPtr3);
	rectangle_property rp4(rectPtr4);
	rectangle_property rp5(rectPtr5);
	
	BOOST_REQUIRE( rp1.isFloor());
	BOOST_REQUIRE( rp2.isFloor());
	BOOST_REQUIRE( rp3.isFloor());
	BOOST_REQUIRE(!rp4.isFloor());
	BOOST_REQUIRE(!rp5.isFloor());
}

BOOST_AUTO_TEST_CASE( is_space_separating_check )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr1 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,3000,-3000},{6000,3000,-3000}});
	auto rectPtr2 = geomModel->addRectangle({{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}});
	auto rectPtr3 = geomModel->addRectangle({{9000,0,6000},{12000,0,6000},{12000,3000,6000},{9000,3000,6000}});
	auto rectPtr4 = geomModel->addRectangle({{6000,-1000,-3000},{9000,-1000,-3000},{9000,-1000,0},{6000,-1000,0}});
	auto rectPtr5 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,0,0},{6000,0,0}});

	rectangle_property rp1(rectPtr1);
	rectangle_property rp2(rectPtr2);
	rectangle_property rp3(rectPtr3);
	rectangle_property rp4(rectPtr4);
	rectangle_property rp5(rectPtr5);
	
	BOOST_REQUIRE( rp1.isSpaceSeparating());
	BOOST_REQUIRE( rp2.isSpaceSeparating());
	BOOST_REQUIRE( rp3.isSpaceSeparating());
	BOOST_REQUIRE( rp4.isSpaceSeparating());
	BOOST_REQUIRE(!rp5.isSpaceSeparating());
}

BOOST_AUTO_TEST_CASE( is_external_check )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr1 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,3000,-3000},{6000,3000,-3000}});
	auto rectPtr2 = geomModel->addRectangle({{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}});
	auto rectPtr3 = geomModel->addRectangle({{9000,0,6000},{9000,3000,6000},{9000,3000,9000},{9000,0,9000}});
	auto rectPtr4 = geomModel->addRectangle({{6000,-1000,-3000},{9000,-1000,-3000},{9000,-1000,0},{6000,-1000,0}});
	auto rectPtr5 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,0,0},{6000,0,0}});

	rectangle_property rp1(rectPtr1);
	rectangle_property rp2(rectPtr2);
	rectangle_property rp3(rectPtr3);
	rectangle_property rp4(rectPtr4);
	rectangle_property rp5(rectPtr5);
	
	BOOST_REQUIRE( rp1.isExternal());
	BOOST_REQUIRE(!rp2.isExternal());
	BOOST_REQUIRE(!rp3.isExternal());
	BOOST_REQUIRE( rp4.isExternal());
	BOOST_REQUIRE(!rp5.isExternal());
}

BOOST_AUTO_TEST_CASE( is_roof_or_overhang_check )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr1 = geomModel->addRectangle({{0,0,6000},{3000,0,6000},{3000,3000,6000},{0,3000,6000}});
	auto rectPtr2 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,3000,-3000},{6000,3000,-3000}});
	auto rectPtr3 = geomModel->addRectangle({{6000,-1000,0},{9000,-1000,0},{9000,0,0},{6000,0,0}});
	auto rectPtr4 = geomModel->addRectangle({{12000,0,3000},{15000,0,3000},{15000,3000,3000},{12000,3000,3000}});

	rectangle_property rp1(rectPtr1);
	rectangle_property rp2(rectPtr2);
	rectangle_property rp3(rectPtr3);
	rectangle_property rp4(rectPtr4);

	BOOST_REQUIRE( rp1.isRoof());
	BOOST_REQUIRE(!rp2.isRoof());
	BOOST_REQUIRE( rp3.isRoof());
	BOOST_REQUIRE(!rp4.isRoof());
	
	BOOST_REQUIRE(!rp1.isOverhang());
	BOOST_REQUIRE(!rp2.isOverhang());
	BOOST_REQUIRE(!rp3.isOverhang());
	BOOST_REQUIRE( rp4.isOverhang());
}

BOOST_AUTO_TEST_CASE( is_above_or_below_ground_check )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr1 = geomModel->addRectangle({{0,0,6000},{3000,0,6000},{3000,3000,6000},{0,3000,6000}});
	auto rectPtr2 = geomModel->addRectangle({{6000,0,0},{9000,0,0},{9000,3000,0},{600,3000,0}});
	auto rectPtr3 = geomModel->addRectangle({{9000,0,3000},{9000,3000,3000},{9000,3000,6000},{9000,0,6000}});
	auto rectPtr4 = geomModel->addRectangle({{6000,0,-3000},{9000,0,-3000},{9000,3000,-3000},{6000,3000,-3000}});
	auto rectPtr5 = geomModel->addRectangle({{6000,-1000,-3000},{6000,0,-3000},{6000,0,0},{6000,-1000,0}});

	rectangle_property rp1(rectPtr1);
	rectangle_property rp2(rectPtr2);
	rectangle_property rp3(rectPtr3);
	rectangle_property rp4(rectPtr4);
	rectangle_property rp5(rectPtr5);

	BOOST_REQUIRE( rp1.isAboveGroundSurface());
	BOOST_REQUIRE(!rp2.isAboveGroundSurface());
	BOOST_REQUIRE( rp3.isAboveGroundSurface());
	BOOST_REQUIRE(!rp4.isAboveGroundSurface());
	BOOST_REQUIRE(!rp5.isAboveGroundSurface());
	
	BOOST_REQUIRE(!rp1.isBelowGroundSurface());
	BOOST_REQUIRE(!rp2.isBelowGroundSurface());
	BOOST_REQUIRE(!rp3.isBelowGroundSurface());
	BOOST_REQUIRE( rp4.isBelowGroundSurface());
	BOOST_REQUIRE( rp5.isBelowGroundSurface());
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_rule_set_test