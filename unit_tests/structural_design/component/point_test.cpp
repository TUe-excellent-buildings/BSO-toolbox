#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_point_component"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/component/point.hpp>

#include <sstream>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace component_test {
using namespace bso::structural_design::component;

BOOST_AUTO_TEST_SUITE( sd_point_component )
	
	
	BOOST_AUTO_TEST_CASE( initializer_list_init )
	{
		point pc1(0,{0,1,2});
		
		BOOST_REQUIRE(pc1(0) == 0);
		BOOST_REQUIRE(pc1(1) == 1);
		BOOST_REQUIRE(pc1(2) == 2);
		BOOST_REQUIRE(pc1.getID() == 0);
		BOOST_REQUIRE(std::distance(pc1.loadBegin(), pc1.loadEnd()) == 0);
		BOOST_REQUIRE(std::distance(pc1.constraintBegin(), pc1.constraintEnd()) == 0);
	}
	
	BOOST_AUTO_TEST_CASE( Eigen_vector_init )
	{
		Eigen::Vector3d v1;
		v1 << 1,2,3;
		point pc1(1,v1);
		
		BOOST_REQUIRE(pc1(0) == 1);
		BOOST_REQUIRE(pc1(1) == 2);
		BOOST_REQUIRE(pc1(2) == 3);
		BOOST_REQUIRE(pc1.getID() == 1);
		BOOST_REQUIRE(std::distance(pc1.loadBegin(), pc1.loadEnd()) == 0);
		BOOST_REQUIRE(std::distance(pc1.constraintBegin(), pc1.constraintEnd()) == 0);
	}
	
	BOOST_AUTO_TEST_CASE( loads )
	{
		point pc1(0,{0,1,2});
		load_case lc1("test_case");
		load l1(&lc1, -300, 2);
		pc1.addLoad(l1);
		
		auto ite = pc1.loadBegin();
		BOOST_REQUIRE(ite->loadCase() == &lc1);
		BOOST_REQUIRE(ite->magnitude() == -300);
		BOOST_REQUIRE(ite->DOF() == 2);
	}
	
	BOOST_AUTO_TEST_CASE( constraints )
	{
		point pc1(0,{0,1,2});
		constraint c1(1);
		pc1.addConstraint(c1);
		
		auto ite = pc1.constraintBegin();
		BOOST_REQUIRE(ite->DOF() == 1);
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace component_test