#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "quadrilateral"
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

BOOST_AUTO_TEST_SUITE( quadrilateral_tests )
	
	BOOST_AUTO_TEST_CASE( empty_init )
	{
		quadrilateral q1;
		std::stringstream check;
		check << q1;
		BOOST_REQUIRE(check.str() == "{}");
	}
	
	BOOST_AUTO_TEST_CASE( initializer_list_init )
	{
		quadrilateral q1 = {{0,0,0},{1,0,0},{1,1,0},{0,1,0}};
		quadrilateral q2 = {{0,0,0},{1,0,0},{0,1,0},{1,1,0}};
		std::stringstream check1, check2;
		check1 << q1;
		check2 << q2;
		BOOST_REQUIRE(check1.str() == "{{0,0,0},{1,0,0},{1,1,0},{0,1,0}}");
		BOOST_REQUIRE(check2.str() == "{{0,0,0},{1,0,0},{1,1,0},{0,1,0}}" ||
									check2.str() == "{{0,0,0},{0,1,0},{1,1,0},{1,0,0}}");
		BOOST_REQUIRE_THROW(quadrilateral({{0,0,0},{1,0,0}}), std::invalid_argument);
		
		vertex p1 = {0,0,0};
		vertex p2 = {1,0,0};
		vertex p3 = {0,1,0};
		vertex p4 = {1,1,0};
		vertex p5 = {0.5,1.5,0};
		vertex p6 = {0.4,0.4,0};
		quadrilateral q3 = {p3,p2,p4,p1};
		std::stringstream check3;
		check3 << q3;
		BOOST_REQUIRE(check3.str() == "{{0,1,0},{1,1,0},{1,0,0},{0,0,0}}" ||
									check3.str() == "{{0,1,0},{0,0,0},{1,0,0},{1,1,0}}");
		BOOST_REQUIRE_THROW(quadrilateral({p1,p2}), std::invalid_argument);
		BOOST_REQUIRE_THROW(quadrilateral({p1,p2,p2,p3}), std::invalid_argument);
		BOOST_REQUIRE_THROW(quadrilateral({p1,p2,p3,p4,p5}), std::invalid_argument);
		BOOST_REQUIRE_THROW(quadrilateral({p1,p2,p3,p6}), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( container_init )
	{
		std::vector<vertex> cont1 = {{0,0,0},{1,0,0},{0.5,1,0}};
		triangle t1(cont1);
		std::stringstream check1;
		check1 << t1;
		BOOST_REQUIRE(check1.str() == "{{0,0,0},{1,0,0},{0.5,1,0}}");
		BOOST_REQUIRE_THROW(triangle(std::vector<vertex>({{0,0,0},{1,1,1}})), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( copy_init )
	{
		quadrilateral q1 = {{0,0,0},{1,0,0},{1,1,0},{0,1,0}};
		quadrilateral q2 = q1;
		std::stringstream check1;
		check1 << q2;

		BOOST_REQUIRE(check1.str() == "{{0,0,0},{1,0,0},{1,1,0},{0,1,0}}");
		
		std::stringstream check2;
		for (const auto& i : q2.getLines())
		{
			check2 << i << "\n";
		}
		BOOST_REQUIRE(check2.str() == "{{0,0,0},{1,0,0}}\n{{1,0,0},{1,1,0}}\n{{1,1,0},{0,1,0}}\n{{0,1,0},{0,0,0}}\n");
	}
	
	BOOST_AUTO_TEST_CASE( is_inside_or_on )
	{
		quadrilateral q = {{-1,-1,0},{1,-1,0},{-1,1,0},{1,1,0}};
		vertex p1 = {0,0,0};
		vertex p2 = {1,0,0};
		vertex p3 = {0.9999,0,0};
		vertex p4 = {0.9,0,0};
		vertex p5 = {0,0,0.0001};
		vertex p6 = {0,0,0.1};
		vertex p7 = {1,1,0};
		vertex p8 = {1.0001,0,0};
		vertex p9 = {1.01,0,0};
		vertex p10= {-1,-1,0};
		vertex p11= { 1,-1,0};
		vertex p12= {-1, 1,0};
		vertex p13= { 1, 1,0};
		
		BOOST_REQUIRE( q.isInside(p1));
		BOOST_REQUIRE(!q.isInside(p2));
		BOOST_REQUIRE(!q.isInside(p3));
		BOOST_REQUIRE( q.isInside(p4));
		BOOST_REQUIRE( q.isInside(p5));
		BOOST_REQUIRE(!q.isInside(p6));
		
		BOOST_REQUIRE( q.isInsideOrOn(p1));
		BOOST_REQUIRE( q.isInsideOrOn(p2));
		BOOST_REQUIRE( q.isInsideOrOn(p3));
		BOOST_REQUIRE( q.isInsideOrOn(p4));
		BOOST_REQUIRE( q.isInsideOrOn(p5));
		BOOST_REQUIRE(!q.isInsideOrOn(p6));
		BOOST_REQUIRE( q.isInsideOrOn(p7));
		BOOST_REQUIRE( q.isInsideOrOn(p8));
		BOOST_REQUIRE(!q.isInsideOrOn(p9));
		BOOST_REQUIRE( q.isInsideOrOn(p10));
		BOOST_REQUIRE( q.isInsideOrOn(p11));
		BOOST_REQUIRE( q.isInsideOrOn(p12));
		BOOST_REQUIRE( q.isInsideOrOn(p13));
	}
	
	BOOST_AUTO_TEST_CASE( is_inside_or_on_2 )
	{
		quadrilateral q1 = {{6575,5150,28171},{7690,5150,28171},{7690,5150,35130},{6575,5150,35130}};
		vertex p1 = {7087,5150,31389};

		BOOST_REQUIRE( q1.isInside(p1));
		BOOST_REQUIRE( q1.isInsideOrOn(p1));
		
		quadrilateral q2 = {{9170,11997,62242},{9170,11997,12015},{9170,12497,12015},{9170,12497,62242}};
		vertex p2 = {9170,11997,59242};
		
		BOOST_REQUIRE(!q2.isInside(p2));
		BOOST_REQUIRE( q2.isInsideOrOn(p2));
	}
	
	BOOST_AUTO_TEST_CASE( is_coplanar )
	{
		quadrilateral q = {{1,0,0},{1,1,0},{0,1,0},{0,0,0}};
		vertex p1 = {0,0,0};
		vertex p2 = {0,0,0.001};
		vertex p3 = {0,0,0.01};
		vertex p4 = {0,0,0.01};
		
		BOOST_REQUIRE( q.isCoplanar(p1));
		BOOST_REQUIRE( q.isCoplanar(p2));
		BOOST_REQUIRE(!q.isCoplanar(p3));
		BOOST_REQUIRE( q.isCoplanar(p4,1e-1));
	}
	
	BOOST_AUTO_TEST_CASE( get_area )
	{
		quadrilateral q1 = {{1,0,0},{1,1,0},{0,1,0},{0,0,0}};
		quadrilateral q2 = {{1,3,2},{4,5,0},{2,0,4},{5,2,2}};

		BOOST_REQUIRE(abs(q1.getArea() - 1.0) < 1e-9);
		BOOST_REQUIRE(abs(q2.getArea() - sqrt(189)) < 1e-9);
	}
	
	BOOST_AUTO_TEST_CASE( intersects_with_line )
	{
		quadrilateral q = {{1,0,0},{1,1,0},{0,1,0},{0,0,0}};
		line_segment l1 = {{1,0.5,-1},{1,0.5,1}};
		line_segment l2 = {{0.9,0.5,-1},{0.9,0.5,1}};
		line_segment l3 = {{0.9999,0.5,-1},{0.9999,0.5,1}};
		line_segment l4 = {{0.9,0.5,0.0001},{0.9,0.5,1}};
		line_segment l5 = {{0.9,0.5,0.1},{0.9,0.5,1}};
		vertex p1,p2,p3,p4,p5;
		
		BOOST_REQUIRE(!q.intersectsWith(l1,p1));
		BOOST_REQUIRE(!q.intersectsWith(l1));
		BOOST_REQUIRE( q.intersectsWith(l2,p2));
		BOOST_REQUIRE( q.intersectsWith(l2));
		BOOST_REQUIRE(!q.intersectsWith(l2,1e0));
		BOOST_REQUIRE(!q.intersectsWith(l3,p3));
		BOOST_REQUIRE(!q.intersectsWith(l3));
		BOOST_REQUIRE( q.intersectsWith(l4,p4));
		BOOST_REQUIRE( q.intersectsWith(l4));
		BOOST_REQUIRE(!q.intersectsWith(l5,p5));
		BOOST_REQUIRE(!q.intersectsWith(l5));
		BOOST_REQUIRE((p2-vertex({0.9,0.5,0})).isZero(1e-9));
		BOOST_REQUIRE((p4-vertex({0.9,0.5,0})).isZero(1e-9));
	}

	BOOST_AUTO_TEST_CASE( same_side )
	{
		quadrilateral q = {{1,0,0},{1,1,0},{0,1,0},{0,0,0}};
		vertex p1 = {0,0,0};
		vertex p2 = {0,0,1};
		vertex p3 = {0,0,-1};
		vertex p4 = {0,0,0.001};
		vertex p5 = {0,0,-0.1};
		
		BOOST_REQUIRE(!q.sameSide(p1,p2));
		BOOST_REQUIRE( q.sameSide(p2,p2));
		BOOST_REQUIRE(!q.sameSide(p2,p3));
		BOOST_REQUIRE(!q.sameSide(p4,p2));
		BOOST_REQUIRE( q.sameSide(p5,p3));
	}
	
	BOOST_AUTO_TEST_CASE( is_parallel )
	{
		quadrilateral q = {{1,0,0},{1,1,0},{0,1,0},{0,0,0}};
		vector v1 = {5,5,0};
		vector v2 = {5,5,0.001};
		vector v3 = {5,5,0.1};
		vector v4 = {5,0,5};
		
		BOOST_REQUIRE( q.isParallel(v1));
		BOOST_REQUIRE( q.isParallel(v2));
		BOOST_REQUIRE(!q.isParallel(v3));
		BOOST_REQUIRE(!q.isParallel(v4));
	}
	
	BOOST_AUTO_TEST_CASE( get_normal )
	{
		quadrilateral q1 = {{1,0,0},{1,1,0},{0,1,0},{0,0,0}};
		BOOST_REQUIRE((q1.normal() - vector({0,0,1})).isZero(1e-9));
	}

	
BOOST_AUTO_TEST_SUITE_END()
} // namespace geometry_test