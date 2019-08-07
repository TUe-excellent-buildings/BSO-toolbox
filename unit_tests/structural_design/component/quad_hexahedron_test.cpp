#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_quad_hexahedrononent"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/component/quad_hexahedron.hpp>

#include <sstream>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace component_test {
using namespace bso::structural_design::component;

BOOST_AUTO_TEST_SUITE( sd_quad_hexahedrononent )
	
	BOOST_AUTO_TEST_CASE( initialize_from_container )
	{
		point p1(1,{-1,-1,-1});
		point p2(2,{ 1,-1,-1});
		point p3(3,{ 1, 1,-1});
		point p4(4,{-1, 1,-1});
		point p5(5,{-1,-1, 1});
		point p6(6,{ 1,-1, 1});
		point p7(7,{ 1, 1, 1});
		point p8(8,{-1, 1, 1});
		std::vector<point> pl1 = {p1, p2, p3, p4, p5, p6, p7, p8};
		quad_hexahedron qh1(pl1);
		
		BOOST_REQUIRE(p1.isSameAs(qh1[0]));
		BOOST_REQUIRE(p2.isSameAs(qh1[1]));
		BOOST_REQUIRE(p3.isSameAs(qh1[2]));
		BOOST_REQUIRE(p4.isSameAs(qh1[3]));
		BOOST_REQUIRE(p5.isSameAs(qh1[4]));
		BOOST_REQUIRE(p6.isSameAs(qh1[5]));
		BOOST_REQUIRE(p7.isSameAs(qh1[6]));
		BOOST_REQUIRE(p8.isSameAs(qh1[7]));
	}

	BOOST_AUTO_TEST_CASE( initialize_from_initializer_list )
	{
		point p1(1,{-1,-1,-1});
		point p2(2,{ 1,-1,-1});
		point p3(3,{ 1, 1,-1});
		point p4(4,{-1, 1,-1});
		point p5(5,{-1,-1, 1});
		point p6(6,{ 1,-1, 1});
		point p7(7,{ 1, 1, 1});
		point p8(8,{-1, 1, 1});
		quad_hexahedron qh1({p1, p2, p3, p4, p5, p6, p7, p8});
		
		BOOST_REQUIRE(p1.isSameAs(qh1[0]));
		BOOST_REQUIRE(p2.isSameAs(qh1[1]));
		BOOST_REQUIRE(p3.isSameAs(qh1[2]));
		BOOST_REQUIRE(p4.isSameAs(qh1[3]));
		BOOST_REQUIRE(p5.isSameAs(qh1[4]));
		BOOST_REQUIRE(p6.isSameAs(qh1[5]));
		BOOST_REQUIRE(p7.isSameAs(qh1[6]));
		BOOST_REQUIRE(p8.isSameAs(qh1[7]));
	}
	
	BOOST_AUTO_TEST_CASE( quad_hexahedron_structure )
	{
		point p1(1,{-1,-1,-1});
		point p2(2,{ 1,-1,-1});
		point p3(3,{ 1, 1,-1});
		point p4(4,{-1, 1,-1});
		point p5(5,{-1,-1, 1});
		point p6(6,{ 1,-1, 1});
		point p7(7,{ 1, 1, 1});
		point p8(8,{-1, 1, 1});
		quad_hexahedron qh1({p1, p2, p3, p4, p5, p6, p7, p8});
		
		structure st1("quad_hexahedron",{{"poisson",0.3},{"E",1e5}});
		qh1.addStructure(st1);
		
		BOOST_REQUIRE(!qh1.hasTruss());
		BOOST_REQUIRE(!qh1.hasBeam());
		BOOST_REQUIRE(!qh1.hasFlatShell());
		BOOST_REQUIRE( qh1.hasQuadHexahedron());

		BOOST_REQUIRE(qh1.structureBegin()->poisson() == 0.3);
		BOOST_REQUIRE(qh1.structureBegin()->E() == 1e5);
	}
	
	BOOST_AUTO_TEST_CASE( invalid_structure )
	{
		point p1(1,{-1,-1,-1});
		point p2(2,{ 1,-1,-1});
		point p3(3,{ 1, 1,-1});
		point p4(4,{-1, 1,-1});
		point p5(5,{-1,-1, 1});
		point p6(6,{ 1,-1, 1});
		point p7(7,{ 1, 1, 1});
		point p8(8,{-1, 1, 1});
		quad_hexahedron qh1({p1, p2, p3, p4, p5, p6, p7, p8});
		
		structure st1("truss",{{"A",100},{"E",1e5}});
		structure st2("beam",{{"width",100},{"height",400},{"poisson",0.3},{"E",1e5}});
		structure st3("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e5}});
		
		BOOST_REQUIRE_THROW(qh1.addStructure(st1),std::invalid_argument);
		BOOST_REQUIRE_THROW(qh1.addStructure(st2),std::invalid_argument);
		BOOST_REQUIRE_THROW(qh1.addStructure(st3),std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( mesh_flat_shell )
	{
		point p1(1,{-1,-1,-1});
		point p2(2,{ 1,-1,-1});
		point p3(3,{ 1, 1,-1});
		point p4(4,{-1, 1,-1});
		point p5(5,{-1,-1, 1});
		point p6(6,{ 1,-1, 1});
		point p7(7,{ 1, 1, 1});
		point p8(8,{-1, 1, 1});
		quad_hexahedron qh1({p1, p2, p3, p4, p5, p6, p7, p8});
		
		structure st1("quad_hexahedron",{{"poisson",0.3},{"E",1e5}});
		qh1.addStructure(st1);
		
		std::vector<point*> pointStore;
		qh1.mesh(2,pointStore);

		BOOST_REQUIRE(std::distance(qh1.elementPointsBegin(), qh1.elementPointsEnd()) == 8);
		BOOST_REQUIRE(std::distance(qh1.meshedPointsBegin(),  qh1.meshedPointsEnd())  == 27);
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 0))->isSameAs({-1,-1,-1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 1))->isSameAs({-1,-1, 0}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 2))->isSameAs({-1,-1, 1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 3))->isSameAs({ 0,-1,-1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 4))->isSameAs({ 0,-1, 0}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 5))->isSameAs({ 0,-1, 1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 6))->isSameAs({ 1,-1,-1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 7))->isSameAs({ 1,-1, 0}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 8))->isSameAs({ 1,-1, 1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+ 9))->isSameAs({-1, 0,-1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+10))->isSameAs({-1, 0, 0}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+11))->isSameAs({-1, 0, 1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+12))->isSameAs({ 0, 0,-1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+13))->isSameAs({ 0, 0, 0}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+14))->isSameAs({ 0, 0, 1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+15))->isSameAs({ 1, 0,-1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+16))->isSameAs({ 1, 0, 0}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+17))->isSameAs({ 1, 0, 1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+18))->isSameAs({-1, 1,-1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+19))->isSameAs({-1, 1, 0}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+20))->isSameAs({-1, 1, 1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+21))->isSameAs({ 0, 1,-1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+22))->isSameAs({ 0, 1, 0}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+23))->isSameAs({ 0, 1, 1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+24))->isSameAs({ 1, 1,-1}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+25))->isSameAs({ 1, 1, 0}));
		BOOST_REQUIRE((*(qh1.meshedPointsBegin()+26))->isSameAs({ 1, 1, 1}));
		
		qh1.clearMesh();
		BOOST_REQUIRE(std::distance(qh1.elementPointsBegin(), qh1.elementPointsEnd()) == 0);
		BOOST_REQUIRE(std::distance(qh1.meshedPointsBegin(),  qh1.meshedPointsEnd())  == 0);
	
	}
	
	BOOST_AUTO_TEST_CASE( load_test )
	{
		point p1(1,{-1,-1,-1});
		point p2(2,{ 1,-1,-1});
		point p3(3,{ 1, 1,-1});
		point p4(4,{-1, 1,-1});
		point p5(5,{-1,-1, 1});
		point p6(6,{ 1,-1, 1});
		point p7(7,{ 1, 1, 1});
		point p8(8,{-1, 1, 1});
		quad_hexahedron qh1({p1, p2, p3, p4, p5, p6, p7, p8});
		
		structure st1("quad_hexahedron",{{"poisson",0.3},{"E",1e5}});
		qh1.addStructure(st1);
		
		load_case lc1("test_case");
		load l1(&lc1,80,0);
		qh1.addLoad(l1);
		
		std::vector<point*> pointStore;
		qh1.mesh(2,pointStore);
		
		double loadSum = 0;
		for (auto& i : pointStore)
		{
			const auto itEnd = i->loadEnd();
			for (auto ite = i->loadBegin(); ite != itEnd; ++ite)
			{
				BOOST_REQUIRE(ite->magnitude() == 10);
				loadSum += ite->magnitude();
			}
		}
		BOOST_REQUIRE(loadSum == 640);
		
		BOOST_REQUIRE(std::distance(pointStore[0]->loadBegin(), pointStore[0]->loadEnd()) == 1);
		BOOST_REQUIRE(std::distance(pointStore[3]->loadBegin(), pointStore[3]->loadEnd()) == 2);
		BOOST_REQUIRE(std::distance(pointStore[4]->loadBegin(), pointStore[4]->loadEnd()) == 4);
		BOOST_REQUIRE(std::distance(pointStore[13]->loadBegin(), pointStore[13]->loadEnd()) == 8);
	}
	
	BOOST_AUTO_TEST_CASE( constraint_test )
	{
		point p1(1,{-1,-1,-1});
		point p2(2,{ 1,-1,-1});
		point p3(3,{ 1, 1,-1});
		point p4(4,{-1, 1,-1});
		point p5(5,{-1,-1, 1});
		point p6(6,{ 1,-1, 1});
		point p7(7,{ 1, 1, 1});
		point p8(8,{-1, 1, 1});
		quad_hexahedron qh1({p1, p2, p3, p4, p5, p6, p7, p8});
		
		structure st1("quad_hexahedron",{{"poisson",0.3},{"E",1e5}});
		qh1.addStructure(st1);
		
		constraint c1(5);
		qh1.addConstraint(c1);
		
		std::vector<point*> pointStore;
		qh1.mesh(2,pointStore);
		
		for (auto& i : pointStore)
		{
			const auto itEnd = i->constraintEnd();
			for (auto ite = i->constraintBegin(); ite != itEnd; ++ite)
			{
				BOOST_REQUIRE(ite->DOF() == 5);
			}
		}
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace component_test