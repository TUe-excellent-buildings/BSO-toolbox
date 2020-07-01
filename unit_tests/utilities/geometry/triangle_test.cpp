#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "triangle"
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

BOOST_AUTO_TEST_SUITE( triangle_tests )

	BOOST_AUTO_TEST_CASE( empty_init )
	{
		triangle t1;
		std::stringstream check;
		check << t1;
		BOOST_REQUIRE(check.str() == "{}");
	}
	
	BOOST_AUTO_TEST_CASE( initializer_list_init )
	{
		triangle t1 = {{0,0,0},{1,0,0},{0.5,1,0}};
		std::stringstream check1;
		check1 << t1;
		BOOST_REQUIRE(check1.str() == "{{0,0,0},{1,0,0},{0.5,1,0}}");
		BOOST_REQUIRE_THROW(triangle({{0,0,0},{1,0,0}}), std::invalid_argument);
		
		vertex p1 = {0,0,0};
		vertex p2 = {1,0,0};
		vertex p3 = {0.5,1,0};
		vertex p4 = {1,1,0};
		triangle t2 = {p1,p2,p3};
		std::stringstream check2;
		check2 << t2;
		BOOST_REQUIRE(check2.str() == "{{0,0,0},{1,0,0},{0.5,1,0}}");
		BOOST_REQUIRE_THROW(triangle({p1,p2}), std::invalid_argument);
		BOOST_REQUIRE_THROW(triangle({p1,p2,p2}), std::invalid_argument);
		BOOST_REQUIRE_THROW(triangle({p1,p2,p3,p4}), std::invalid_argument);
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
		triangle t1 = {{0,0,0},{1,0,0},{0.5,1,0}};
		triangle t2 = t1;
		std::stringstream check1;
		check1 << t2;

		BOOST_REQUIRE(check1.str() == "{{0,0,0},{1,0,0},{0.5,1,0}}");
		
		std::stringstream check2;
		for (const auto& i : t2.getLines())
		{
			check2 << i << "\n";
		}
		BOOST_REQUIRE(check2.str() == "{{0,0,0},{1,0,0}}\n{{1,0,0},{0.5,1,0}}\n{{0.5,1,0},{0,0,0}}\n");
	}
	
	BOOST_AUTO_TEST_CASE( is_inside_or_on )
	{
		triangle t = {{1,0,0},{1,1,0},{0,1,0}};
		vertex p1 = {0.5,0.5,0};
		vertex p2 = {0.75,0.75,0};
		vertex p3 = {0.50001,0.5001,0};
		vertex p4 = {0.75,0.75,0.0001};
		vertex p5 = {0.51,0.51,0};
		vertex p6 = {0.75,0.75,0.1};
		vertex p7 = {0.49,0.49,0.0};
		vertex p8 = {1,0,0};
		vertex p9 = {1,1,0};
		vertex p10= {0,1,0};
		
		BOOST_REQUIRE(!t.isInside(p1));
		BOOST_REQUIRE( t.isInside(p2));
		BOOST_REQUIRE(!t.isInside(p3));
		BOOST_REQUIRE( t.isInside(p4));
		BOOST_REQUIRE( t.isInside(p5));
		BOOST_REQUIRE(!t.isInside(p6));
		
		BOOST_REQUIRE( t.isInsideOrOn(p1));
		BOOST_REQUIRE( t.isInsideOrOn(p2));
		BOOST_REQUIRE( t.isInsideOrOn(p3));
		BOOST_REQUIRE( t.isInsideOrOn(p4));
		BOOST_REQUIRE( t.isInsideOrOn(p5));
		BOOST_REQUIRE(!t.isInsideOrOn(p6));
		BOOST_REQUIRE(!t.isInsideOrOn(p7));
		BOOST_REQUIRE( t.isInsideOrOn(p8));
		BOOST_REQUIRE( t.isInsideOrOn(p9));
		BOOST_REQUIRE( t.isInsideOrOn(p10));
	}
	
	BOOST_AUTO_TEST_CASE( is_same_as )
	{
		triangle t1 = {{1,0,0},{1,1,0},{0,1,0}};
		triangle t2 = {{1,0,0},{1,1,0},{0,1,0}};
		triangle t3 = {{1.01,0,0},{1,1,0},{0,1,0}};
		
		BOOST_REQUIRE( t1.isSameAs(t2));
		BOOST_REQUIRE(!t1.isSameAs(t3));
		BOOST_REQUIRE( t1.isSameAs(t3,1e-1));
	}
	
	BOOST_AUTO_TEST_CASE( is_coplanar )
	{
		triangle t = {{1,0,0},{1,1,0},{0,1,0}};
		vertex p1 = {0,0,0};
		vertex p2 = {0,0,0.001};
		vertex p3 = {0,0,0.01};
		vertex p4 = {0,0,0.01};
		vertex p5 = {-3,2,1};
		vertex p6 = {-4,4,3};
		
		BOOST_REQUIRE( t.isCoplanar(p1));
		BOOST_REQUIRE( t.isCoplanar(p2));
		BOOST_REQUIRE(!t.isCoplanar(p3));
		BOOST_REQUIRE( t.isCoplanar(p4,1e-1));
		t = {{3,2,-5},{-1,4,-3},{-3,8,-5}};
		BOOST_REQUIRE( t.isCoplanar(p5));
		t = {{0,-1,-1},{4,5,1},{3,9,4}};
		BOOST_REQUIRE(!t.isCoplanar(p6));
	}
	
	BOOST_AUTO_TEST_CASE( get_area )
	{
		triangle t1 = {{1,0,0},{1,1,0},{0,1,0}};
		triangle t2 = {{-1,0,1},{0,2,2},{0,-1,2}};

		BOOST_REQUIRE(t1.getArea() == 0.5);
		BOOST_REQUIRE(abs(t2.getArea() - 3*sqrt(2)/2.0) < 1e-3);
	}
	
	BOOST_AUTO_TEST_CASE( intersects_with_line )
	{
		triangle t = {{1,0,0},{1,1,0},{0,1,0}};
		line_segment l1 = {{1,0.5,-1},{1,0.5,1}};
		line_segment l2 = {{0.9,0.5,-1},{0.9,0.5,1}};
		line_segment l3 = {{0.9999,0.5,-1},{0.9999,0.5,1}};
		line_segment l4 = {{0.9,0.5,0.0001},{0.9,0.5,1}};
		line_segment l5 = {{0.9,0.5,0.1},{0.9,0.5,1}};
		vertex p1,p2,p3,p4,p5;
		
		BOOST_REQUIRE(!t.intersectsWith(l1,p1));
		BOOST_REQUIRE(!t.intersectsWith(l1));
		BOOST_REQUIRE( t.intersectsWith(l2,p2));
		BOOST_REQUIRE( t.intersectsWith(l2));
		BOOST_REQUIRE(!t.intersectsWith(l2,1e0));
		BOOST_REQUIRE(!t.intersectsWith(l3,p3));
		BOOST_REQUIRE(!t.intersectsWith(l3));
		BOOST_REQUIRE( t.intersectsWith(l4,p4));
		BOOST_REQUIRE( t.intersectsWith(l4));
		BOOST_REQUIRE(!t.intersectsWith(l5,p5));
		BOOST_REQUIRE(!t.intersectsWith(l5));
		BOOST_REQUIRE((p2-vertex({0.9,0.5,0})).isZero(1e-9));
		BOOST_REQUIRE((p4-vertex({0.9,0.5,0})).isZero(1e-9));
	}
	
	BOOST_AUTO_TEST_CASE( get_point_closest_to )
	{
		triangle t1 = {{1,0,1},{1,1,0},{0,1,0}};
		vertex p1 = {0.6,0.6,0.5};
		
		vertex pCheck;
		BOOST_REQUIRE_NO_THROW(pCheck = t1.getPointClosestTo(p1));
		BOOST_REQUIRE(pCheck.isSameAs({0.6,0.55,0.45}));
	}
	
	BOOST_AUTO_TEST_CASE( same_side )
	{
		triangle t = {{1,0,0},{1,1,0},{0,1,0}};
		vertex p1 = {0,0,0};
		vertex p2 = {0,0,1};
		vertex p3 = {0,0,-1};
		vertex p4 = {0,0,0.001};
		vertex p5 = {0,0,-0.1};
		
		BOOST_REQUIRE(!t.sameSide(p1,p2));
		BOOST_REQUIRE( t.sameSide(p2,p2));
		BOOST_REQUIRE(!t.sameSide(p2,p3));
		BOOST_REQUIRE(!t.sameSide(p4,p2));
		BOOST_REQUIRE( t.sameSide(p5,p3));
	}
	
	BOOST_AUTO_TEST_CASE( is_parallel )
	{
		triangle t = {{1,0,0},{1,1,0},{0,1,0}};
		vector v1 = {5,5,0};
		vector v2 = {5,5,0.001};
		vector v3 = {5,5,0.1};
		vector v4 = {5,0,5};
		
		BOOST_REQUIRE( t.isParallel(v1));
		BOOST_REQUIRE( t.isParallel(v2));
		BOOST_REQUIRE(!t.isParallel(v3));
		BOOST_REQUIRE(!t.isParallel(v4));
	}
	
	BOOST_AUTO_TEST_CASE( get_normal )
	{
		triangle t1 = {{1,0,0},{1,1,0},{0,1,0}};
		BOOST_REQUIRE((t1.normal() - vector({0,0,1})).isZero(1e-9));
	}
	
	BOOST_AUTO_TEST_CASE( offstream )
	{
		triangle t = {{1,sqrt(2),0},{1,4.1234e5,0},{0,53.45678,0}};
		std::stringstream check1, check2;
		check1 << std::setprecision(3) << t;
		check2 << std::setprecision(8) << t;
		
		BOOST_REQUIRE(check1.str() == "{{1,1.41,0},{1,4.12e+05,0},{0,53.5,0}}");
		BOOST_REQUIRE(check2.str() == "{{1,1.4142136,0},{1,412340,0},{0,53.45678,0}}");
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace geometry_test