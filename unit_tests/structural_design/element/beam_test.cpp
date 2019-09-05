#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_beam_element"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/element/beam.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace element_test {
using namespace bso::structural_design::element;

BOOST_AUTO_TEST_SUITE( sd_beam_test )
	
	BOOST_AUTO_TEST_CASE( initialize_from_initializer_list )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		BOOST_REQUIRE_NO_THROW(beam b1(1,1.0,20,20,0.3,{&n1,&n2}));
	}
	
	BOOST_AUTO_TEST_CASE( initialize_from_container )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		
		std::vector<node*> nl1 = {&n1,&n2};
		BOOST_REQUIRE_NO_THROW(beam b1(1,1.0,20,20,0.3,nl1));
	}
	
	BOOST_AUTO_TEST_CASE( getProperty )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 100;
		double v = 0.3;
		beam b1(1,E,w,h,v,{&n1,&n2});

		BOOST_REQUIRE(b1.getProperty("E") == 1e5);
		BOOST_REQUIRE(abs(b1.getProperty("Emin")/0.1 - 1) < 1e-9);
		BOOST_REQUIRE(b1.getProperty("E0") == 1e5);
		BOOST_REQUIRE(b1.getProperty("width") == 100);
		BOOST_REQUIRE(b1.getProperty("height") == 100);
		BOOST_REQUIRE(b1.getProperty("v") == 0.3);
		BOOST_REQUIRE(abs(b1.getProperty("Iz")/8.3333333333e6 - 1) < 1e-9);
		BOOST_REQUIRE(abs(b1.getProperty("Iy")/8.3333333333e6 - 1) < 1e-9);
		BOOST_REQUIRE(abs(b1.getProperty("J")/1.666666666667e7 - 1) < 1e-9);
		BOOST_REQUIRE(abs(b1.getProperty("G")/38461.5384615 - 1) < 1e-9);
		BOOST_REQUIRE_THROW(b1.getProperty("B"), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( isBeam )
	{
		node n1({0,0,0},1);
		node n2({3.5,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 500;
		double v = 0.3;
		beam b1(1,E,w,h,v,{&n1,&n2});
		
		BOOST_REQUIRE(!b1.isTruss());
		BOOST_REQUIRE( b1.isBeam());
		BOOST_REQUIRE(!b1.isFlatShell());
		BOOST_REQUIRE(!b1.isQuadHexahedron());
	}
	
	BOOST_AUTO_TEST_CASE( ID )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 100;
		double v = 0.3;
		beam b1(341,E,w,h,v,{&n1,&n2});
		
		BOOST_REQUIRE(b1.ID() == 341);
	}
	
	BOOST_AUTO_TEST_CASE( visualize )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 100;
		double v = 0.3;
		beam b1(1,E,w,h,v,{&n1,&n2});
		
		BOOST_REQUIRE(b1.visualize());
		b1.visualize() = false;
		BOOST_REQUIRE(!b1.visualize());
	}
	
	BOOST_AUTO_TEST_CASE( density )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 100;
		double v = 0.3;
		beam b1(1,E,w,h,v,{&n1,&n2});
		
		BOOST_REQUIRE(b1.getDensity() == 1);
		b1.updateDensity(0.3);
		BOOST_REQUIRE(b1.getDensity() == 0.3);
	}
	
	BOOST_AUTO_TEST_CASE( stiffness_terms )
	{
		node n1({0,0,0},1);
		node n2({3.5,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 400;
		double v = 0.3;
		beam b1(1,E,w,h,v,{&n1,&n2});
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		b1.generateEFT();
		
		std::vector<triplet> triplets;
		auto b1Triplets = b1.getSMTriplets();
		triplets.insert(triplets.end(), b1Triplets.begin(), b1Triplets.end());
		
		Eigen::SparseMatrix<double> GSM;
		GSM.resize(12,12);
		GSM.resize(DOFCount, DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::MatrixXd testMatrix(12,12);
		testMatrix << 1142857143,0,0,0,0,0,-1142857143,0,0,0,0,0,
									0,9.32945E+11,0,0,0,1.63265E+12,0,-9.32945E+11,0,0,0,1.63265E+12,
									0,0,1.49271E+13,0,-2.61224E+13,0,0,0,-1.49271E+13,0,-2.61224E+13,0,
									0,0,0,6.22711E+12,0,0,0,0,0,-6.22711E+12,0,0,
									0,0,-2.61224E+13,0,6.09524E+13,0,0,0,2.61224E+13,0,3.04762E+13,0,
									0,1.63265E+12,0,0,0,3.80952E+12,0,-1.63265E+12,0,0,0,1.90476E+12,
									-1142857143,0,0,0,0,0,1142857143,0,0,0,0,0,
									0,-9.32945E+11,0,0,0,-1.63265E+12,0,9.32945E+11,0,0,0,-1.63265E+12,
									0,0,-1.49271E+13,0,2.61224E+13,0,0,0,1.49271E+13,0,2.61224E+13,0,
									0,0,0,-6.22711E+12,0,0,0,0,0,6.22711E+12,0,0,
									0,0,-2.61224E+13,0,3.04762E+13,0,0,0,2.61224E+13,0,6.09524E+13,0,
									0,1.63265E+12,0,0,0,1.90476E+12,0,-1.63265E+12,0,0,0,3.80952E+12;
		
		BOOST_REQUIRE(GSM.isApprox(testMatrix.sparseView(),1e-5));
	}
	
	BOOST_AUTO_TEST_CASE( energy_from_displacement )
	{
		node n1({0,0,0},1);
		node n2({3.5,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 400;
		double v = 0.3;
		beam b1(1,E,w,h,v,{&n1,&n2});

		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		b1.generateEFT();

		bso::structural_design::component::load_case lc_test("test_case");
		Eigen::VectorXd displacementValues(DOFCount);
		displacementValues << 0.1,0,0,0,0,0,0.2,0,0,0,0,0;
		std::map<bso::structural_design::component::load_case, Eigen::VectorXd> displacements;
		
		displacements[lc_test] = displacementValues;
		n1.addDisplacements(displacements);
		n2.addDisplacements(displacements);

		b1.computeResponse(lc_test);

		BOOST_REQUIRE(abs(b1.getEnergy(lc_test)/5714285.71429 - 1) < 1e-9);
		bso::structural_design::component::load_case lc_invalid("invalid_case");
		BOOST_REQUIRE_THROW(b1.getEnergy(lc_invalid), std::runtime_error);
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_1 )
	{ 
		node n1({0,0,0},1);
		node n2({3500,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 400;
		double v = 0.3;
		beam b1(1,E,w,h,v,{&n1,&n2});

		std::vector<node*> constrainedNodes = {&n1};
		std::vector<node*> allNodes = {&n1, &n2};
		for (auto& i : constrainedNodes)
		{
			i->addConstraint(0);
			i->addConstraint(1);
			i->addConstraint(2);
			i->addConstraint(3);
			i->addConstraint(4);
			i->addConstraint(5);			
		}

		std::vector<beam> beamElements;
		beamElements.push_back(beam(1,E,w,h,v,{&n2,&n1}));

		unsigned long DOFCount = 0;
		for (auto& i : allNodes) i->generateNFT(DOFCount);
		for (auto& i : beamElements) i.generateEFT();

		std::vector<triplet> triplets;
		for (const auto& i : beamElements)
		{
			auto temp = i.getSMTriplets();
			triplets.insert(triplets.end(), temp.begin(), temp.end());
		}

		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount, DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());
		
		Eigen::VectorXd loads(6);
		loads << 300,-200,400,0,0,0;
		Eigen::VectorXd displacements(6);
		displacements = Eigen::SimplicialLLT<Eigen::SparseMatrix<double> >(GSM).solve(loads);

		Eigen::VectorXd checkDisplacement(6);
		checkDisplacement << 2.625e-4,-0.8575,0.107188,1.04917e-22,-4.59375e-5,-3.675e-4; // from abaqus, B33 Euler-Bernoulli element

		for (unsigned int i = 0; i < 6; ++i)
		{
			if (abs(displacements(i)) > 1e-15) BOOST_REQUIRE(abs(displacements(i)/checkDisplacement(i)-1) < 1e-3);
			else BOOST_REQUIRE(abs(displacements(i)-checkDisplacement(i)) < 1e-15);
		}
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_2 )
	{
		node n1({0,0,0},1);
		node n2({3500,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 400;
		double v = 0.3;
		beam b1(1,E,w,h,v,{&n1,&n2});

		std::vector<node*> constrainedNodes = {&n1};
		std::vector<node*> allNodes = {&n1, &n2};
		for (auto& i : constrainedNodes)
		{
			i->addConstraint(0);
			i->addConstraint(1);
			i->addConstraint(2);
			i->addConstraint(3);
			i->addConstraint(4);
			i->addConstraint(5);			
		}

		std::vector<beam> beamElements;
		beamElements.push_back(beam(1,E,w,h,v,{&n1,&n2})); // nodes are switched compared to benchmark_1

		unsigned long DOFCount = 0;
		for (auto& i : allNodes) i->generateNFT(DOFCount);
		for (auto& i : beamElements) i.generateEFT();

		std::vector<triplet> triplets;
		for (const auto& i : beamElements)
		{
			auto temp = i.getSMTriplets();
			triplets.insert(triplets.end(), temp.begin(), temp.end());
		}

		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount, DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::VectorXd loads(6);
		loads << 300,-200,400,0,0,0;
		Eigen::VectorXd displacements(6);
		displacements = Eigen::SimplicialLLT<Eigen::SparseMatrix<double> >(GSM).solve(loads);

		Eigen::VectorXd checkDisplacement(6);
		checkDisplacement << 2.625e-4,-0.8575,0.107188,1.04917e-22,-4.59375e-5,-3.675e-4; // from abaqus, B33 Euler-Bernoulli element
		
		for (unsigned int i = 0; i < 6; ++i)
		{
			if (abs(displacements(i)) > 1e-15) BOOST_REQUIRE(abs(displacements(i)/checkDisplacement(i)-1) < 1e-3);
			else BOOST_REQUIRE(abs(displacements(i)-checkDisplacement(i)) < 1e-15);
		}
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_3 )
	{ 
		node n1({0,0,0},1);
		node n2({3500,0,0},2);
		double E = 1e5;
		double w = 100;
		double h = 400;
		double v = 0.3;
		beam b1(1,E,w,h,v,{&n1,&n2});

		std::vector<node*> constrainedNodes = {&n1};
		std::vector<node*> allNodes = {&n2, &n1};
		for (auto& i : constrainedNodes)
		{
			i->addConstraint(0);
			i->addConstraint(1);
			i->addConstraint(2);
			i->addConstraint(3);
			i->addConstraint(4);
			i->addConstraint(5);			
		}

		std::vector<beam> beamElements;
		beamElements.push_back(beam(1,E,w,h,v,{&n1,&n2})); // nodes are switched compared to benchmark_1

		unsigned long DOFCount = 0;
		for (auto& i : allNodes) i->generateNFT(DOFCount);
		for (auto& i : beamElements) i.generateEFT();

		std::vector<triplet> triplets;
		for (const auto& i : beamElements)
		{
			auto temp = i.getSMTriplets();
			triplets.insert(triplets.end(), temp.begin(), temp.end());
		}

		Eigen::SparseMatrix<double> GSM;
		GSM.resize(DOFCount, DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::VectorXd loads(6);
		loads << 0,0,0,0,200,300; // torsional stiffness is formulated different in Abaqus (not known how) so it is not benchmarked here
		Eigen::VectorXd displacements(6);
		displacements = Eigen::SimplicialLLT<Eigen::SparseMatrix<double> >(GSM).solve(loads);

		Eigen::VectorXd checkDisplacement(6);
		checkDisplacement << 1.14389e-21,5.5125e-4,-2.29688e-5,0,1.3125e-8,3.15e-7; // from abaqus, B33 Euler-Bernoulli element

		for (unsigned int i = 0; i < 6; ++i)
		{
			if (abs(displacements(i)) > 1e-15) BOOST_REQUIRE(abs(displacements(i)/checkDisplacement(i)-1) < 1e-3);
			else BOOST_REQUIRE(abs(displacements(i)-checkDisplacement(i)) < 1e-15);
		}
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace element_test