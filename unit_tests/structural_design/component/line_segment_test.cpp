#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_line_segment_component"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/component/line_segment.hpp>

#include <sstream>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace component_test {
using namespace bso::structural_design::component;

BOOST_AUTO_TEST_SUITE( sd_line_segment_component )
	
	BOOST_AUTO_TEST_CASE( initialize_from_container )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		std::vector<point> pl1 = {p1, p2};
		line_segment ls1(pl1);
		
		BOOST_REQUIRE(p1.isSameAs(ls1[0]));
		BOOST_REQUIRE(p2.isSameAs(ls1[1]));
	}

	BOOST_AUTO_TEST_CASE( initialize_from_initializer_list )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		line_segment ls1({p1, p2});
		
		BOOST_REQUIRE(p1.isSameAs(ls1[0]));
		BOOST_REQUIRE(p2.isSameAs(ls1[1]));
	}
	
	BOOST_AUTO_TEST_CASE( truss )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		line_segment ls1({p1, p2});
		
		structure st1("truss",{{"A",100},{"E",1e5}});
		ls1.addStructure(st1);
		
		BOOST_REQUIRE( ls1.hasTruss());
		BOOST_REQUIRE(!ls1.hasBeam());
		BOOST_REQUIRE(!ls1.hasFlatShell());
		BOOST_REQUIRE(!ls1.hasQuadHexahedron());
		
		BOOST_REQUIRE(ls1.getStructures()[0].A() == 100);
		BOOST_REQUIRE(ls1.getStructures()[0].E() == 1e5);
	}
	
	BOOST_AUTO_TEST_CASE( beam )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		line_segment ls1({p1, p2});
		
		structure st1("beam",{{"width",100},{"height",400},{"poisson",0.3},{"E",1e5}});
		ls1.addStructure(st1);
		
		BOOST_REQUIRE(!ls1.hasTruss());
		BOOST_REQUIRE( ls1.hasBeam());
		BOOST_REQUIRE(!ls1.hasFlatShell());
		BOOST_REQUIRE(!ls1.hasQuadHexahedron());
		
		BOOST_REQUIRE(ls1.getStructures()[0].poisson() == 0.3);
		BOOST_REQUIRE(ls1.getStructures()[0].width() == 100);
		BOOST_REQUIRE(ls1.getStructures()[0].height() == 400);
		BOOST_REQUIRE(ls1.getStructures()[0].E() == 1e5);
	}
	
	BOOST_AUTO_TEST_CASE( invalid_structure )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		line_segment ls1({p1, p2});
		
		structure st1("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e5}});
		structure st2("quad_hexahedron",{{"poisson",0.3},{"E",1e5}});
		
		BOOST_REQUIRE_THROW(ls1.addStructure(st1),std::invalid_argument);
		BOOST_REQUIRE_THROW(ls1.addStructure(st2),std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( mesh_beam )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		line_segment ls1({p1, p2});
		
		structure st1("beam",{{"width",100},{"height",400},{"poisson",0.3},{"E",1e5}});
		ls1.addStructure(st1);
		
		std::vector<point*> pointStore;
		ls1.mesh(2,pointStore);

		BOOST_REQUIRE(ls1.getElementPoints().size() == 2);
		BOOST_REQUIRE(ls1.getMeshedPoints().size() == 3);
		BOOST_REQUIRE(ls1.getMeshedPoints()[0]->isSameAs({0,0,0}));
		BOOST_REQUIRE(ls1.getMeshedPoints()[1]->isSameAs({0.5,0,0}));
		BOOST_REQUIRE(ls1.getMeshedPoints()[2]->isSameAs({1,0,0}));
		
		ls1.clearMesh();
		BOOST_REQUIRE(ls1.getElementPoints().size()  == 0);
		BOOST_REQUIRE(ls1.getMeshedPoints().size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( mesh_truss )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		line_segment ls1({p1, p2});
		
		structure st1("truss",{{"A",100},{"E",1e5}});
		ls1.addStructure(st1);
		
		std::vector<point*> pointStore;
		ls1.mesh(2,pointStore);

		BOOST_REQUIRE(ls1.getElementPoints().size() == 2);
		BOOST_REQUIRE(ls1.getMeshedPoints().size() == 3);
		BOOST_REQUIRE(ls1.getMeshedPoints()[0]->isSameAs({0,0,0}));
		BOOST_REQUIRE(ls1.getMeshedPoints()[1]->isSameAs({0.5,0,0}));
		BOOST_REQUIRE(ls1.getMeshedPoints()[2]->isSameAs({1,0,0}));
		
		ls1.clearMesh();
		BOOST_REQUIRE(ls1.getElementPoints().size()  == 0);
		BOOST_REQUIRE(ls1.getMeshedPoints().size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( load_test )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		line_segment ls1({p1, p2});
		
		structure st1("beam",{{"width",100},{"height",400},{"poisson",0.3},{"E",1e5}});
		ls1.addStructure(st1);
		
		load_case lc1("test_case");
		load l1(lc1,40,0);
		ls1.addLoad(l1);
		
		std::vector<point*> pointStore;
		ls1.mesh(2,pointStore);
		
		BOOST_REQUIRE(ls1.getMeshedPoints()[0]->getLoads()[0].magnitude() == 10);
		BOOST_REQUIRE(ls1.getMeshedPoints()[1]->getLoads()[0].magnitude() == 10);
		BOOST_REQUIRE(ls1.getMeshedPoints()[1]->getLoads()[1].magnitude() == 10);
		BOOST_REQUIRE(ls1.getMeshedPoints()[2]->getLoads()[0].magnitude() == 10);
	}
	
	BOOST_AUTO_TEST_CASE( constraint_test )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		line_segment ls1({p1, p2});
		
		structure st1("beam",{{"width",100},{"height",400},{"poisson",0.3},{"E",1e5}});
		ls1.addStructure(st1);
		
		constraint c1(2);
		ls1.addConstraint(c1);
		
		std::vector<point*> pointStore;
		ls1.mesh(2,pointStore);
		
		BOOST_REQUIRE(ls1.getMeshedPoints()[0]->getConstraints()[0].DOF() == 2);
		BOOST_REQUIRE(ls1.getMeshedPoints()[1]->getConstraints()[0].DOF() == 2);
		BOOST_REQUIRE(ls1.getMeshedPoints()[2]->getConstraints()[0].DOF() == 2);
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace component_test