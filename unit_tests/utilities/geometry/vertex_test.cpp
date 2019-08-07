#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "vertex"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/utilities/geometry.hpp>

#include <stdexcept>
#include <iostream>
#include <iterator>
#include <vector>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace geometry_test {
using namespace bso::utilities::geometry;

BOOST_AUTO_TEST_SUITE( vertex_tests )

	BOOST_AUTO_TEST_CASE( empty_init ) {
		std::vector<double> check = {0,0,0};
		vertex v1;
		
		BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), check.begin(), check.end());
	}
	BOOST_AUTO_TEST_CASE( eigen_vector_init ) {
		std::vector<double> check = {1,2,3};
		Eigen::Vector3d eigenVector;
		eigenVector << 1,2,3;
		vertex v1(eigenVector);
		
		BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), check.begin(), check.end());
	}
	
	BOOST_AUTO_TEST_CASE( arg_init ) {
		std::vector<double> check = {1,2,3};
		vertex v1 = {1,2,3};
		vertex v2;
		v2 << 1,2,3;
		vertex v3;
		v3 = {1,2,3};
		
		BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), check.begin(), check.end());
		BOOST_CHECK_EQUAL_COLLECTIONS(v2.begin(), v2.end(), check.begin(), check.end());
		BOOST_CHECK_EQUAL_COLLECTIONS(v3.begin(), v3.end(), check.begin(), check.end());
		BOOST_REQUIRE_THROW(vertex({0,0}), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( based_for_loop ) {
		std::vector<double> check = {2,3,4};
		vertex v1 = {1,2,3};
		for (auto& i : v1) i++;
		
		BOOST_CHECK_EQUAL_COLLECTIONS(v1.begin(), v1.end(), check.begin(), check.end());
	}
	
	BOOST_AUTO_TEST_CASE(round ) {
		
		vertex v1 = {0.1,0.0001,10};
		v1.round(3);
		vertex v2 = {0.1,0.0001,10};
		v2.round();
		vertex v3 = {0.1,0.0001,10};
		v3.round(-2);
		vertex v4 = {0.5,0.49,0.51};
		v4.round();
		
		BOOST_REQUIRE(v1 == vertex({0.1,0,10}));
		BOOST_REQUIRE(v2 == vertex({0,0,10}));
		BOOST_REQUIRE(v3 == vertex({0,0,0}));
		BOOST_REQUIRE(v4 == vertex({1,0,1}));
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace geometry_test