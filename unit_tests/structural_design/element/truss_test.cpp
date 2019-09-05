#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_truss_element"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/element/truss.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace element_test {
using namespace bso::structural_design::element;

BOOST_AUTO_TEST_SUITE( sd_truss_test )

	BOOST_AUTO_TEST_CASE( initialize_from_initializer_list )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		BOOST_REQUIRE_NO_THROW(truss t1(1,1.0,2.0,{&n1,&n2}));
	}
	
	BOOST_AUTO_TEST_CASE( initialize_from_container )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		
		std::vector<node*> nl1 = {&n1,&n2};
		BOOST_REQUIRE_NO_THROW(truss t1(1,1.0,2.0,nl1));
	}
	
	BOOST_AUTO_TEST_CASE( getProperty )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1e5;
		double A = 100;
		truss t1(1,E,A,{&n1,&n2});
		
		BOOST_REQUIRE(t1.getProperty("A") == 100);
		BOOST_REQUIRE(t1.getProperty("E") == 1e5);
		BOOST_REQUIRE(abs(t1.getProperty("Emin") - 0.1) < 1e-9);
		BOOST_REQUIRE(t1.getProperty("E0") == 1e5);
		BOOST_REQUIRE_THROW(t1.getProperty("B"), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( isTruss )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1e5;
		double A = 100;
		truss t1(1,E,A,{&n1,&n2});
		
		BOOST_REQUIRE(t1.isTruss());
		BOOST_REQUIRE(!t1.isBeam());
		BOOST_REQUIRE(!t1.isFlatShell());
		BOOST_REQUIRE(!t1.isQuadHexahedron());
	}
	
	BOOST_AUTO_TEST_CASE( ID )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1e5;
		double A = 100;
		truss t1(250,E,A,{&n1,&n2});
		
		BOOST_REQUIRE(t1.ID() == 250);
	}
	
	BOOST_AUTO_TEST_CASE( visualize )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1e5;
		double A = 100;
		truss t1(1,E,A,{&n1,&n2});
		
		BOOST_REQUIRE(t1.visualize());
		t1.visualize() = false;
		BOOST_REQUIRE(!t1.visualize());
	}
	
	BOOST_AUTO_TEST_CASE( density )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1e5;
		double A = 100;
		truss t1(1,E,A,{&n1,&n2});
		
		BOOST_REQUIRE( t1.getDensity() == 1);
		t1.updateDensity(0.3);
		BOOST_REQUIRE( t1.getDensity() == 0.3);
	}
	
	BOOST_AUTO_TEST_CASE( stiffness_terms )
	{
		node n1({0,0,0},1);
		node n2({1,1,1},2);
		double E = 1;
		double A = 1;
		truss t1(1,E,A,{&n1,&n2});
		
		unsigned long DOFCount = 3;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		t1.generateEFT();
		
		std::vector<triplet> triplets;
		auto t1Triplets = t1.getSMTriplets();
		triplets.insert(triplets.end(), t1Triplets.begin(), t1Triplets.end());

		Eigen::SparseMatrix<double> GSM;
		GSM.resize(9,9);
		GSM.resize(DOFCount, DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());
		
		for (unsigned int i = 0; i < 9; ++i)
		{
			for (unsigned int j = 0; j < 9; ++j)
			{
				if (i < 3 || j < 3)
					BOOST_REQUIRE(GSM.coeff(i,j) == 0);
				else if ((i < 6 && j < 6) ^ (i > 5 && j > 5))
					BOOST_REQUIRE(abs(GSM.coeff(i,j) - 0.19245) < 1e-4);
				else
					BOOST_REQUIRE(abs(GSM.coeff(i,j) + 0.19245) < 1e-4);
			}
		}
	}
	
	BOOST_AUTO_TEST_CASE( energy_from_displacement )
	{
		node n1({0,0,0},1);
		node n2({1,0,0},2);
		double E = 1;
		double A = 1;
		truss t1(1,E,A,{&n1,&n2});
		n1.addConstraint(1);
		
		unsigned long DOFCount = 0;
		n1.generateNFT(DOFCount);
		n2.generateNFT(DOFCount);
		t1.generateEFT();
		
		bso::structural_design::component::load_case lc_test("test_case");
		Eigen::VectorXd displacementValues(DOFCount);
		displacementValues << 0.1,0,0.2,0,0;
		std::map<bso::structural_design::component::load_case, Eigen::VectorXd> displacements;
		displacements[lc_test] = displacementValues;
		n1.addDisplacements(displacements);
		n2.addDisplacements(displacements);
		
		t1.computeResponse(lc_test);
		
		BOOST_REQUIRE(abs(t1.getEnergy(lc_test) - 0.005) < 1e-9);
	}
	
	BOOST_AUTO_TEST_CASE( benchmark_1 )
	{ // following example found on: http://ocw.ump.edu.my/pluginfile.php/9827/mod_resource/content/2/10_Space_Truss_Example.pdf
		node n1({0,0,0},1);
		node n2({0.320,1.500,0.32*tan(60*M_PI/180.0)/3.0},2);
		node n3({0.640,0,0},3);
		node n4({0.320,0,0.32*tan(60*M_PI/180.0)},4);
		double E = 1e7; // [kN/m²]
		double A = M_PI*pow(2e-2,2); // [m²]
		std::vector<node*> constrainedNodes = {&n1, &n3, &n4};
		std::vector<node*> allNodes = {&n1, &n2, &n3, &n4};
		for (auto& i : constrainedNodes)
		{
			i->addConstraint(0);
			i->addConstraint(1);
			i->addConstraint(2);	
		}
		
		std::vector<truss> trussElements;
		trussElements.push_back(truss(1,E,A,{&n2,&n1}));
		trussElements.push_back(truss(2,E,A,{&n2,&n3}));
		trussElements.push_back(truss(3,E,A,{&n2,&n4}));
		
		unsigned long DOFCount = 0;
		for (auto& i : allNodes) i->generateNFT(DOFCount);
		for (auto& i : trussElements) i.generateEFT();
		
		std::vector<triplet> triplets;
		for (const auto& i : trussElements)
		{
			auto temp = i.getSMTriplets();
			triplets.insert(triplets.end(), temp.begin(), temp.end());
		}

		Eigen::SparseMatrix<double> GSM;
		GSM.resize(0,0);
		GSM.resize(DOFCount, DOFCount);
		GSM.setFromTriplets(triplets.begin(), triplets.end());

		Eigen::Vector3d loads;
		loads << 0, -0.2, 0;
		Eigen::Vector3d displacements = Eigen::SimplicialLLT<Eigen::SparseMatrix<double> >(GSM).solve(loads);
		
		Eigen::Vector3d checkDisplacement;
		checkDisplacement << 0,-8.693e-6,0;
		BOOST_REQUIRE((displacements - checkDisplacement).isZero(1e-9));
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace element_test