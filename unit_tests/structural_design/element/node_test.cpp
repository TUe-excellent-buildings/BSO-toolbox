#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_node"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/element/node.hpp>
#include <sstream>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace element_test {
using namespace bso::structural_design::element;

BOOST_AUTO_TEST_SUITE( sd_node_test )

	BOOST_AUTO_TEST_CASE( list_initialization )
	{
		node n({1.5,2.5,3.6}, 1);

		BOOST_REQUIRE(n[0] == 1.5);
		BOOST_REQUIRE(n[1] == 2.5);
		BOOST_REQUIRE(n[2] == 3.6);
		BOOST_REQUIRE(n.ID() == 1);
	}
	
	BOOST_AUTO_TEST_CASE( eigen_vector_initialization )
	{
		Eigen::Vector3d init_vec;
		init_vec << 1.2,2.4,3.6;
		node n(init_vec, 340);
		
		BOOST_REQUIRE(n[0] == 1.2);
		BOOST_REQUIRE(n[1] == 2.4);
		BOOST_REQUIRE(n[2] == 3.6);
		BOOST_REQUIRE(n.ID() == 340);
	}
	
	BOOST_AUTO_TEST_CASE( update_nodal_freedom_signature )
	{
		node n({1,2,3}, 4);
		Eigen::Vector6i dummyEFS;
		dummyEFS << 1,1,1,0,0,0;
		n.updateNFS(dummyEFS);
		
		std::vector<int> check1 = {1,1,1,0,0,0};
		for (unsigned int i = 0 ; i < 6; ++i)
		{
			BOOST_REQUIRE(check1[i] == n.getNFS(i));
		}
		
		dummyEFS << 0,1,1,0,0,1;
		n.updateNFS(dummyEFS);
		
		std::vector<int> check2 = {1,1,1,0,0,1};
		for (unsigned int i = 0 ; i < 6; ++i)
		{
			BOOST_REQUIRE(check2[i] == n.getNFS(i));
		}
	}
	
	BOOST_AUTO_TEST_CASE( add_constraint )
	{
		node n({1,2,3}, 4);
		n.addConstraint(3);
		
		std::vector<int> check1 = {0,0,0,1,0,0};
		for (unsigned int i = 0; i < 6; ++i)
		{
			BOOST_REQUIRE(n.getConstraint(i) == check1[i]);
		}

		n.addConstraint(0);
		n.addConstraint(5);
		std::vector<int> check2 = {1,0,0,1,0,1};
		for (unsigned int i = 0; i < 6; ++i)
		{
			BOOST_REQUIRE(n.getConstraint(i) == check2[i]);
		}
		
		BOOST_REQUIRE_THROW(n.addConstraint(6), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( add_load )
	{
		node n({1,2,3}, 4);
		bso::structural_design::component::load_case lc1("lc1"), lc2("lc2"), lc3("lc3");
		n.addLoad(load(lc1, 1.5, 0));
		n.addLoad(load(lc1, 2.5, 1));
		n.addLoad(load(lc1, 3.5, 2));
		n.addLoad(load(lc2, 0.5, 3));
		
		BOOST_REQUIRE_NO_THROW(n.getLoads(lc1));
		BOOST_REQUIRE_NO_THROW(n.getLoads(lc2));
		BOOST_REQUIRE(n.getLoads(lc1)(0) == 1.5);
		BOOST_REQUIRE(n.getLoads(lc1)(1) == 2.5);
		BOOST_REQUIRE(n.getLoads(lc1)(2) == 3.5);
		BOOST_REQUIRE(n.getLoads(lc2)(3) == 0.5);
		BOOST_REQUIRE_THROW(n.getLoads(lc3), std::runtime_error);
	}
	
	BOOST_AUTO_TEST_CASE( add_displacements )
	{
		node n({1,2,3}, 4);
		Eigen::Vector6i dummyEFS;
		dummyEFS << 1,1,0,0,1,0;
		n.updateNFS(dummyEFS);
		unsigned long DOFCount = 0;
		n.generateNFT(DOFCount);
		Eigen::VectorXd displacements = Eigen::Matrix<double,3,1>();
		displacements << 0.5,0.3,0.4;
		bso::structural_design::component::load_case lc1("lc1"), lc2("lc2");
		
		std::map<bso::structural_design::component::load_case,Eigen::VectorXd> tempMap;
		tempMap[lc1] = displacements;
		
		n.addDisplacements(tempMap);
		std::vector<double> check1 = {0.5,0.3,0,0,0.4,0};
		for (unsigned int i = 0; i < 6; ++i)
		{
			BOOST_REQUIRE(n.getDisplacements(lc1)[i] == check1[i]);
		}
		BOOST_REQUIRE_THROW(auto temp = n.getDisplacements(lc2), std::runtime_error);
	}
	
	BOOST_AUTO_TEST_CASE( generate_nodal_freedom_table )
	{
		node n({1,2,3}, 4);
		unsigned long NFM = 5; // nodal freedom mapping variable
		Eigen::Vector6i dummyEFS;
		dummyEFS << 1,1,0,0,1,0;
		n.addConstraint(0);
		n.updateNFS(dummyEFS);
		n.generateNFT(NFM);
		BOOST_REQUIRE(NFM == 7);
		BOOST_REQUIRE(n.getGlobalDOF(1) == 5);
		BOOST_REQUIRE(n.getGlobalDOF(4) == 6);
		BOOST_REQUIRE_THROW(n.getGlobalDOF(0), std::invalid_argument);
		BOOST_REQUIRE_THROW(n.getGlobalDOF(7), std::runtime_error);
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace element_test