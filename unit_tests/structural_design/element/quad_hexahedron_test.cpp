#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_quad_hexahedron_element"
#endif
#include <fstream>
#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/element/quad_hexahedron.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace element_test {
using namespace bso::structural_design::element;

BOOST_AUTO_TEST_SUITE( sd_quad_hexahedron_test )
	
	BOOST_AUTO_TEST_CASE( initialize_from_initializer_list )
	{
		node n1({-1,-1,-1},1);
		node n2({ 1,-1,-1},2);
		node n3({ 1, 1,-1},3);
		node n4({-1, 1,-1},4);
		node n5({-1,-1, 1},5);
		node n6({ 1,-1, 1},6);
		node n7({ 1, 1, 1},7);
		node n8({-1, 1, 1},8);
		double E = 1e5;
		double v = 0.3;
		BOOST_REQUIRE_NO_THROW(quad_hexahedron qh1(1,E,v,
													 {&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8}));
	}
	
	BOOST_AUTO_TEST_CASE( initialize_from_container )
	{
		node n1({-1,-1,-1},1);
		node n2({ 1,-1,-1},2);
		node n3({ 1, 1,-1},3);
		node n4({-1, 1,-1},4);
		node n5({-1,-1, 1},5);
		node n6({ 1,-1, 1},6);
		node n7({ 1, 1, 1},7);
		node n8({-1, 1, 1},8);
		double E = 1e5;
		double v = 0.3;
		
		std::vector<node*> nl1 = {&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8};
		BOOST_REQUIRE_NO_THROW(quad_hexahedron qh1(1,E,v,nl1));
	}
	
	BOOST_AUTO_TEST_CASE( getProperty )
	{
		node n1({-1,-1,-1},1);
		node n2({ 1,-1,-1},2);
		node n3({ 1, 1,-1},3);
		node n4({-1, 1,-1},4);
		node n5({-1,-1, 1},5);
		node n6({ 1,-1, 1},6);
		node n7({ 1, 1, 1},7);
		node n8({-1, 1, 1},8);
		double E = 1e5;
		double v = 0.3;
		quad_hexahedron qh1(1,E,v,{&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8});
													 
		BOOST_REQUIRE(qh1.getProperty("E") == 1e5);
		BOOST_REQUIRE(abs(qh1.getProperty("Emin")/0.1 - 1) < 1e-9);
		BOOST_REQUIRE(qh1.getProperty("E0") == 1e5);
		BOOST_REQUIRE(qh1.getProperty("v") == 0.3);
		BOOST_REQUIRE_THROW(qh1.getProperty("thickness"), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( isQuadHexahedron )
	{
		node n1({-1,-1,-1},1);
		node n2({ 1,-1,-1},2);
		node n3({ 1, 1,-1},3);
		node n4({-1, 1,-1},4);
		node n5({-1,-1, 1},5);
		node n6({ 1,-1, 1},6);
		node n7({ 1, 1, 1},7);
		node n8({-1, 1, 1},8);
		double E = 1e5;
		double v = 0.3;
		quad_hexahedron qh1(1,E,v,{&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8});
		
		BOOST_REQUIRE(!qh1.isTruss());
		BOOST_REQUIRE(!qh1.isBeam());
		BOOST_REQUIRE(!qh1.isFlatShell());
		BOOST_REQUIRE( qh1.isQuadHexahedron());
	}
	
	BOOST_AUTO_TEST_CASE( ID )
	{
		node n1({-1,-1,-1},1);
		node n2({ 1,-1,-1},2);
		node n3({ 1, 1,-1},3);
		node n4({-1, 1,-1},4);
		node n5({-1,-1, 1},5);
		node n6({ 1,-1, 1},6);
		node n7({ 1, 1, 1},7);
		node n8({-1, 1, 1},8);
		double E = 1e5;
		double v = 0.3;
		quad_hexahedron qh1(873,E,v,{&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8});
		
		BOOST_REQUIRE(qh1.ID() == 873);
	}
	
	BOOST_AUTO_TEST_CASE( visualize )
	{
		node n1({-1,-1,-1},1);
		node n2({ 1,-1,-1},2);
		node n3({ 1, 1,-1},3);
		node n4({-1, 1,-1},4);
		node n5({-1,-1, 1},5);
		node n6({ 1,-1, 1},6);
		node n7({ 1, 1, 1},7);
		node n8({-1, 1, 1},8);
		double E = 1e5;
		double v = 0.3;
		quad_hexahedron qh1(873,E,v,{&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8});
		
		BOOST_REQUIRE(qh1.visualize());
		qh1.visualize() = false;
		BOOST_REQUIRE(!qh1.visualize());
		BOOST_REQUIRE(!qh1.visualizeTransparant());
	}
	
	BOOST_AUTO_TEST_CASE( density )
	{
		node n1({-1,-1,-1},1);
		node n2({ 1,-1,-1},2);
		node n3({ 1, 1,-1},3);
		node n4({-1, 1,-1},4);
		node n5({-1,-1, 1},5);
		node n6({ 1,-1, 1},6);
		node n7({ 1, 1, 1},7);
		node n8({-1, 1, 1},8);
		double E = 1e5;
		double v = 0.3;
		quad_hexahedron qh1(873,E,v,{&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8});
		
		BOOST_REQUIRE(qh1.getDensity() == 1);
		qh1.updateDensity(0.432);
		BOOST_REQUIRE(qh1.getDensity() == 0.432);
	}
	
	BOOST_AUTO_TEST_CASE( stiffness_terms )
	{
		node n1({-1,-1,-1},1);
		node n2({ 1,-1,-1},2);
		node n3({ 1, 1,-1},3);
		node n4({-1, 1,-1},4);
		node n5({-1,-1, 1},5);
		node n6({ 1,-1, 1},6);
		node n7({ 1, 1, 1},7);
		node n8({-1, 1, 1},8);
		double E = 1e5;
		double v = 0.3;
		quad_hexahedron qh1(1,E,v,{&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8});
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		n3.generateNFT(DOFCount);
		n4.generateNFT(DOFCount);
		n5.generateNFT(DOFCount);
		n6.generateNFT(DOFCount);
		n7.generateNFT(DOFCount);
		n8.generateNFT(DOFCount);
		qh1.generateEFT();
		
		std::vector<triplet> triplets = qh1.getSMTriplets();
		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount,DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());
		
		Eigen::MatrixXd testMatrix(24,24);
		testMatrix.setZero(24,24);
		// testMatrix << 
		
		// BOOST_REQUIRE(GSM.isApprox(testMatrix.sparseView(),1e-5));
	}

	BOOST_AUTO_TEST_CASE( energy_from_displacement )
	{
		
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_1 )
	{ 
		node n1({-1,-1,-1},1);
		node n2({ 1,-1,-1},2);
		node n3({ 1, 1,-1},3);
		node n4({-1, 1,-1},4);
		node n5({-1,-1, 1},5);
		node n6({ 1,-1, 1},6);
		node n7({ 1, 1, 1},7);
		node n8({-1, 1, 1},8);
		double E = 2e4;
		double v = 0.3;
		
		n1.addConstraint(2); // {0:dx,1:dy,2:dz,3:rx,4:ry,5:rz}
		n2.addConstraint(0);
		n2.addConstraint(2);
		n3.addConstraint(0);
		n3.addConstraint(1);
		n3.addConstraint(2);
		n4.addConstraint(2);
		
		quad_hexahedron qh1(1,E,v,{&n1,&n2,&n3,&n4,&n5,&n6,&n7,&n8});
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		n3.generateNFT(DOFCount);
		n4.generateNFT(DOFCount);
		n5.generateNFT(DOFCount);
		n6.generateNFT(DOFCount);
		n7.generateNFT(DOFCount);
		n8.generateNFT(DOFCount);
		qh1.generateEFT();
		
		std::vector<triplet> triplets = qh1.getSMTriplets();
		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount,DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::VectorXd loads;
		loads.setZero(DOFCount);
		loads(7)  = -10; // node 5, force in z-direction
		loads(10) = -10; // node 6, force in z-direction
		loads(13) = -10; // node 7, force in z-direction
		loads(16) = -10; // node 8, force in z-direction
		Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > solver;
		solver.compute(GSM);
		BOOST_REQUIRE(solver.info() == Eigen::Success);
		Eigen::VectorXd displacements = solver.solve(loads);

		Eigen::VectorXd checkDisplacements;
		checkDisplacements.setZero(DOFCount);
		checkDisplacements(0)  = -3e-4; // results from abaqus and ansys
		checkDisplacements(1)  = -3e-4; // see graduation work Diane Schoenmaker
		checkDisplacements(2)  = -3e-4;		
		checkDisplacements(3)  = -3e-4;
		checkDisplacements(5)  = -3e-4;
		checkDisplacements(6)  = -3e-4; 
		checkDisplacements(7)  = -1e-3; 
		checkDisplacements(9)  = -3e-4;
		checkDisplacements(10) = -1e-3;
		checkDisplacements(13) = -1e-3; 
		checkDisplacements(14) = -3e-4; 
		checkDisplacements(16) = -1e-3;

		BOOST_REQUIRE(displacements.isApprox(checkDisplacements, 1e-3));
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_2 )
	{
		
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_3 )
	{ 
		
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_4 )
	{ 
		
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace element_test