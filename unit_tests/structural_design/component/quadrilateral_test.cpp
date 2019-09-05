#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_quadrilateral_component"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/component/quadrilateral.hpp>

#include <sstream>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace component_test {
using namespace bso::structural_design::component;

BOOST_AUTO_TEST_SUITE( sd_quadrilateral_component )
	
	BOOST_AUTO_TEST_CASE( initialize_from_container )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		point p3(1,{1,1,0});
		point p4(1,{0,1,0});
		std::vector<point> pl1 = {p1, p2, p3, p4};
		quadrilateral q1(pl1);
		
		BOOST_REQUIRE(p1.isSameAs(q1[0]));
		BOOST_REQUIRE(p2.isSameAs(q1[1]));
		BOOST_REQUIRE(p3.isSameAs(q1[2]));
		BOOST_REQUIRE(p4.isSameAs(q1[3]));
	}

	BOOST_AUTO_TEST_CASE( initialize_from_initializer_list )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		point p3(1,{1,1,0});
		point p4(1,{0,1,0});
		quadrilateral q1({p1, p2, p3, p4});
		
		BOOST_REQUIRE(p1.isSameAs(q1[0]));
		BOOST_REQUIRE(p2.isSameAs(q1[1]));
		BOOST_REQUIRE(p3.isSameAs(q1[2]));
		BOOST_REQUIRE(p4.isSameAs(q1[3]));
	}
	
	BOOST_AUTO_TEST_CASE( flat_shell )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		point p3(1,{1,1,0});
		point p4(1,{0,1,0});
		quadrilateral q1({p1, p2, p3, p4});
		
		structure st1("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e5}});
		q1.addStructure(st1);
		
		BOOST_REQUIRE(!q1.hasTruss());
		BOOST_REQUIRE(!q1.hasBeam());
		BOOST_REQUIRE( q1.hasFlatShell());
		BOOST_REQUIRE(!q1.hasQuadHexahedron());
		
		BOOST_REQUIRE(q1.getStructures()[0].thickness() == 100);
		BOOST_REQUIRE(q1.getStructures()[0].poisson() == 0.3);
		BOOST_REQUIRE(q1.getStructures()[0].E() == 1e5);
	}
	
	BOOST_AUTO_TEST_CASE( invalid_structure )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		point p3(1,{1,1,0});
		point p4(1,{0,1,0});
		quadrilateral q1({p1, p2, p3, p4});
		
		structure st1("truss",{{"A",100},{"E",1e5}});
		structure st2("beam",{{"width",100},{"height",400},{"poisson",0.3},{"E",1e5}});
		structure st3("quad_hexahedron",{{"poisson",0.3},{"E",1e5}});
		
		BOOST_REQUIRE_THROW(q1.addStructure(st1),std::invalid_argument);
		BOOST_REQUIRE_THROW(q1.addStructure(st2),std::invalid_argument);
		BOOST_REQUIRE_THROW(q1.addStructure(st3),std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( mesh )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		point p3(1,{1,1,0});
		point p4(1,{0,1,0});
		quadrilateral q1({p1, p2, p3, p4});
		
		structure st1("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e5}});
		q1.addStructure(st1);
		
		std::vector<point*> pointStore;
		q1.mesh(2,pointStore);

		BOOST_REQUIRE(q1.getElementPoints().size() == 4);
		BOOST_REQUIRE(q1.getMeshedPoints().size() == 9);

		BOOST_REQUIRE(q1.getMeshedPoints()[0]->isSameAs({0,  0,  0}));
		BOOST_REQUIRE(q1.getMeshedPoints()[1]->isSameAs({0.5,0,  0}));
		BOOST_REQUIRE(q1.getMeshedPoints()[2]->isSameAs({1,  0,  0}));
		BOOST_REQUIRE(q1.getMeshedPoints()[3]->isSameAs({0,  0.5,0}));
		BOOST_REQUIRE(q1.getMeshedPoints()[4]->isSameAs({0.5,0.5,0}));
		BOOST_REQUIRE(q1.getMeshedPoints()[5]->isSameAs({1,  0.5,0}));
		BOOST_REQUIRE(q1.getMeshedPoints()[6]->isSameAs({0,  1,  0}));
		BOOST_REQUIRE(q1.getMeshedPoints()[7]->isSameAs({0.5,1,  0}));
		BOOST_REQUIRE(q1.getMeshedPoints()[8]->isSameAs({1,  1,  0}));
		
		q1.clearMesh();
		BOOST_REQUIRE(q1.getElements().size() == 0);
		BOOST_REQUIRE(q1.getMeshedPoints().size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( load_test )
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		point p3(1,{1,1,0});
		point p4(1,{0,1,0});
		quadrilateral q1({p1, p2, p3, p4});
		
		structure st1("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e5}});
		q1.addStructure(st1);
		
		load_case lc1("test_case");
		load l1(lc1,160,0);
		q1.addLoad(l1);
		
		std::vector<point*> pointStore;
		q1.mesh(2,pointStore);
		
		for (auto& i : pointStore)
		{
			for (const auto& j : i->getLoads())
			{
				BOOST_REQUIRE(j.magnitude() == 10);
			}
		}
		
		BOOST_REQUIRE(pointStore[0]->getLoads().size() == 1);
		BOOST_REQUIRE(pointStore[3]->getLoads().size() == 2);
		BOOST_REQUIRE(pointStore[4]->getLoads().size() == 4);
	}
	
	BOOST_AUTO_TEST_CASE( constraint_test)
	{
		point p1(1,{0,0,0});
		point p2(1,{1,0,0});
		point p3(1,{1,1,0});
		point p4(1,{0,1,0});
		quadrilateral q1({p1, p2, p3, p4});
		
		structure st1("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e5}});
		q1.addStructure(st1);
		
		constraint c1(4);
		q1.addConstraint(c1);
		
		std::vector<point*> pointStore;
		q1.mesh(2,pointStore);
		
		for (auto& i : pointStore)
		{
			for (const auto& j : i->getConstraints())
			{
				BOOST_REQUIRE(j.DOF() == 4);
			}
		}
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace component_test