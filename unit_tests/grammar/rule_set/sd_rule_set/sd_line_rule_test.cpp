#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_sd_line_rule_test"
#endif

#include <bso/grammar/rule_set/sd_rule_set/sd_line_rule.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_sd_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_sd_line_rule_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto linePtr = geomModel->addLine({{0,0,3000},{3000,0,3000}});
	line_property lp1(linePtr);
	
	BOOST_REQUIRE_NO_THROW(sd_line_rule sdl1(&lp1));
	sd_line_rule sdl1(&lp1);
	BOOST_REQUIRE(sdl1.getStructure().type() == "none");
	
	bso::structural_design::sd_model sd;
	sdl1.apply(sd);
	
	BOOST_REQUIRE(sd.getGeometries().size() == 0);
}
	
BOOST_AUTO_TEST_CASE( assign_structure )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto linePtr = geomModel->addLine({{0,0,3000},{3000,0,3000}});
	line_property lp1(linePtr);

	sd_line_rule sdl1(&lp1);
	BOOST_REQUIRE(sdl1.getStructure().type() == "none");
	typedef bso::structural_design::component::structure sd_structure;
	std::vector<sd_structure> structures;
	structures.push_back(sd_structure("truss",{{"A",1e5},{"E",1e6}}));
	
	sdl1.assignStructure(structures.back());
	BOOST_REQUIRE(sdl1.getStructure().type() == "truss");
	structures.back().isGhostComponent() = true;
	sdl1.assignStructure(structures);
	BOOST_REQUIRE(sdl1.getStructure().type() == "truss");
	BOOST_REQUIRE(sdl1.getStructure().isGhostComponent());
	
	structures.push_back(sd_structure("truss",{{"A",1e5},{"E",1e6}}));
	sdl1.assignStructure(structures);
	BOOST_REQUIRE(sdl1.getStructure().type() == "truss");
	BOOST_REQUIRE(sdl1.getStructure().A() == 1e5);
	BOOST_REQUIRE(sdl1.getStructure().E() == 1e6);
	BOOST_REQUIRE(!sdl1.getStructure().isGhostComponent());
	
	structures.push_back(sd_structure("beam",{{"width",100},{"height",300},{"E",1e6},{"poisson",0.3}}));
	sdl1.assignStructure(structures);
	BOOST_REQUIRE(sdl1.getStructure().type() == "beam");
	BOOST_REQUIRE(sdl1.getStructure().width() == 100);
	BOOST_REQUIRE(sdl1.getStructure().height() == 300);
	BOOST_REQUIRE(sdl1.getStructure().E() == 1e6);
	BOOST_REQUIRE(sdl1.getStructure().poisson() == 0.3);
	BOOST_REQUIRE(!sdl1.getStructure().isGhostComponent());
	
	structures.push_back(sd_structure("flat_shell",{{"thickness",100},{"E",1e7},{"poisson",0.3}}));
	sdl1.assignStructure(structures);
	BOOST_REQUIRE(sdl1.getStructure().type() == "flat_shell");
	BOOST_REQUIRE(sdl1.getStructure().thickness() == 100);
	BOOST_REQUIRE(sdl1.getStructure().E() == 1e7);
	BOOST_REQUIRE(sdl1.getStructure().poisson() == 0.3);
	BOOST_REQUIRE(!sdl1.getStructure().isGhostComponent());
}
	
BOOST_AUTO_TEST_CASE( apply_rule )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto linePtr1 = geomModel->addLine({{0,0,0},{3000,0,0}});
	auto linePtr2 = geomModel->addLine({{0,0,3000},{3000,0,3000}});
	line_property lp1(linePtr1);
	line_property lp2(linePtr2);

	sd_line_rule sdl1(&lp1);
	sd_line_rule sdl2(&lp2);
	typedef bso::structural_design::component::structure sd_structure;
	
	bso::structural_design::sd_model sd;
	sdl2.apply(sd);
	BOOST_REQUIRE(sd.getGeometries().size() == 0);
	sdl1.apply(sd);
	BOOST_REQUIRE(sd.getGeometries().size() == 1);
	BOOST_REQUIRE(sd.getGeometries().front()->getConstraints().size() == 3);

	sdl1.assignStructure(sd_structure("truss",{{"A",1e5},{"E",1e6}}));
	sd = bso::structural_design::sd_model();
	sdl1.apply(sd);
	
	BOOST_REQUIRE(sd.getGeometries().size() == 1);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().size() == 1);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().front().type() == "truss");
	
	sdl1.assignStructure(sd_structure("beam",{{"width",100},{"height",300},{"E",1e6},{"poisson",0.3}}));
	sd = bso::structural_design::sd_model();
	sdl1.apply(sd);
	
	BOOST_REQUIRE(sd.getGeometries().size() == 1);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().size() == 1);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().front().type() == "beam");
	
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_sd_rule_set_test