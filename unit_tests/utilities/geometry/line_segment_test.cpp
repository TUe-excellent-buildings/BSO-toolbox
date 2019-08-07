#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "line_segment"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/utilities/geometry.hpp>



/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace geometry_test {
using namespace bso::utilities::geometry;

BOOST_AUTO_TEST_SUITE( line_segment_tests )

	BOOST_AUTO_TEST_CASE( empty_init ) {
		std::vector<vertex> check = {{0,0,0},{0,0,0}};
		line_segment l1;
		
		BOOST_CHECK_EQUAL_COLLECTIONS(l1.begin(), l1.end(), check.begin(), check.end());
	}
	
	BOOST_AUTO_TEST_CASE( list_init ) {
		std::vector<vertex> check = {{1,2,3},{4,5,6}};
		line_segment l1 = {vertex({1,2,3}), vertex({4,5,6})};
		
		BOOST_CHECK_EQUAL_COLLECTIONS(l1.begin(), l1.end(), check.begin(), check.end());
	}
	
	BOOST_AUTO_TEST_CASE( container_init ) {
		std::vector<vertex> check = {{1,2,3},{4,5,6}};
		line_segment l1 = check;
		
		BOOST_CHECK_EQUAL_COLLECTIONS(l1.begin(), l1.end(), check.begin(), check.end());
	}
	
	BOOST_AUTO_TEST_CASE( vertex_init ) {
		std::vector<vertex> check = {{2,3,4},{5,6,7}};
		line_segment l1(vertex({2,3,4}), vertex({5,6,7}));
		
		BOOST_CHECK_EQUAL_COLLECTIONS(l1.begin(), l1.end(), check.begin(), check.end());
	}
	
	BOOST_AUTO_TEST_CASE( is_same_as_test ) {
		
	}
	
	BOOST_AUTO_TEST_CASE( is_on_line_test ) {
		line_segment l1 = {{1,0,0},{-1,0,0}};
		line_segment l2 = {{1,1,0},{-1,-1,0}};
		line_segment l3 = {{1,0,0},{0,0,0}};
		line_segment l4 = {{1,0,0},{-0.0001,0,0}};
		line_segment l5 = {{1,0,0},{-0.1,0,0}};
		line_segment l6 = {{1,0,0},{-1,0,0.1}};
		line_segment l7 = {{0.9,1.1,0},{-1.1,-0.9,0}};
		line_segment l8 = {{0.9999,1.0001,0},{-1.0001,-0.9999,0}};
		
		vertex vCheck = {0,0,0};
		BOOST_REQUIRE( l1.isOnLine(vCheck)); 
		BOOST_REQUIRE( l2.isOnLine(vCheck)); 
		BOOST_REQUIRE(!l3.isOnLine(vCheck)); 
		BOOST_REQUIRE(!l4.isOnLine(vCheck)); 
		BOOST_REQUIRE( l5.isOnLine(vCheck)); 
		BOOST_REQUIRE(!l6.isOnLine(vCheck)); 
		BOOST_REQUIRE(!l7.isOnLine(vCheck)); 
		BOOST_REQUIRE( l8.isOnLine(vCheck)); 
	}
	
	BOOST_AUTO_TEST_CASE( intersects_with_test ) {
		vertex vCheck1, vCheck2, vCheck3, vCheck4, vCheck5, vCheck6, 
					 vCheck7, vCheck8, vCheck9, vCheck10, vCheck11, vCheck12;
		
		line_segment l1  = {{0,1,0},{2,1,0}};
		line_segment l2  = {{1,0,0},{1,2,0}};
		BOOST_REQUIRE(l1.intersectsWith(l2,vCheck1));
		BOOST_REQUIRE(l2.intersectsWith(l1,vCheck1));
		BOOST_REQUIRE(l1.intersectsWith(l2));
		BOOST_REQUIRE(vCheck1 == vertex({1,1,0}));
		
		l1  = {{0,1,1e-4},{2,1,1e-4}};
		BOOST_REQUIRE(l1.intersectsWith(l2,vCheck2));
		BOOST_REQUIRE(l2.intersectsWith(l1,vCheck2));
		BOOST_REQUIRE(vCheck2 == vertex({1,1,0}));
		
		l1  = {{0,1,0.1},{2,1,0.1}};
		BOOST_REQUIRE(!l1.intersectsWith(l2,vCheck3));
		BOOST_REQUIRE(!l2.intersectsWith(l1,vCheck3));
		BOOST_REQUIRE(!l1.intersectsWith(l2));
		
		l1  = {{0,1,0},{0.9999,1,0}};
		BOOST_REQUIRE(l1.intersectsWith(l2,vCheck4));
		BOOST_REQUIRE(l2.intersectsWith(l1,vCheck4));
		BOOST_REQUIRE(vCheck4 == vertex({1,1,0}));
		
		l1  = {{0,1,0},{0.9,1,0}};
		BOOST_REQUIRE(!l1.intersectsWith(l2,vCheck5));
		BOOST_REQUIRE(!l2.intersectsWith(l1,vCheck5));
		
		l1  = {{1.0001,1,0},{2,1,0}};
		BOOST_REQUIRE(l1.intersectsWith(l2,vCheck6));
		BOOST_REQUIRE(l2.intersectsWith(l1,vCheck6));
		BOOST_REQUIRE(vCheck6 == vertex({1,1,0}));
		
		l1  = {{1.1,1,0},{2,1,0}};
		BOOST_REQUIRE(!l1.intersectsWith(l2,vCheck7));
		BOOST_REQUIRE(!l2.intersectsWith(l1,vCheck7));
		
		l1  = {{0,1,0},{2,1,0}};
		l2  = {{1,0.9999,0},{1,2,0}};
		BOOST_REQUIRE(l1.intersectsWith(l2,vCheck8));
		BOOST_REQUIRE(l2.intersectsWith(l1,vCheck8));
		BOOST_REQUIRE(vCheck8 == vertex({1,1,0}));
		
		l2  = {{1,0,0},{1,0.9,0}};
		BOOST_REQUIRE(!l1.intersectsWith(l2,vCheck9));
		BOOST_REQUIRE(!l2.intersectsWith(l1,vCheck9));
		
		l2  = {{1,1.0001,0},{1,2,0}};
		BOOST_REQUIRE(l1.intersectsWith(l2,vCheck10));
		BOOST_REQUIRE(l2.intersectsWith(l1,vCheck10));
		BOOST_REQUIRE(vCheck10 == vertex({1,1,0}));

		l2  = {{1,1.1,0},{1,2,0}};
		BOOST_REQUIRE(!l1.intersectsWith(l2,vCheck11));
		BOOST_REQUIRE(!l2.intersectsWith(l1,vCheck11));
		
		l2  = {{0,1,0},{1,2,0}};
		BOOST_REQUIRE(!l1.intersectsWith(l2,vCheck12));
		BOOST_REQUIRE(!l2.intersectsWith(l1,vCheck12));
	}
	
	BOOST_AUTO_TEST_CASE( get_point_closest_to ) {
		line_segment l1 = {{-1,1,0}, {1,1,0}};
		vertex p1 = {0,1,0};
		line_segment l2 = {{-3,3,-1},{-1,-3,3}};
		vertex p2 = {0,0,0};
		
		vertex pCheck;
		BOOST_REQUIRE_NO_THROW(pCheck = l1.getPointClosestTo(p1));
		BOOST_REQUIRE(pCheck.isSameAs({0,1,0}));
		BOOST_REQUIRE_NO_THROW(pCheck = l2.getPointClosestTo(p2));
		BOOST_REQUIRE(pCheck.isSameAs({-2,0,1}));
	}
	
	BOOST_AUTO_TEST_CASE( get_vector_test ) {
		line_segment l = {{1,1,1}, {3,-2,0}};
		
		BOOST_REQUIRE(l.getVector(1) == vector({2,-3,-1}));
		BOOST_REQUIRE(l.getVector(0) == vector({-2,3,1}));
	}
	
	BOOST_AUTO_TEST_CASE( get_length_test ) {
		line_segment l1 = {{0,0,0},{1,0,0}};
		line_segment l2 = {{0,0,0},{1,1,0}};
		line_segment l3 = {{0,0,0},{1,1,1}};
		line_segment l4 =	{{1,1,1}, {3,-2,0}};
		
		BOOST_REQUIRE(std::abs(l1.getLength() - 1) < 1e-9);
		BOOST_REQUIRE(std::abs(l2.getLength() - std::sqrt(2)) < 1e-9);
		BOOST_REQUIRE(std::abs(l3.getLength() - std::sqrt(3)) < 1e-9);
		BOOST_REQUIRE(std::abs(l4.getLength() - std::sqrt(14)) < 1e-9);
	}
	
	BOOST_AUTO_TEST_CASE( index_operator_test ) {
		line_segment l1 = {{1,2,3},{4,5,6}};
		const line_segment l2 = {{1,2,3},{4,5,6}};
		
		BOOST_REQUIRE(l1[1] == vertex({4,5,6}));
		BOOST_REQUIRE_THROW(l1[3],std::invalid_argument);
		l1[1] = vertex({4,5,7});
		BOOST_REQUIRE(l1[1][2] == 7);
		BOOST_REQUIRE(l2[0] == vertex({1,2,3}));
	}
	
	BOOST_AUTO_TEST_CASE( range_based_for_loop_test ) {
		line_segment l1 = {{1,2,3},{4,5,6}};
		int counter = 0;
		for (auto& i : l1)
		{
			for (auto& j : i) 
			{
				BOOST_REQUIRE(j == ++counter);
				j += 1;
				BOOST_REQUIRE(j == counter+1);
			}
		}
		
		const line_segment l2 = l1;
		counter = 1;
		for (const auto& i : l2)
		{
			for (const auto& j : i) 
			{
				BOOST_REQUIRE(j == ++counter);
			}
		}
	}
	
	BOOST_AUTO_TEST_CASE( ofstream_operator_test ) {
		line_segment l1 = {{1,std::sqrt(2),3},{4.1234e5,53.45678,6}};
		std::stringstream check1, check2;
		check1 << std::setprecision(3) << l1;
		check2 << std::setprecision(8) << l1;
		
		BOOST_REQUIRE(check1.str() == "{{1,1.41,3},{4.12e+05,53.5,6}}");
		BOOST_REQUIRE(check2.str() == "{{1,1.4142136,3},{412340,53.45678,6}}");
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace geometry_test