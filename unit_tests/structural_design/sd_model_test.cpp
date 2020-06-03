#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_model_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/sd_model.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace structural_design_test {
using namespace bso::structural_design;

BOOST_AUTO_TEST_SUITE( sd_analysis_test )
	
	BOOST_AUTO_TEST_CASE( initialize_empty )
	{
		BOOST_REQUIRE_NO_THROW(sd_model test_sd());
	}
	
	BOOST_AUTO_TEST_CASE( add_point )
	{
		sd_model sd1;
		auto p1 = sd1.addPoint({1,0,0});
		
		BOOST_REQUIRE(p1->isSameAs({1,0,0}));
		BOOST_REQUIRE(p1->getID() == 0);
		BOOST_REQUIRE(sd1.getPoints().size() == 1);
		BOOST_REQUIRE(sd1.getGeometries().size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( add_geometry )
	{
		sd_model sd1;
		auto geom1 = sd1.addGeometry(bso::utilities::geometry::line_segment({{0,0,0},{1,0,0}}));
		
		BOOST_REQUIRE( geom1->isLineSegment());
		BOOST_REQUIRE(!geom1->isQuadrilateral());
		BOOST_REQUIRE(!geom1->isQuadHexahedron());
		
		BOOST_REQUIRE(sd1.getPoints().size() == 0);
		BOOST_REQUIRE(sd1.getGeometries().size() == 1);
		
		auto geom2 = sd1.addGeometry(bso::utilities::geometry::quadrilateral(
		{{0,0,0},{1,0,0},{1,1,0},{0,1,0}}));
		
		BOOST_REQUIRE(!geom2->isLineSegment());
		BOOST_REQUIRE( geom2->isQuadrilateral());
		BOOST_REQUIRE(!geom2->isQuadHexahedron());
		BOOST_REQUIRE(sd1.getPoints().size() == 0);
		BOOST_REQUIRE(sd1.getGeometries().size() == 2);
		
		auto geom3 = sd1.addGeometry(bso::utilities::geometry::quad_hexahedron(
		{{0,0,0},{1,0,0},{1,1,0},{0,1,0},{0,0,1},{1,0,1},{1,1,1},{0,1,1}}));
		
		BOOST_REQUIRE(!geom3->isLineSegment());
		BOOST_REQUIRE(!geom3->isQuadrilateral());
		BOOST_REQUIRE( geom3->isQuadHexahedron());
		BOOST_REQUIRE(sd1.getPoints().size() == 0);
		BOOST_REQUIRE(sd1.getGeometries().size() == 3);
	}

	BOOST_AUTO_TEST_CASE( mesh )
	{
		sd_model sd1;
		auto geom1 = sd1.addGeometry(bso::utilities::geometry::line_segment({{0,0,0},{1,0,0}}));
		auto geom2 = sd1.addGeometry(bso::utilities::geometry::quadrilateral(
			{{0,0,0},{1,0,0},{1,1,0},{0,1,0}}));
		auto geom3 = sd1.addGeometry(bso::utilities::geometry::quad_hexahedron(
			{{0,0,0},{1,0,0},{1,1,0},{0,1,0},{0,0,1},{1,0,1},{1,1,1},{0,1,1}}));
		
		component::structure str1("truss",{{"E",1e5},{"A",1e3}});
		geom1->addStructure(str1);
		component::structure str2("beam",{{"E",1e5},{"width",50},{"height",200},{"poisson",0.3}});
		geom1->addStructure(str2);
		component::structure str3("flat_shell",{{"E",1e5},{"thickness",10},{"poisson",0.3}});
		geom2->addStructure(str3);
		component::structure str4("quad_hexahedron",{{"E",1e5},{"poisson",0.3}});
		geom3->addStructure(str4);

		sd1.mesh(2);
		BOOST_REQUIRE(sd1.getPoints().size() == 0);
		BOOST_REQUIRE(sd1.getGeometries().size() == 3);
		
		unsigned int trussCount = 0, beamCount = 0, flatShellCount = 0,
								 quadHexahedronCount = 0, unknownCount = 0;
		for (const auto& i : sd1.getFEA()->getElements())
		{
			if (i->isTruss()) ++trussCount;
			else if (i->isBeam()) ++beamCount;
			else if (i->isFlatShell()) ++flatShellCount;
			else if (i->isQuadHexahedron()) ++quadHexahedronCount;
		}

		BOOST_REQUIRE(trussCount == 1);
		BOOST_REQUIRE(beamCount == 2);
		BOOST_REQUIRE(flatShellCount == 4);
		BOOST_REQUIRE(quadHexahedronCount == 8);
		BOOST_REQUIRE(unknownCount == 0);
	}

	BOOST_AUTO_TEST_CASE( analyze_truss )
	{
		sd_model sd1;
		namespace geom = bso::utilities::geometry;
		
		auto p1 = sd1.addPoint({0,0,0});
		auto p2 = sd1.addPoint({0.320,1.500,0.32*tan(60*M_PI/180.0)/3.0});
		auto p3 = sd1.addPoint({0.640,0,0});
		auto p4 = sd1.addPoint({0.320,0,0.32*tan(60*M_PI/180.0)});
		
		component::constraint c0(0);
		component::constraint c1(1);
		component::constraint c2(2);
		p1->addConstraint(c0); p1->addConstraint(c1); p1->addConstraint(c2);
		p3->addConstraint(c0); p3->addConstraint(c1); p3->addConstraint(c2);
		p4->addConstraint(c0); p4->addConstraint(c1); p4->addConstraint(c2);
		
		component::load_case lc1("vertical load");
		component::load l1(lc1,-0.2,1);
		p2->addLoad(l1);
		
		auto geom1 = sd1.addGeometry(geom::line_segment({*p2,*p1}));
		auto geom2 = sd1.addGeometry(geom::line_segment({*p2,*p3}));
		auto geom3 = sd1.addGeometry(geom::line_segment({*p2,*p4}));
		
		component::structure str1("truss",{{"E",1e7},{"A",M_PI*pow(2e-2,2)}});
		geom1->addStructure(str1);
		geom2->addStructure(str1);
		geom3->addStructure(str1);
		
		sd1.mesh(2); // trusses should still be meshe into one element
		sd1.analyze();
		
		auto displacements = sd1.getFEA()->getNodes()[1]->getDisplacements(lc1);
		Eigen::Vector6d checkDisp;
		checkDisp << 0,-8.693e-6,0,0,0,0;

		BOOST_REQUIRE(checkDisp.isApprox(displacements,1e-4));
	}
	
	BOOST_AUTO_TEST_CASE( analyze_beam )
	{
		sd_model sd1;
		namespace geom = bso::utilities::geometry;
		
		auto p1 = sd1.addPoint({0,0,0});
		auto p2 = sd1.addPoint({3500,0,0});
		
		component::constraint c0(0); p1->addConstraint(c0);
		component::constraint c1(1); p1->addConstraint(c1);
		component::constraint c2(2); p1->addConstraint(c2);
		component::constraint c3(3); p1->addConstraint(c3);
		component::constraint c4(4); p1->addConstraint(c4);
		component::constraint c5(5); p1->addConstraint(c5);
		
		component::load_case lc1("vertical load");
		component::load l1(lc1, 300,0);
		component::load l2(lc1,-200,1);
		component::load l3(lc1, 400,2);
		p2->addLoad(l1);
		p2->addLoad(l2);
		p2->addLoad(l3);
		
		auto geom1 = sd1.addGeometry(geom::line_segment({*p2,*p1}));
		
		component::structure str1("beam",{{"E",1e5},{"width",100},{"height",400},{"poisson",0.3}});
		geom1->addStructure(str1);
		
		sd1.mesh(10);
		sd1.analyze();
		
		element::node* checkNode;
		bool checkNodeFound = false;
		for (const auto& i : sd1.getFEA()->getNodes())
		{
			if (i->isSameAs(*p2))
			{
				checkNode = i;
				checkNodeFound = true;
			}
		}
		BOOST_REQUIRE(checkNodeFound);

		Eigen::Vector6d checkDisp;
		checkDisp << 2.625e-4,-0.8575,0.107188,1.04917e-22,-4.59375e-5,-3.675e-4;

		BOOST_REQUIRE(checkDisp.isApprox(checkNode->getDisplacements(lc1),1e-4));
	}
	
	BOOST_AUTO_TEST_CASE( topopt_SIMP )
	{ // benchmarked with 88-line matlab code from DTU
		sd_model sd1;
		namespace geom = bso::utilities::geometry;
		
		component::constraint c0(0);
		component::constraint c1(1);
		component::constraint c2(2);
		component::constraint c3(3);
		component::constraint c4(4);
		component::constraint c5(5);
		
		component::load_case lc1("vertical load");
		component::load l1(lc1, 1,1);
		
		component::structure str1("flat_shell",{{"E",1},{"thickness",1},{"poisson",0.3}});
		
		auto p1 = sd1.addPoint({0,20,0});
		auto p2 = sd1.addPoint({60,0,0});
		p2->addConstraint(c1);
		p1->addLoad(l1);

		auto line1 = sd1.addGeometry(geom::line_segment({{0,0,0},{0,20,0}}));
		line1->addConstraint(c0);
		
		auto quad1 = sd1.addGeometry(geom::quadrilateral({{0,0,0},{0,20,0},{20,20,0},{20,0,0}}));
		auto quad2 = sd1.addGeometry(geom::quadrilateral({{20,0,0},{20,20,0},{40,20,0},{40,0,0}}));
		auto quad3 = sd1.addGeometry(geom::quadrilateral({{40,0,0},{40,20,0},{60,20,0},{60,0,0}}));
		
		quad1->addStructure(str1);
		quad2->addStructure(str1);
		quad3->addStructure(str1);
		quad1->addConstraint(c2); quad1->addConstraint(c3); quad1->addConstraint(c4);
		quad2->addConstraint(c2); quad2->addConstraint(c3); quad2->addConstraint(c4);
		quad3->addConstraint(c2); quad3->addConstraint(c3); quad3->addConstraint(c4);
		
		sd1.mesh(20);
		sd1.topologyOptimization<topology_optimization::SIMP>(0.5,1.5,3.0,0.2,1e-2);
		
		double compliance = 0;
		
		for (const auto& i : sd1.getFEA()->getElements())
		{
			compliance += i->getTotalEnergy();
		}
		BOOST_REQUIRE(abs(compliance/101.5963 - 1) < 1e-5);
	}
	
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace structural_design_test