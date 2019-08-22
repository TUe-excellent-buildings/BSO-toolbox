#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "vector"
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

BOOST_AUTO_TEST_SUITE( vector_tests )

	// vector is inherited from vertex, check vertex_test.cpp for some of the inherited functions
	
	BOOST_AUTO_TEST_CASE( is_perpendicular ) {
		vector v1 = {0,0,0};
		vector v2 = {1,0,0};
		vector v3 = {0,1,0};
		vector v4 = {0.1,1,0};
		vector v5 = {0.0001,1,0};
		vector v6 = {0.01,1,0};

		vector vCheck = {0,0,0};
		BOOST_REQUIRE(!v1.isPerpendicular(vCheck));
		BOOST_REQUIRE(!v2.isPerpendicular(vCheck));
		vCheck = {1,0,0};
		BOOST_REQUIRE(v3.isPerpendicular(vCheck));
		BOOST_REQUIRE(!v4.isPerpendicular(vCheck));
		BOOST_REQUIRE(v5.isPerpendicular(vCheck));
		BOOST_REQUIRE(v6.isPerpendicular(vCheck,0.1));
	}
	
	BOOST_AUTO_TEST_CASE( is_parallel ) {
		vector v1 = {0,0,0};
		vector v2 = {1,0,0};
		vector v3 = {1,0,0};
		vector v4 = {1,0.1,0};
		vector v5 = {1,0.0001,0};
		vector v6 = {1,0.01,0};
		
		vector vCheck = {0,0,0};
		BOOST_REQUIRE(!v1.isParallel(vCheck));
		BOOST_REQUIRE(!v2.isParallel(vCheck));
		vCheck = {1,0,0};
		BOOST_REQUIRE(v3.isParallel(vCheck));
		BOOST_REQUIRE(!v4.isParallel(vCheck));
		BOOST_REQUIRE(v5.isParallel(vCheck));
		BOOST_REQUIRE(v6.isParallel(vCheck,0.1));
	}
	
	BOOST_AUTO_TEST_CASE( is_codirectional ) {
		vector v1 = {0,0,0};
		vector v2 = {1,0,0};
		vector v3 = {5,0,0};
		vector v4 = {1,0.1,0};
		vector v5 = {1,0.0001,0};
		vector v6 = {1,0.1,0};
		vector v7 = {-1,0.01,0};
		
		vector vCheck = {0,0,0};
		BOOST_REQUIRE(!v1.isCodirectional(vCheck));
		BOOST_REQUIRE(!v2.isCodirectional(vCheck));
		vCheck = {1,0,0};
		BOOST_REQUIRE(v3.isCodirectional(vCheck));
		BOOST_REQUIRE(!v4.isCodirectional(vCheck));
		BOOST_REQUIRE(v5.isCodirectional(vCheck));
		BOOST_REQUIRE(v6.isCodirectional(vCheck,0.1));
		BOOST_REQUIRE(!v7.isCodirectional(vCheck,0.1));
	}
	
	BOOST_AUTO_TEST_CASE( is_vertical ) {
		vector v1 = {0,0,1};
		vector v2 = {0,0.0001,1};
		vector v3 = {0,0.01,1};
		vector v4 = {0,1,1};
		vector v5 = {0,1,0};
		vector v6 = {0,0,-1};
		
		BOOST_REQUIRE(v1.isVertical());
		BOOST_REQUIRE(v2.isVertical());
		BOOST_REQUIRE(v3.isVertical(1e-1));
		BOOST_REQUIRE(!v4.isVertical());
		BOOST_REQUIRE(!v5.isVertical());
		BOOST_REQUIRE(v6.isVertical());
	}
	
	BOOST_AUTO_TEST_CASE( is_horizontal ) {
		vector v1 = {1,1,0};
		vector v2 = {1,1,0.0001};
		vector v3 = {1,1,0.01};
		vector v4 = {1,1,1};
		vector v5 = {0,0,1};
		
		BOOST_REQUIRE(v1.isHorizontal());
		BOOST_REQUIRE(v2.isHorizontal());
		BOOST_REQUIRE(v3.isHorizontal(1e-1));
		BOOST_REQUIRE(!v4.isHorizontal());
		BOOST_REQUIRE(!v5.isHorizontal());
	}
	
	BOOST_AUTO_TEST_CASE( get_heading ) {
		vector v1 = {1,1,1};
		vector v2 = {1,-1,0};
		vector v3 = {-1,-1,0};
		vector v4 = {-1,1,0};
		vector v5 = {0,1,1};
		vector v6 = {0.001,1,0};
		vector v7 = {-0.001,1,0};
		vector v8 = {0,0,1};
		vector v9 = {0,0,-1};
		
		BOOST_REQUIRE(v1.getHeading()  == std::make_pair(45.0,35.264));
		BOOST_REQUIRE(v1.getHeading(0) == std::make_pair(45.0,35.0));
		BOOST_REQUIRE(v2.getHeading()  == std::make_pair(135.0,0.0));
		BOOST_REQUIRE(v3.getHeading()  == std::make_pair(225.0,0.0));
		BOOST_REQUIRE(v4.getHeading()  == std::make_pair(315.0,0.0));
		BOOST_REQUIRE(v5.getHeading()  == std::make_pair(0.0,45.0));
		BOOST_REQUIRE(v6.getHeading()  == std::make_pair(0.057,0.0));
		BOOST_REQUIRE(v6.getHeading(0) == std::make_pair(0.0,0.0));
		BOOST_REQUIRE(v7.getHeading()  == std::make_pair(359.943,0.0));
		BOOST_REQUIRE(v7.getHeading(0) == std::make_pair(360.0,0.0));
		BOOST_REQUIRE(v8.getHeading()  == std::make_pair(0.0,90.0));
		BOOST_REQUIRE(v9.getHeading()  == std::make_pair(0.0,-90.0));
	}
	
	BOOST_AUTO_TEST_CASE( set_heading ) {
		// this also tests the initializer with heading
		vector v1 = {0,0,1};
		vector v2 = {0,1,1};
		vector v3 = {1,1,1};
		vector v4 = {0,0,-1};
		vector v5 = {-1,0,-1};
		v2.normalize();
		v3.normalize();
		v5.normalize();
		
		vector vCheck(std::make_pair(180,90)); // initializer is tested here
		BOOST_REQUIRE((vCheck - v1).isZero(1e-3));
		vCheck.setHeading(std::make_pair(0,45));
		BOOST_REQUIRE((vCheck - v2).isZero(1e-3));
		vCheck.setHeading(std::make_pair(45,35.264));
		BOOST_REQUIRE((vCheck - v3).isZero(1e-3));
		vCheck.setHeading(std::make_pair(0,-90));
		BOOST_REQUIRE((vCheck - v4).isZero(1e-3));
		vCheck.setHeading(std::make_pair(270,-45));
		BOOST_REQUIRE((vCheck - v5).isZero(1e-3));
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace geometry_test