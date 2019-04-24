#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_truss_element"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/element/truss.hpp>

using namespace bso::structural_design::element;

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

BOOST_AUTO_TEST_SUITE( sd_node_test )
	
	BOOST_AUTO_TEST_CASE( initialization )
	{
		BOOST_REQUIRE_NO_THROW(node n1({0,0,0},1));
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