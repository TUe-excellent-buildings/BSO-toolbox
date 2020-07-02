#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_flat_shell_element"
#endif
#include <fstream>
#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/element/flat_shell.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace element_test {
using namespace bso::structural_design::element;

BOOST_AUTO_TEST_SUITE( sd_flat_shell_test )

	BOOST_AUTO_TEST_CASE( initialize_from_initializer_list )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		BOOST_REQUIRE_NO_THROW(flat_shell fs1(1,1.0,150,0.3,{&n1,&n2,&n3,&n4}));
	}
	
	BOOST_AUTO_TEST_CASE( initialize_from_container )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		
		std::vector<node*> nl1 = {&n1,&n2,&n3,&n4};
		BOOST_REQUIRE_NO_THROW(flat_shell fs1(1,1.0,150,0.3,nl1));
	}
	
	BOOST_AUTO_TEST_CASE( getProperty )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 100;
		double v = 0.3;
		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});

		BOOST_REQUIRE(fs1.getProperty("E") == 1e5);
		BOOST_REQUIRE(abs(fs1.getProperty("Emin")/0.1 - 1) < 1e-9);
		BOOST_REQUIRE(fs1.getProperty("E0") == 1e5);
		BOOST_REQUIRE(fs1.getProperty("thickness") == 100);
		BOOST_REQUIRE(fs1.getProperty("v") == 0.3);
		BOOST_REQUIRE_THROW(fs1.getProperty("B"), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( isFlatShell )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 100;
		double v = 0.3;
		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});
		
		BOOST_REQUIRE(!fs1.isTruss());
		BOOST_REQUIRE(!fs1.isBeam());
		BOOST_REQUIRE( fs1.isFlatShell());
		BOOST_REQUIRE(!fs1.isQuadHexahedron());
	}
	
	BOOST_AUTO_TEST_CASE( ID )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 100;
		double v = 0.3;
		flat_shell fs1(7565,E,t,v,{&n1,&n2,&n3,&n4});
		
		BOOST_REQUIRE(fs1.ID() == 7565);
	}
	
	BOOST_AUTO_TEST_CASE( visualize )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 100;
		double v = 0.3;
		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});
		
		BOOST_REQUIRE(fs1.visualize());
		fs1.visualize() = false;
		BOOST_REQUIRE(!fs1.visualize());
	}
	
	BOOST_AUTO_TEST_CASE( density )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 100;
		double v = 0.3;
		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});
		
		BOOST_REQUIRE(fs1.getDensity() == 1);
		fs1.updateDensity(0.3);
		BOOST_REQUIRE(fs1.getDensity() == 0.3);
	}
	
	BOOST_AUTO_TEST_CASE( stiffness_terms )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 100;
		double v = 0.3;
		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		n3.generateNFT(DOFCount);
		n4.generateNFT(DOFCount);
		fs1.generateEFT();
		
		std::vector<triplet> triplets;
		auto fs1Triplets = fs1.getSMTriplets();
		triplets.insert(triplets.end(), fs1Triplets.begin(), fs1Triplets.end());
		
		Eigen::SparseMatrix<double> GSM;
		GSM.resize(24,24);
		GSM.resize(DOFCount, DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::MatrixXd testMatrix(24,24);
		testMatrix << 4.94505E+06,-1.78571E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-3.02198E+06,1.37363E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-2.47253E+06,1.78571E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,5.49451E+05,-1.37363E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		-1.78571E+06,4.94505E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-1.37363E+05,5.49451E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,1.78571E+06,-2.47253E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,1.37363E+05,-3.02198E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		0.00000E+00,0.00000E+00,2.28938E+10,-1.05311E+10,-1.05311E+10,0.00000E+00,0.00000E+00,0.00000E+00,-9.15751E+09,-3.20513E+09,-9.15751E+09,0.00000E+00,0.00000E+00,0.00000E+00,-4.57875E+09,-4.57875E+09,-4.57875E+09,0.00000E+00,0.00000E+00,0.00000E+00,-9.15751E+09,-9.15751E+09,-3.20513E+09,0.00000E+00,
		0.00000E+00,0.00000E+00,-1.05311E+10,1.24771E+10,2.74725E+09,0.00000E+00,0.00000E+00,0.00000E+00,-3.20513E+09,5.83791E+09,-1.78814E-07,0.00000E+00,0.00000E+00,0.00000E+00,4.57875E+09,3.31960E+09,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,9.15751E+09,5.83791E+09,3.05474E-07,0.00000E+00,
		0.00000E+00,0.00000E+00,-1.05311E+10,2.74725E+09,1.24771E+10,0.00000E+00,0.00000E+00,0.00000E+00,9.15751E+09,3.65078E-07,5.83791E+09,0.00000E+00,0.00000E+00,0.00000E+00,4.57875E+09,0.00000E+00,3.31960E+09,0.00000E+00,0.00000E+00,0.00000E+00,-3.20513E+09,-2.16067E-07,5.83791E+09,0.00000E+00,
		0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,3.81563E+08,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		-3.02198E+06,-1.37363E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,4.94505E+06,1.78571E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,5.49451E+05,1.37363E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-2.47253E+06,-1.78571E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		1.37363E+05,5.49451E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,1.78571E+06,4.94505E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-1.37363E+05,-3.02198E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-1.78571E+06,-2.47253E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		0.00000E+00,0.00000E+00,-9.15751E+09,-3.20513E+09,9.15751E+09,0.00000E+00,0.00000E+00,0.00000E+00,2.28938E+10,-1.05311E+10,1.05311E+10,0.00000E+00,0.00000E+00,0.00000E+00,-9.15751E+09,-9.15751E+09,3.20513E+09,0.00000E+00,0.00000E+00,0.00000E+00,-4.57875E+09,-4.57875E+09,4.57875E+09,0.00000E+00,
		0.00000E+00,0.00000E+00,-3.20513E+09,5.83791E+09,2.60770E-07,0.00000E+00,0.00000E+00,0.00000E+00,-1.05311E+10,1.24771E+10,-2.74725E+09,0.00000E+00,0.00000E+00,0.00000E+00,9.15751E+09,5.83791E+09,-3.57628E-07,0.00000E+00,0.00000E+00,0.00000E+00,4.57875E+09,3.31960E+09,0.00000E+00,0.00000E+00,
		0.00000E+00,0.00000E+00,-9.15751E+09,-2.98023E-07,5.83791E+09,0.00000E+00,0.00000E+00,0.00000E+00,1.05311E+10,-2.74725E+09,1.24771E+10,0.00000E+00,0.00000E+00,0.00000E+00,3.20513E+09,2.38419E-07,5.83791E+09,0.00000E+00,0.00000E+00,0.00000E+00,-4.57875E+09,0.00000E+00,3.31960E+09,0.00000E+00,
		0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,3.81563E+08,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		-2.47253E+06,1.78571E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,5.49451E+05,-1.37363E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,4.94505E+06,-1.78571E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-3.02198E+06,1.37363E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		1.78571E+06,-2.47253E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,1.37363E+05,-3.02198E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-1.78571E+06,4.94505E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-1.37363E+05,5.49451E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		0.00000E+00,0.00000E+00,-4.57875E+09,4.57875E+09,4.57875E+09,0.00000E+00,0.00000E+00,0.00000E+00,-9.15751E+09,9.15751E+09,3.20513E+09,0.00000E+00,0.00000E+00,0.00000E+00,2.28938E+10,1.05311E+10,1.05311E+10,0.00000E+00,0.00000E+00,0.00000E+00,-9.15751E+09,3.20513E+09,9.15751E+09,0.00000E+00,
		0.00000E+00,0.00000E+00,-4.57875E+09,3.31960E+09,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-9.15751E+09,5.83791E+09,3.57628E-07,0.00000E+00,0.00000E+00,0.00000E+00,1.05311E+10,1.24771E+10,2.74725E+09,0.00000E+00,0.00000E+00,0.00000E+00,3.20513E+09,5.83791E+09,-1.19209E-07,0.00000E+00,
		0.00000E+00,0.00000E+00,-4.57875E+09,0.00000E+00,3.31960E+09,0.00000E+00,0.00000E+00,0.00000E+00,3.20513E+09,-2.38419E-07,5.83791E+09,0.00000E+00,0.00000E+00,0.00000E+00,1.05311E+10,2.74725E+09,1.24771E+10,0.00000E+00,0.00000E+00,0.00000E+00,-9.15751E+09,3.57628E-07,5.83791E+09,0.00000E+00,
		0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,3.81563E+08,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		5.49451E+05,1.37363E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-2.47253E+06,-1.78571E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-3.02198E+06,-1.37363E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,4.94505E+06,1.78571E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		-1.37363E+05,-3.02198E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,-1.78571E+06,-2.47253E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,1.37363E+05,5.49451E+05,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,1.78571E+06,4.94505E+06,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,
		0.00000E+00,0.00000E+00,-9.15751E+09,9.15751E+09,-3.20513E+09,0.00000E+00,0.00000E+00,0.00000E+00,-4.57875E+09,4.57875E+09,-4.57875E+09,0.00000E+00,0.00000E+00,0.00000E+00,-9.15751E+09,3.20513E+09,-9.15751E+09,0.00000E+00,0.00000E+00,0.00000E+00,2.28938E+10,1.05311E+10,-1.05311E+10,0.00000E+00,
		0.00000E+00,0.00000E+00,-9.15751E+09,5.83791E+09,-3.20375E-07,0.00000E+00,0.00000E+00,0.00000E+00,-4.57875E+09,3.31960E+09,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,3.20513E+09,5.83791E+09,2.38419E-07,0.00000E+00,0.00000E+00,0.00000E+00,1.05311E+10,1.24771E+10,-2.74725E+09,0.00000E+00,
		0.00000E+00,0.00000E+00,-3.20513E+09,2.01166E-07,5.83791E+09,0.00000E+00,0.00000E+00,0.00000E+00,4.57875E+09,0.00000E+00,3.31960E+09,0.00000E+00,0.00000E+00,0.00000E+00,9.15751E+09,-3.57628E-07,5.83791E+09,0.00000E+00,0.00000E+00,0.00000E+00,-1.05311E+10,-2.74725E+09,1.24771E+10,0.00000E+00,
		0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,0.00000E+00,3.81563E+08;
		
		BOOST_REQUIRE(GSM.isApprox(testMatrix.sparseView(),1e-5));
	}

	BOOST_AUTO_TEST_CASE( energy_from_displacement )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 100;
		double v = 0.3;
		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		n3.generateNFT(DOFCount);
		n4.generateNFT(DOFCount);
		fs1.generateEFT();
		
		bso::structural_design::component::load_case lc_test("test_case");
		Eigen::VectorXd displacementValues(DOFCount);
		displacementValues << 0,0.5,0,0,0,0,0.2,0,0,0,0,0,0.2,0,0,0.01,0,0.001,0,0,0,0,0,0;
		std::map<bso::structural_design::component::load_case, Eigen::VectorXd> displacements;
		displacements[lc_test] = displacementValues;
		n1.addDisplacements(displacements);
		n2.addDisplacements(displacements);
		n3.addDisplacements(displacements);
		n4.addDisplacements(displacements);

		fs1.computeResponse(lc_test);
		BOOST_REQUIRE(abs(fs1.getEnergy(lc_test)            / 1.62679e6 - 1) < 1e-5);
		BOOST_REQUIRE(abs(fs1.getEnergy(lc_test,"normal")   / 842491    - 1) < 1e-5);
		BOOST_REQUIRE(abs(fs1.getEnergy(lc_test,"shear")    / 160256    - 1) < 1e-5);
		BOOST_REQUIRE(abs(fs1.getEnergy(lc_test,"bending")  / 623855    - 1) < 1e-5);
		BOOST_REQUIRE(abs((fs1.getEnergy(lc_test,"normal")
										 + fs1.getEnergy(lc_test,"shear")
										 + fs1.getEnergy(lc_test,"bending")) / 1.6266e6  - 1) < 1e-5);
										
		bso::structural_design::component::load_case lc_invalid("invalid_case");
		BOOST_REQUIRE_THROW(fs1.getEnergy(lc_invalid), std::runtime_error);
		BOOST_REQUIRE_THROW(fs1.getEnergy(lc_invalid,"normal"), std::invalid_argument);
		BOOST_REQUIRE_THROW(fs1.getEnergy(lc_test,"bad_type"), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_1 )
	{ 
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 1;
		double v = 0.3;

		n1.addConstraint(0);
		n1.addConstraint(2);
		n2.addConstraint(2);
		n3.addConstraint(2);
		n4.addConstraint(0);
		n4.addConstraint(1);
		n4.addConstraint(2);		

		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		n3.generateNFT(DOFCount);
		n4.generateNFT(DOFCount);
		fs1.generateEFT();
		
		std::vector<triplet> triplets = fs1.getSMTriplets();
		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount,DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::VectorXd loads;
		loads.setZero(DOFCount);
		loads(4) = 1e6;
		loads(9) = 1e6;
		Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > solver;
		solver.compute(GSM);
		BOOST_REQUIRE(solver.info() == Eigen::Success);
		Eigen::VectorXd displacements = solver.solve(loads);
		
		Eigen::VectorXd checkDisplacements(DOFCount);
		checkDisplacements.setZero(17);
		checkDisplacements(0) = -6; // dy = v*dx
		checkDisplacements(4) = 20; // dx = (F*L/(E*A)) (theory is linear so exact solution)
		checkDisplacements(5) = -6;
		checkDisplacements(9) = 20;
		
		BOOST_REQUIRE(displacements.isApprox(checkDisplacements, 1e-3));

		bso::structural_design::component::load_case lc_test("test_case");
		std::map<bso::structural_design::component::load_case, Eigen::VectorXd> displacementsnew;
		displacementsnew[lc_test] = displacements;
		n1.addDisplacements(displacementsnew);
		n2.addDisplacements(displacementsnew);
		n3.addDisplacements(displacementsnew);
		n4.addDisplacements(displacementsnew);

		fs1.computeResponse(lc_test);
		Eigen::VectorXd Stress = fs1.getStress();
		Eigen::VectorXd checkStress(3);
		checkStress << 1e6,0,0; // sx = F/A, sy = sxy = 0
		double VMStress = fs1.getStressAtCenter();
		double checkVMStress = VMStress / 1e6 - 1;

		BOOST_REQUIRE(Stress.isApprox(checkStress, 1e-3));
		BOOST_REQUIRE(checkVMStress < 1e-3);
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_2 )
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 1;
		double v = 0.3;

		n1.addConstraint(0);
		n1.addConstraint(2);
		n1.addConstraint(4);
		n4.addConstraint(0);
		n4.addConstraint(1);
		n4.addConstraint(2);
		n4.addConstraint(4);

		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		n3.generateNFT(DOFCount);
		n4.generateNFT(DOFCount);
		fs1.generateEFT();
		
		std::vector<triplet> triplets = fs1.getSMTriplets();
		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount,DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::VectorXd loads;
		loads.setZero(DOFCount);
		loads(7)  = -1e6;
		loads(13) = -1e6;
		Eigen::VectorXd displacements;
		displacements.setZero(DOFCount);
		displacements = Eigen::SimplicialLLT<Eigen::SparseMatrix<double> >(GSM).solve(loads);

		Eigen::VectorXd checkDisplacements(DOFCount);
		checkDisplacements.setZero(17);
		checkDisplacements(1)  = -36; // results from abaqus
		checkDisplacements(5)  = 240;
		checkDisplacements(6)  = -36;
		checkDisplacements(7)  = -240;
		checkDisplacements(11) = 240;
		checkDisplacements(12) = 36;
		checkDisplacements(13) = -240;
		checkDisplacements(15) = 36;	

		BOOST_REQUIRE(displacements.isApprox(checkDisplacements, 1e-3));

		bso::structural_design::component::load_case lc_test("test_case");
		std::map<bso::structural_design::component::load_case, Eigen::VectorXd> displacementsnew;
		displacementsnew[lc_test] = displacements;
		n1.addDisplacements(displacementsnew);
		n2.addDisplacements(displacementsnew);
		n3.addDisplacements(displacementsnew);
		n4.addDisplacements(displacementsnew);

		fs1.computeResponse(lc_test);
		Eigen::VectorXd Stress = fs1.getStress();
		Eigen::VectorXd checkStress(3);
		checkStress << 0,0,0; // out-of-plane loads do not cause in-plane stresses at the center plane of this element type (NOTE: stresses do occur at the top- and bottom of the flat shell element, but these are not considered (not calculated) in this toolbox) // Abaqus: sxx = +/- 6e6 (top and bottom)

		BOOST_REQUIRE(Stress.isApprox(checkStress, 1e-3));
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_3 )
	{ 
		node n1({0,0,  0},1);
		node n2({2,0.5,0},2);
		node n3({2,1.5,0},3);
		node n4({0,2,  0},4);
		double E = 1e5;
		double t = 1;
		double v = 0.3;

		n1.addConstraint(0);
		n1.addConstraint(1);
		n1.addConstraint(2);
		n2.addConstraint(2);
		n3.addConstraint(2);
		n4.addConstraint(0);
		n4.addConstraint(2);

		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		n3.generateNFT(DOFCount);
		n4.generateNFT(DOFCount);
		fs1.generateEFT();
		
		std::vector<triplet> triplets = fs1.getSMTriplets();
		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount,DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::VectorXd loads;
		loads.setZero(DOFCount);
		loads(3)  = 1e6;
		loads(8) = 1e6;
		Eigen::VectorXd displacements;
		displacements.setZero(DOFCount);
		displacements = Eigen::SimplicialLLT<Eigen::SparseMatrix<double> >(GSM).solve(loads);

		Eigen::VectorXd checkDisplacements(DOFCount);
		checkDisplacements.setZero(17); // results from abaqus
		checkDisplacements(3)  = 26.6667;
		checkDisplacements(4)  = -2;
		checkDisplacements(8)  = 26.6667;
		checkDisplacements(9)  = -6;
		checkDisplacements(13) = -8;
				
		BOOST_REQUIRE(displacements.isApprox(checkDisplacements, 1e-3));

		bso::structural_design::component::load_case lc_test("test_case");
		std::map<bso::structural_design::component::load_case, Eigen::VectorXd> displacementsnew;
		displacementsnew[lc_test] = displacements;
		n1.addDisplacements(displacementsnew);
		n2.addDisplacements(displacementsnew);
		n3.addDisplacements(displacementsnew);
		n4.addDisplacements(displacementsnew);

		fs1.computeResponse(lc_test);
		Eigen::VectorXd Stress = fs1.getStress();
		Eigen::VectorXd checkStress(3);
		checkStress << 1.33333e6,0,0; // results from abaqus
		double VMStress = fs1.getStressAtCenter();
		double checkVMStress = VMStress / 1.33333e6 - 1;

		BOOST_REQUIRE(Stress.isApprox(checkStress, 1e-3));
		BOOST_REQUIRE(checkVMStress < 1e-3);
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_4 )
	{ 
		node n1({0,0,  0},1);
		node n2({2,0.5,0},2);
		node n3({2,1.5,0},3);
		node n4({0,2,  0},4);
		double E = 1e5;
		double t = 1;
		double v = 0.3;

		n1.addConstraint(0);
		n1.addConstraint(1);
		n1.addConstraint(2);
		n1.addConstraint(4);
		n4.addConstraint(0);
		n4.addConstraint(2);
		n4.addConstraint(4);

		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		n3.generateNFT(DOFCount);
		n4.generateNFT(DOFCount);
		fs1.generateEFT();
		
		std::vector<triplet> triplets = fs1.getSMTriplets();
		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount,DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::VectorXd loads;
		loads.setZero(DOFCount);
		loads(6)  = -1e6; // node 2, moment around y-axis
		loads(12) = -1e6; // node 2, moment around y-axis
		Eigen::VectorXd displacements;
		displacements.setZero(DOFCount);
		displacements = Eigen::SimplicialLLT<Eigen::SparseMatrix<double> >(GSM).solve(loads);

		Eigen::VectorXd checkDisplacements(DOFCount);
		checkDisplacements.setZero(17); // results from abaqus
		checkDisplacements(0)  = 48;
		checkDisplacements(4)  = 338;
		checkDisplacements(5)  = 24;
		checkDisplacements(6)  = -320;
		checkDisplacements(10) = 338;
		checkDisplacements(11) = -24;
		checkDisplacements(12) = -320;
		checkDisplacements(15) = -48;
		
		// Eigen::MatrixXd che;
		// che.setZero(17,2);
		// che.block<17,1>(0,0) << displacements;
		// che.block<17,1>(0,1) << checkDisplacements;
		// std::cout << che << std::endl;
		BOOST_REQUIRE(displacements.isApprox(checkDisplacements, 1e-1));
	}

	BOOST_AUTO_TEST_CASE( benchmark_5 ) // Mixed loads
	{
		node n1({-1, 1,0},1);
		node n2({ 1, 1,0},2);
		node n3({ 1,-1,0},3);
		node n4({-1,-1,0},4);
		double E = 1e5;
		double t = 1;
		double v = 0.3;

		n1.addConstraint(0);
		n1.addConstraint(2);
		n2.addConstraint(2);
		n3.addConstraint(2);
		n4.addConstraint(0);
		n4.addConstraint(1);
		n4.addConstraint(2);

		flat_shell fs1(1,E,t,v,{&n1,&n2,&n3,&n4});

		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		n3.generateNFT(DOFCount);
		n4.generateNFT(DOFCount);
		fs1.generateEFT();

		std::vector<triplet> triplets = fs1.getSMTriplets();
		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount,DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::VectorXd loads;
		loads.setZero(DOFCount);
		loads(5) = 1e6;
		loads(9) = 1e6;
		Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > solver;
		solver.compute(GSM);
		BOOST_REQUIRE(solver.info() == Eigen::Success);
		Eigen::VectorXd displacements = solver.solve(loads);

		Eigen::VectorXd checkDisplacements(DOFCount);
		checkDisplacements.setZero(17);
		checkDisplacements(0) = 7; // results from abaqus
		checkDisplacements(4) = -33.4444;
		checkDisplacements(5) = 73.4444;
		checkDisplacements(9) = 47.4444;
		checkDisplacements(10) = 66.4444;

		BOOST_REQUIRE(displacements.isApprox(checkDisplacements, 1e-2));

		bso::structural_design::component::load_case lc_test("test_case");
		std::map<bso::structural_design::component::load_case, Eigen::VectorXd> displacementsnew;
		displacementsnew[lc_test] = displacements;
		n1.addDisplacements(displacementsnew);
		n2.addDisplacements(displacementsnew);
		n3.addDisplacements(displacementsnew);
		n4.addDisplacements(displacementsnew);

		fs1.computeResponse(lc_test);
		Eigen::VectorXd Stress = fs1.getStress();
		Eigen::VectorXd checkStress1(3), checkStress2(3);
		checkStress1 << 5.0e5,5.0e5,5.0e5; // results from abaqus
		checkStress2 << 5.0e5,5.0e5,-5.0e5; // results from abaqus (negative shear)
		double VMStress = fs1.getStressAtCenter();
		double checkVMStress = VMStress / 1e6 - 1; // results from abaqus

		BOOST_REQUIRE((Stress.isApprox(checkStress1, 1e-3) || Stress.isApprox(checkStress2,1e-3)));
		BOOST_REQUIRE(checkVMStress < 1e-3);
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace element_test